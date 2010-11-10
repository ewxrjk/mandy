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
	.intel_syntax

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
 * See: http://www.x86-64.org/documentation/abi.pdf
 */

#if NFIXED == 4
	.text

/* void Fixed_shl_unsigned(struct Fixed *a=rdi) */
	.globl _Fixed_shl_unsigned
_Fixed_shl_unsigned:
	shl	[qword ptr rdi],1
	rcl	[qword ptr rdi+8],1
	ret

/* void Fixed_shr_unsigned(struct Fixed *a=rdi) */
	.globl _Fixed_shr_unsigned
_Fixed_shr_unsigned:
	shr	[qword ptr rdi+8],1
	rcr	[qword ptr rdi],1
	ret

/* void Fixed_add(struct Fixed *r=rdi,
 *                const struct Fixed *a=rsi,
 *                const struct Fixed *b=rdx) */
	.globl _Fixed_add
_Fixed_add:
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
	.globl _Fixed_sub
_Fixed_sub:
	mov	rax,[rsi]
	mov	rcx,[rsi+8]
	sub	rax,[rdx]
	mov	[rdi],rax
	sbb	rcx,[rdx+8]
	mov	[rdi+8],rcx
	ret

/* int Fixed_neg(struct Fixed *r=rdi, const struct Fixed *a=rsi) */
	.globl _Fixed_neg
_Fixed_neg:
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
	.globl _Fixed_mul
_Fixed_mul:
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
	jae	1f
	not	r8
	add	r8,1
	not	r9
	adc	r9,0
	not	cl
1:
	cmp	r11,0
	jae	2f
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
	// Now we must:
	//  - set the sign of the result
	//  - store the result
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
#endif
#endif
