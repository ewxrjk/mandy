/* Copyright © Richard Kettlewell.
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

#if __linux__
# define SYMBOL(s) s
#else
# define SYMBOL(s) _##s
#endif

#if __amd64__
        .intel_syntax noprefix

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
 *   %rax       Smash, 1st return value
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

/*  int Fixed64_iterate(Fixed64 zx[rdi], Fixed64 zy[rsi],
 *                      Fixed64 cx[rdx], Fixed64 cy[rcx],
 *                      double *r2p[r8],
 *                      int maxiters[r9);
 */
#if R2LIMIT > 4
/*
 * Register allocation:
 * rax,rdx      Accumulator for multiplications
 * rbx          iterations
 * rcx          cy
 * rdi          zx
 * rsi          zy
 * r8           r2p
 * r9           maxiters
 * r10,r11      zx^2
 * r12,r13      zy^2
 * r14          cx
 * r15          constants
 *
 * Although the inputs are in 8.56 format, some of the intermediates
 * are in 16.112 format (i.e. what you get if you multiply two 8.56
 * values together).
 */

        .globl  SYMBOL(Fixed64_iterate)
SYMBOL(Fixed64_iterate):
        push    rbx
        push    r12
        push    r13
        push    r14
        push    r15
        mov     r14,rdx
        mov     rbx,0                          // iterations = 0
        movabs  r15,R2LIMIT << 48              // r15 = R2LIMIT, in 16.112 format
        .align  16
iter1:
        // We keep z^2 in 16.112 format, since it may overflow 8.56
        mov     rax,rdi
        imul    rdi                            // rdx:rax = zx^2
        mov     r11,rax
        mov     r10,rdx                        // r10:r11 = zx^2
        mov     rax,rsi
        imul    rsi                            // rdx:rax = zy^2
        mov     r13,rax
        mov     r12,rdx                        // r12:r13 = zy^2
        add     rax,r11
        adc     rdx,r10                        // rdx:rax = zx^2+zy^2
        cmp     rdx,r15
        jge     2f                             // escape if zx^2+zy^2 > R2LIMIT
        shrd    r11,r10,56                     // r11 = zx^2
        mov     rax,rdi
        imul    rsi                            // rdx:rax = zx * zy
        shrd    r13,r12,56                     // r13 = zy^2
        shrd    rax,rdx,56                     // rax = zx * zy
        lea     rsi,[rcx+rax*2]                // zy = 2 * zx * zy + cy
        sub     r11,r13                        // r11 = zx^2 - zy^2
        lea     rdi,[r11+r14]                  // zx = zx^2 - zy^2 + cx
        inc     rbx                            // iterations += 1
        cmp     rbx,r9
        jb      iter1                          // repeat if iterations < maxiters
        // Breached iteration limit
        jmp     6f
2:
        // Escaped.  We can't return r^2 in an 8.56 since it may have
        // overflowed.  We can return it in a double however (and in fact
        // that's more useful to the caller).
        cvtsi2sd xmm0,rdx
        cvtsi2sd xmm1,rax
        // rdx had units at bit 2*56-64=48, so it needs to be
        // "shifted right" by 48 bits.
        movabs  r15,0x3CF0000000000000        // =(double)2^-48
        movq    xmm2,r15
        mulsd   xmm0,xmm2
        // rax is a further 64 bits down
        movabs  r15,0x38F0000000000000        // =(double)2^-112
        movq    xmm2,r15
        mulsd   xmm1,xmm2
        addsd   xmm0,xmm1
        movq    qword ptr [r8],xmm0
6:
        mov     rax,rbx                       // Return iteration count
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     rbx
        ret
#else
        /* For smaller R2LIMIT we try to get away with overflow
         *
         * Register allocation:
         * rax,rdx      Accumulator for multiplications
         * rbx          iterations
         * rcx          cy
         * rdi          zx
         * rsi          zy
         * r8           r2p
         * r9           maxiters
         * r10          zx^2
         * r11          zy^2
         * r12          cx
         * r13          escape limit
          */
        .globl  SYMBOL(Fixed64_iterate)
SYMBOL(Fixed64_iterate):
        push    rbx
        push    r13
        push    r12
        push    r13
        mov     r12,rdx                         // r12 <- cx
        mov     rbx,0                           // iterations = 0
        movabs  r13,R2LIMIT << 56               // r13 = R2LIMIT
        .align  16
iter2:
        mov     rax,rdi                         // rax <- zx
        imul    rdi                             // rdx:rax < zx^2 (16.112)
        shrd    rax,rdx,56                      // rax <- zx^2
        mov     r10,rax                         // r10 <- zx^2
        mov     rax,rsi                         // rax <- zy^2
        imul    rsi                             // rdx::rax <- zy^2 (16.112)
        shrd    rax,rdx,56                      // rax <- zy^2
        mov     r11,rax                         // r11 <- zy^2
        add     rax,r10                         // rax <- zx^2 + zy^2 = r^2
        cmp     rax,r13
        jge     2f                              // escape if zx^2+zy^2 > R2LIMIT
        mov     rax,rdi                         // rax <- zx
        imul    rsi                             // rdx:rax <- zx * zy (16.112)
        shrd    rax,rdx,55                      // rax <- 2 * zx * zy
        lea     rsi,[rax+rcx]                   // rdi <- 2 * zx * zy + cy (new zy)
        sub     r10,r11                         // r10 <- zx^2 - zy^2
        lea     rdi,[r10+r12]                   // rdi <- zx^2 - zy^2 + cx (new zx)
        inc     rbx                             // iterations += 1

#if 1 // 1 level of unrolling seems to be a marginal performance improvement
        cmp     rbx,r9
        jae     3f

        mov     rax,rdi                         // rax <- zx
        imul    rdi                             // rdx:rax < zx^2 (16.112)
        shrd    rax,rdx,56                      // rax <- zx^2
        mov     r10,rax                         // r10 <- zx^2
        mov     rax,rsi                         // rax <- zy^2
        imul    rsi                             // rdx::rax <- zy^2 (16.112)
        shrd    rax,rdx,56                      // rax <- zy^2
        mov     r11,rax                         // r11 <- zy^2
        add     rax,r10                         // rax <- zx^2 + zy^2 = r^2
        cmp     rax,r13
        jge     2f                              // escape if zx^2+zy^2 > R2LIMIT
        mov     rax,rdi                         // rax <- zx
        imul    rsi                             // rdx:rax <- zx * zy (16.112)
        shrd    rax,rdx,55                      // rax <- 2 * zx * zy
        lea     rsi,[rax+rcx]                   // rdi <- 2 * zx * zy + cy (new zy)
        sub     r10,r11                         // r10 <- zx^2 - zy^2
        lea     rdi,[r10+r12]                   // rdi <- zx^2 - zy^2 + cx (new zx)
        inc     rbx                             // iterations += 1
#endif
        
        cmp     rbx,r9
        jb      iter2                           // repeat if iterations < maxiters
        // Breached iteration limit
        jmp     3f
2:
        // Escaped. Return r^2 in a double.
        cvtsi2sd xmm0,rax
        movabs  rdx,0x3c70000000000000          // =(double)2^-56
        movq    xmm1,rdx
        mulsd   xmm0,xmm1
        movq    qword ptr [r8],xmm0
3:
        mov     rax,rbx                         // Return iteration count
        pop     r13
        pop     r12
        pop     r13
        pop     rbx
        ret        
      
#endif
#endif

