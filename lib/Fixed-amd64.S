/* Copyright © 2010 Richard Kettlewell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <config.h>

#if HAVE_ASM_AMD64
	.intel_syntax noprefix

#if __linux__
# define SYMBOL(s) s
#else
# define SYMBOL(s) _##s
#endif

/*
 * Some possibly-relevant rules:
 * - %rsp+8 points at arguments
 * - %rsp+8 mod 16 = 0
 * - %rsp-128 up to %rsp (red zone) is safe to use for temporaries
 * - pointer arguments count as class INTEGER
 * - INTEGER arguments are passed in %rdi %rsi %rdx %rcx %r8 %r9
 *   (other classes get other registers)
 * - when you run out the stack is used in reverse order
 *
 * Register usage table:
 *   %rax	Smash, 1st return value
 *   %rbx       preserve
 *   %rcx       4th arg, smash
 *   %rdx       3rd arg, 2nd return value, smash
 *   %rsi       2nd arg, smash
 *   %rdi       1st arg, smash
 *   %rsp       stack pointer, duh
 *   %rbp       preserve
 *   %r8        5th arg, smash
 *   %r9        6th arg, smash
 *   %r10       smash
 *   %r11       smash
 *   %r12-%r15  preserve
 *   %xmm0-15   smash
 * See: http://www.x86-64.org/documentation/abi.pdf
 */

#if NFIXED == 4
	.text

/* void Fixed_shl_unsigned(struct Fixed *a=rdi) */
	.globl SYMBOL(Fixed_shl_unsigned)
SYMBOL(Fixed_shl_unsigned):
	shl	[qword ptr rdi],1
	rcl	[qword ptr rdi+8],1
	ret

/* void Fixed_shr_unsigned(struct Fixed *a=rdi) */
	.globl SYMBOL(Fixed_shr_unsigned)
SYMBOL(Fixed_shr_unsigned):
	shr	[qword ptr rdi+8],1
	rcr	[qword ptr rdi],1
	ret

/* void Fixed_add(struct Fixed *r=rdi,
 *                const struct Fixed *a=rsi,
 *                const struct Fixed *b=rdx) */
	.globl SYMBOL(Fixed_add)
SYMBOL(Fixed_add):
	mov	rax,[rsi]
	mov	rcx,[rsi+8]
	add	rax,[rdx]
	mov	[rdi],rax
	adc	rcx,[rdx+8]
	mov	[rdi+8],rcx
	ret

/* void Fixed_sub(struct Fixed *r=rdi,
 *                const struct Fixed *a=rsi,
 *                const struct Fixed *b=rdx) */
	.globl SYMBOL(Fixed_sub)
SYMBOL(Fixed_sub):
	mov	rax,[rsi]
	mov	rcx,[rsi+8]
	sub	rax,[rdx]
	mov	[rdi],rax
	sbb	rcx,[rdx+8]
	mov	[rdi+8],rcx
	ret

/* int Fixed_neg(struct Fixed *r=rdi, const struct Fixed *a=rsi) */
	.globl SYMBOL(Fixed_neg)
SYMBOL(Fixed_neg):
	mov	rdx,[rsi]
	mov	rax,[rsi+8]
	mov	rcx,rax		// save for overflow detection
	not	rdx
	add	rdx,1		// we need the carry so we cannot use incq
	mov	[rdi],rdx
	not	rax
	adc	rax,0
	mov	[rdi+8],rax
	xor	rax,rcx	        // yields 1... if the top bit has changed
	not	rax		// yields 1... if the top bit has NOT changed
	shr	rax,63		// 0 for success, 1 for overflow
	ret

/* int Fixed_mul(struct Fixed *r=rdi,
 *               const struct Fixed *a=rsi,
 *               const struct Fixed *b=rdx) */
	// We have, representing each byte with a letter:
	//
	//        AAAA.bbbb cccc dddd
	//      * EEEE.ffff gggg hhhh
	//
	//   ==== ==== ==== ====                      Ab * Ef
	//             ==== ==== ==== ====            Ab * gh
	//             ==== ==== ==== ====            cd * Ef
	//                       ==== ==== ==== ====  cd * gh
	//   <- rsi -> <- rbp -> <- r12 -> <- rax ->
	//
	.globl SYMBOL(Fixed_mul)
SYMBOL(Fixed_mul):
	push	rbp
	push	r12
	// Get the values into registers
	mov	r9,[rdx+8]	// Ef
	mov	r11,[rsi+8]	// Ab
	mov	r8,[rdx]	// gh
	mov	r10,[rsi]	// cd
	mov	cl,0		// sign of result
	// TODO could we stick the sign in some other register and save a push?
	// Make everything +ve but remember the intended sign of the result
	cmp	r9,0
	jge	1f
	not	r8
	add	r8,1
	not	r9
	adc	r9,0
	not	cl
1:
	cmp	r11,0
	jge	2f
	not	r10
	add	r10,1
	not	r11
	adc	r11,0
	not	cl
2:
	// Do the multiplies.
	// All multiplies are rax * something -> rdx:rax
	// Result will be accumulated in rsi:rbp:r12:<junk>
	mov	rax,r11		// Ab
	mul	r9		// Ab * Ef -> rdx:rax
	mov	rsi,rdx
	mov	rbp,rax
	//
	mov	rax,r11		// Ab
	mul	r8		// Ab * gh -> rdx:rax
	mov	r12,rax
	add	rbp,rdx
	adc	rsi,0
	//
	mov	rax,r10		// cd
	mul	r9		// cd * Ef -> rdx:rax
	add	r12,rax
	adc	rbp,rdx
	adc	rsi,0
	//
	mov	rax,r10		// cd
	mul	r8		// cd * fg -> rdx:rax
	add	r12,rdx
	adc	rbp,0
	adc	rsi,0
	// Check for overflow
	xor	rax,rax
	cmp	rsi,2147483647
	jbe	4f
	mov	rax,1
4:
	// Rearrange the top 128 bits of the answer into rsi/rbp
	shld	rsi,rbp,32
	shld	rbp,r12,32
	// Maybe round up
	bt	r12,31
	adc	rbp,0
	adc	rsi,0
	// Set the right sign
	cmp	cl,0
	je	3f
	not	rbp
	add	rbp,1
	not	rsi
	adc	rsi,0
3:
	mov	[rdi+8],rsi
	mov	[rdi],rbp
	pop	r12
	pop	rbp
	ret

/* int Fixed_iterate(struct Fixed *zx=rdi, struct Fixed *zy=rsi,
 *                   const struct Fixed *cx=rdx, const struct Fixed *cy=rcx,
 *                   int maxiters=r8);
 *
 * Register allocation:
 *  rax, rdx       - destination for multiplication
 *  rbx, rcx, rdi  - accumulator for multiplication
 *  xmm0,xmm1      - cx
 *  xmm2,xmm3      - cy
 *  r8,r9          - zx
 *  r10,r11        - zy
 *  r12,r13        - zx^2
 *  r14,r15        - zy^2
 *  xmm4           - iterations
 *  xmm5           - maxiters
 *  rbp            - sign of zx*zy
 */
	.globl SYMBOL(Fixed_iterate)
SYMBOL(Fixed_iterate):
	// Registers we must preserve
	push	rbx
	push	rbp
	push	r12
	push	r13
	push	r14
	push	r15
	// Store iteration count info on the stack
	movd	xmm5,r8
	xor	r8,r8
	movd	xmm4,r8
	// Get cx/cy into registers for later use
	movd	xmm1,[rdx]
	movd	xmm0,[rdx+8]
	movd	xmm3,[rcx]
	movd	xmm2,[rcx+8]
	// Load initial zx/zy
	mov	r9,[rdi]
	mov	r8,[rdi+8]
	mov	r11,[rsi]
	mov	r10,[rsi+8]
	// Main loop
iterloop:
	// Compute zx^2
	mov	r14,r8
	mov	r15,r9
	call	square
	mov	r12,rbx
	mov	r13,rcx
	// Compute zy^2
	mov	r14,r10
	mov	r15,r11
	.att_syntax // hack to work around bizarre bug in apple assembler
	call	square
	.intel_syntax noprefix
	mov	r14,rbx
	mov	r15,rcx
	// Add them up and compare against the limit
	add	rcx,r13
	adc	rbx,r12
	mov	rbp,0x400000000
	cmp	r12,rbp
	jge	escaped
	// Compute zx*zy
	// First figure out what the sign will be
	cmp	r8,0
	jge	1f
	not	r9
	add	r9,1
	not	r8
	adc	r8,0
	not	bpl
1:
	cmp	r10,0
	jge	2f
	not	r11
	add	r11,1
	not	r10
	adc	r10,0
	not	bpl
2:
	// rbx,rcx,rdi are used as the accumulator
	mov	rax,r8
	mul	r10
	mov	rcx,rax
	mov	rbx,rdx
	//
	mov	rax,r8
	mul	r11
	mov	rdi,rax
	add	rcx,rdx
	adc	rbx,0
	//
	mov	rax,r9
	mul	r10
	add	rdi,rax
	adc	rcx,rdx
	adc	rbx,0
	//
	mov	rax,r9
	mul	r11
	add	rdi,rdx
	adc	rcx,0
	adc	rbx,0
	// Rerrange top 128 bits into two registers
	shld	rbx,rcx,32
	shld	rcx,rdi,32
	// Maybe round up
	bt	rdi,31
	adc	rcx,0
	adc	rbx,0
	// Set sign
	cmp	bpl,0
	je	3f
	not	rcx
	add	rcx,1
	not	rbx
	adc	rbx,0
3:
	// Now we have rbx,rcx = zx.zy
	// Let rbx,rcx = 2.zx.zy
	add	rcx,rcx
	adc	rbx,rbx
	// Let zy=r10,r11=2.zx.zy+cy
	movd	r11,xmm1
	movd	r10,xmm0
	add	r11,rcx
	adc	r10,rbx
	// Let zy=r8,r9=zx^2-zy^2
	mov	r9,r13
	mov	r8,r12
	sub	r9,r15
	sbb	r8,r14
	// Increment iteration count
	movd	rax,xmm4
	inc	rax
	movd	xmm4,rax
	// Only go back round the loop if not too big
	movd	rbx,xmm5
	cmp	rax,rbx
	.att_syntax // hack to work around bizarre bug in apple assembler
	jb	iterloop
	.intel_syntax noprefix
escaped:
        // Retrieve iteration count
	movd	rax,xmm4
	// TODO if we're going to do smooth coloring then we'll need
	// to return latest zx/zy.
	// Restore registers
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	rbp
	pop	rbx
	ret

	// Let rbx,rcx=(r14,r15)^2
	// rax,rdx are used by the MUL insn
	// rbx,rcx,rdi are used as accumulator
	//
	// In the language of Fixed_mul, we have r14=Ab=Ef, r15=cd=gh
square:
	// We can throw away sign information as the answer is never -ve
	cmp	r14,0
	jge	1f
	not	r15
	add	r15,1
	not	r14
	adc	r15,1
1:
	// As with Fixed_mul we start at the most significant end and
	// work our way down
	mov	rax,r14
	mul	r14
	mov	rcx,rax
	mov	rbx,rdx
	//
	mov	rax,r14
	mul	r15
	mov	rdi,rax
	add	rcx,rdx
	adc	rbx,0
	//
	// We take advantage of (x+y)^2=(x^2+2xy+y^2) to skip a multiply
	add	rdi,rax
	adc	rcx,rdx
	adc	rbx,0
	//
	mov	rax,r15
	mul	r15
	add	rdi,rdx
	adc	rcx,0
	adc	rbx,0
	// Rearrange top 128 bits into two registers
	shld	rbx,rcx,32
	shld	rcx,rdi,32
	// Maybe round up
	bt	rdi,31
	adc	rcx,0
	adc	rbx,0
	ret
#endif
#endif
