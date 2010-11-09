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
 * - preserve %rbp, %rbx, %r12-%r15
 * -
 * - %rsp points at return address
 * - %rsp+8 points at arguments
 * - %rsp+8 mod 16 = 0
 * - %rsp-128 up to %rsp (red zone) is safe to use for temporaries
 * - pointer arguments count as class INTEGER
 * - INTEGER arguments are passed in %rdi %rsi %rdx %rcx %r8 %r9
 * - other classes get other registers
 * - when you run out the stack is used in reverse order
 * - INTEGER class returns are returned via %rax
 * See: http://www.x86-64.org/documentation/abi.pdf
 *
 * gas syntax is OP SRC,DEST (like traditional 68k syntax)
 * $ means 'immediate'
 * % means 'register'
 * OPl means 32-bit, OPq means 64-bit
 */

#if NFIXED == 4
	.text

/* void Fixed_shl_unsigned(struct Fixed *a) */
	.globl _Fixed_shl_unsigned
_Fixed_shl_unsigned:
	shlq	$1,(%rdi)
	rclq	$1,8(%rdi)
	ret

/* void Fixed_shr_unsigned(struct Fixed *a) */
	.globl _Fixed_shr_unsigned
_Fixed_shr_unsigned:
	shrq	$1,8(%rdi)
	rcrq	$1,(%rdi)
	ret

#endif
#endif
