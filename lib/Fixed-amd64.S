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
 *
 * gas syntax is OP SRC,DEST (like traditional 68k syntax)
 * $ means 'immediate'
 * % means 'register'
 * OPl means 32-bit, OPq means 64-bit
 */

#if NFIXED == 4
	.text

/* void Fixed_shl_unsigned(struct Fixed *a=%rdi) */
	.globl _Fixed_shl_unsigned
_Fixed_shl_unsigned:
	shlq	$1,(%rdi)
	rclq	$1,8(%rdi)
	ret

/* void Fixed_shr_unsigned(struct Fixed *a=%rdi) */
	.globl _Fixed_shr_unsigned
_Fixed_shr_unsigned:
	shrq	$1,8(%rdi)
	rcrq	$1,(%rdi)
	ret

/* void Fixed_add(struct Fixed *r=%rdi,
 *                const struct Fixed *a=%rsi,
 *                const struct Fixed *b=%rdx) */
	.globl _Fixed_add
_Fixed_add:
	movq	(%rsi),%rax
	movq	8(%rsi),%rcx
	addq	(%rdx),%rax
	movq	%rax,(%rdi)
	adcq	8(%rdx),%rcx
	movq	%rcx,8(%rdi)
	ret

/* void Fixed_sub(struct Fixed *r=%rdi,
 *                const struct Fixed *a=%rsi,
 *                const struct Fixed *b=%rdx) */
	.globl _Fixed_sub
_Fixed_sub:
	movq	(%rsi),%rax
	movq	8(%rsi),%rcx
	subq	(%rdx),%rax
	movq	%rax,(%rdi)
	sbbq	8(%rdx),%rcx
	movq	%rcx,8(%rdi)
	ret

/* int Fixed_neg(struct Fixed *r=%rdi, const struct Fixed *a=%rsi) */
	.globl _Fixed_neg
_Fixed_neg:
	movq	(%rsi),%rdx
	movq	8(%rsi),%rax
	movq	%rax,%rcx	// save for overflow detection
	notq	%rdx
	addq	$1,%rdx		// we need the carry so we cannot use incq
	movq	%rdx,(%rdi)
	notq	%rax
	adcq	$0,%rax
	movq	%rax,8(%rdi)
	xorq	%rcx,%rax	// yields 1... if the top bit has changed
	notq	%rax		// yields 1... if the top bit has NOT changed
	shrq	$63,%rax	// 0 for success, 1 for overflow
	ret
#endif
#endif
