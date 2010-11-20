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

	.globl	SYMBOL(Fixed64_mul)
SYMBOL(Fixed64_mul_unsigned):
	mov	rax,rdi
	mul	rsi		// result in tdx:tax
	shrd	rax,rdx,56
	adc	rax,0		// round up
	ret
#endif

