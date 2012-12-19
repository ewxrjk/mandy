/* Copyright © 2010, 2012 Richard Kettlewell.
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

	.text

/* void Fixed128_shl_unsigned(struct Fixed128 *a=rdi) */
	.globl SYMBOL(Fixed128_shl_unsigned)
SYMBOL(Fixed128_shl_unsigned):
	shl	qword ptr [rdi],1
	rcl	qword ptr [rdi+8],1
	ret

/* void Fixed128_shr_unsigned(struct Fixed128 *a=rdi) */
	.globl SYMBOL(Fixed128_shr_unsigned)
SYMBOL(Fixed128_shr_unsigned):
	shr	qword ptr [rdi+8],1
	rcr	qword ptr [rdi],1
	ret

/* void Fixed128_add(struct Fixed128 *r=rdi,
 *                const struct Fixed128 *a=rsi,
 *                const struct Fixed128 *b=rdx) */
	.globl SYMBOL(Fixed128_add)
SYMBOL(Fixed128_add):
	mov	rax,[rsi]
	mov	rcx,[rsi+8]
	add	rax,[rdx]
	mov	[rdi],rax
	adc	rcx,[rdx+8]
	mov	[rdi+8],rcx
	ret

/* void Fixed128_sub(struct Fixed128 *r=rdi,
 *                const struct Fixed128 *a=rsi,
 *                const struct Fixed128 *b=rdx) */
	.globl SYMBOL(Fixed128_sub)
SYMBOL(Fixed128_sub):
	mov	rax,[rsi]
	mov	rcx,[rsi+8]
	sub	rax,[rdx]
	mov	[rdi],rax
	sbb	rcx,[rdx+8]
	mov	[rdi+8],rcx
	ret

/* int Fixed128_neg(struct Fixed128 *r=rdi, const struct Fixed128 *a=rsi) */
	.globl SYMBOL(Fixed128_neg)
SYMBOL(Fixed128_neg):
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

/* int Fixed128_mul(struct Fixed128 *r=rdi,
 *               const struct Fixed128 *a=rsi,
 *               const struct Fixed128 *b=rdx) */
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
	.globl SYMBOL(Fixed128_mul)
SYMBOL(Fixed128_mul):
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
	shr	cl,1
	jnc	3f
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

/* int Fixed128_iterate(struct Fixed128 *zx=rdi, struct Fixed128 *zy=rsi,
 *                   const struct Fixed128 *cx=rdx, const struct Fixed128 *cy=rcx,
 *                   int maxiters=r8);
 *
 * Outline register allocation (see code for more detail):
 *  rax, rdx       - destination for mul instruction
 *  rbx, rcx, rdi  - accumulator for multiplication
 *  [rsp-128]      - cx
 *  [rsp-112]      - cy
 *  r8,r9          - zx
 *  r10,r11        - zy
 *  r12,r13        - zx^2
 *  r14,r15        - zy^2 + input to square routine
 *  rsi            - iterations
 *  [rsp-88]       - maxiters
 *  rbp            - sign of zx*zy
 */
	.align	4
	.globl SYMBOL(Fixed128_iterate)
SYMBOL(Fixed128_iterate):
	// Registers we must preserve
	push	rbx
	push	rbp
	push	r12
	push	r13
	push	r14
	push	r15
	push	rsi
	push	rdi
	// Store maxiters in the red zone
	mov	[rsp-88],r8
	// Ditto cx/cy
	mov	rax,[rdx]
	mov	[rsp-128],rax
	mov	rax,[rdx+8]
	mov	[rsp-120],rax	// [rsp-128] = cx
	mov	rax,[rcx]
	mov	[rsp-112],rax
	mov	rax,[rcx+8]
	mov	[rsp-104],rax	// [rsp-112] = cy
	// Load initial zx/zy
	mov	r9,[rdi]
	mov	r8,[rdi+8]
	mov	r11,[rsi]
	mov	r10,[rsi+8]
	// Iteration count
	xor	rsi,rsi
	// Main loop
iterloop:
	// Compute zx^2
	mov	r14,r8
	mov	r15,r9
	mov	rbp,0x4000000000
	call	square
	// Compute zy^2
	mov	r14,r10
	mov	r15,r11
	mov	r12,rbx		// r12:r13 = zx^2
	mov	r13,rcx
	.att_syntax // hack to work around bizarre bug in apple assembler
	call	square
	.intel_syntax noprefix
	mov	r14,rbx
	mov	r15,rcx
	// Add them up and compare against the limit
	add	rcx,r13
	adc	rbx,r12
	cmp	rbx,rbp
	jge	escaped
	// Compute zx*zy
	// First figure out what the sign will be
	cmp	r8,0
	setl	bpl
	jge	1f
	not	r9
	add	r9,1
	not	r8
	adc	r8,0
1:
	cmp	r10,0
	jge	2f
	not	r11
	add	r11,1
	not	r10
	adc	r10,0
	dec	bpl
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
	// Rearrange top 128 bits into two registers
	shld	rbx,rcx,32
	shld	rcx,rdi,32
	// Double and maybe round up
	bt	rdi,31
	adc	rcx,rcx
	adc	rbx,rbx
	// Set sign
	shr	bpl,1
	jnc	3f
	not	rcx
	add	rcx,1
	not	rbx
	adc	rbx,0
3:
	// Now we have rbx,rcx = 2.zx.zy
	// Let zy=r10,r11=2.zx.zy+cy
	mov	r11,[rsp-112]
	mov	r10,[rsp-104]
	add	r11,rcx
	adc	r10,rbx
	// Let zy=r8,r9=zx^2-zy^2+cx
        mov     r9,[rsp-128]
        mov     r8,[rsp-120]
	add	r9,r13
	adc	r8,r12
	sub	r9,r15
	sbb	r8,r14
	// Increment iteration count
	inc	rsi
	// Only go back round the loop if not too big
	cmp	rsi,[rsp-88]
	.att_syntax // hack to work around bizarre bug in apple assembler
	jb	iterloop
	.intel_syntax noprefix
escaped:
        // Retrieve iteration count
	mov	rax,rsi
	// Return r^2 in the first argument (rather idiosyncratic)
	pop	rdi
	pop	rsi
	mov	[rdi],rcx
	mov	[rdi+8],rbx
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
	// In the language of Fixed128_mul, we have r14=Ab=Ef, r15=cd=gh
	.align	4
square:
	// We can throw away sign information as the answer is never -ve
	cmp	r14,0
	jge	1f
	not	r15
	add	r15,1
	not	r14
	adc	r14,0
1:
	// As with Fixed128_mul we start at the most significant end and
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