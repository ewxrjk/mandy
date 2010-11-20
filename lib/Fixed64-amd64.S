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

	.text
	// Fixed64 Fixed64_mul(Fixed64 a[rdi], Fixed64 b[rsi])
	.globl	SYMBOL(Fixed64_mul)
SYMBOL(Fixed64_mul):
	mov	rax,rdi
	imul	rsi		// gives result in rdx:rax
	shrd	rax,rdx,56
	adc	rax,0		// round up
	ret

	// Fixed64 Fixed64_mul_unsigned(Fixed64 a[rdi], Fixed64 b[rsi])
	.globl	SYMBOL(Fixed64_mul_unsigned)
SYMBOL(Fixed64_mul_unsigned):
	mov	rax,rdi
	mul	rsi		// result in tdx:tax
	shrd	rax,rdx,56
	adc	rax,0		// round up
	ret

/*  int Fixed64_iterate(Fixed64 zx[rdi], Fixed64 zy[rsi],
 *                      Fixed64 cx[rdx], Fixed64 cy[rcx],
 *                      double *r2p[r8],
 *                      int maxiters[r9);
 *
 * Register allocation:
 * rax,rdx	Accumulator for multiplications
 * rbx		iterations
 * rcx		cy
 * rdi		zx
 * rsi		zy
 * r8		r2p
 * r9		maxiters
 * r10,r11	zx^2
 * r12,r13	zy^2
 * r14		cx
 * r15		constants
 *
 */

	.globl	SYMBOL(Fixed64_iterate)
SYMBOL(Fixed64_iterate):
	push	rbx
	push	r12
	push	r13
	push	r14
	push	r15
	mov	r14,rdx
	mov	rbx,0
	mov	r15,0x0040000000000000
1:
	// We keep z^2 in 16.112 format, since it may overflow 8.56
	mov	rax,rdi
	imul	rdi		// rdx:rax = zx^2
	mov	r11,rax
	mov	r10,rdx		// r10:r11 = zx^2
	mov	rax,rsi
	imul	rsi		// rdx:rax = zy^2
	mov	r13,rax
	mov	r12,rdx		// r12:r13 = zy^2
	add	rax,r11
	adc	rdx,r10		// rdx:rax = zx^2+zy^2
	cmp	rdx,r15
	jge	2f
	mov	rax,rdi
	imul	rsi		// rdx:rax = zx * zy
	add	rax,rax
	adc	rdx,rdx		// rdx:rax = 2 * zx * zy
	shrd	rax,rdx,56	// rax = 2 * zx * zy
	adc	rax,rcx		// rax = 2 * zx * zy + cy [plus rounding]
	mov	rsi,rax		// zy = 2 * zx * zy + cy
	sub	r11,r13
	sbb	r10,r12		// r10:r11 = zx^2 - zy^2
	shrd	r11,r10,56	// r11 = zx^2 - zy^2
	adc	r11,r14		// r11 = zx^2 - zy^2 + cy [plus rounding]
	mov	rdi,r11		// zx = zx^2 - zy^2 + cy
	inc	rbx
	cmp	rbx,r9
	jb	1b
	// Breached iteration limit
	jmp	6f
2:
	// Escaped.  We can't return r^2 in an 8.56 since it may have
	// overflowed.  We can return it in a double however (and in fact
	// that's more useful to the caller).
	cvtsi2sd	xmm0,rdx
	cvtsi2sd	xmm1,rax
	// rdx had units at bit 2*56-64=48, so it needs to be
	// "shifted right" by 48 bits.
	mov	r15,0x3CF0000000000000
	movd	xmm2,r15
	mulsd	xmm0,xmm2
	// rax is a further 64 bits down
	mov	r15,0x38F0000000000000
	movd	xmm2,r15
	mulsd	xmm1,xmm2
	addsd	xmm0,xmm1
	movd	qword ptr [r8],xmm0
6:
	mov	rax,rbx		// Return iteration count
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	rbx
	ret
#endif

