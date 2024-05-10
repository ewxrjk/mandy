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
 * - %rsp-128 up to %rsp (red zone) is safe to use for temporaries in leaf functions
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

#if 0
/* int Fixed128_mul(union Fixed128 *r=rdi,
 *               const union Fixed128 *a=rsi,
 *               const union Fixed128 *b=rdx) */
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
        push    rbp
        push    r12
        // Get the values into registers
        mov     r9,[rdx+8]      // Ef
        mov     r11,[rsi+8]     // Ab
        mov     r8,[rdx]        // gh
        mov     r10,[rsi]       // cd
        mov     cl,0            // sign of result
        // TODO could we stick the sign in some other register and save a push?
        // Make everything +ve but remember the intended sign of the result
        cmp     r9,0
        jge     1f
        not     r8
        add     r8,1
        not     r9
        adc     r9,0
        not     cl
1:
        cmp     r11,0
        jge     2f
        not     r10
        add     r10,1
        not     r11
        adc     r11,0
        not     cl
2:
        // Do the multiplies.
        // All multiplies are rax * something -> rdx:rax
        // Result will be accumulated in rsi:rbp:r12:<junk>
        mov     rax,r11         // Ab
        mul     r9              // Ab * Ef -> rdx:rax
        mov     rsi,rdx
        mov     rbp,rax
        //
        mov     rax,r11         // Ab
        mul     r8              // Ab * gh -> rdx:rax
        mov     r12,rax
        add     rbp,rdx
        adc     rsi,0
        //
        mov     rax,r10         // cd
        mul     r9              // cd * Ef -> rdx:rax
        add     r12,rax
        adc     rbp,rdx
        adc     rsi,0
        //
        mov     rax,r10         // cd
        mul     r8              // cd * fg -> rdx:rax
        add     r12,rdx
        adc     rbp,0
        adc     rsi,0
        // Check for overflow
        xor     rax,rax
        cmp     rsi,2147483647
        jbe     4f
        mov     rax,1
4:
        // Rearrange the top 128 bits of the answer into rsi/rbp
        shld    rsi,rbp,32
        shld    rbp,r12,32
        // Maybe round up
        bt      r12,31
        adc     rbp,0
        adc     rsi,0
        // Set the right sign
        shr     cl,1
        jnc     3f
        not     rbp
        add     rbp,1
        not     rsi
        adc     rsi,0
3:
        mov     [rdi+8],rsi
        mov     [rdi],rbp
        pop     r12
        pop     rbp
        ret
#endif

/* int Fixed128_iterate(union Fixed128 *zx=rdi, union Fixed128 *zy=rsi,
 *                   const union Fixed128 *cx=rdx, const union Fixed128 *cy=rcx,
 *                   int maxiters=r8);
 *
 * Outline register allocation (see code for more detail):
 *  rax, rdx       - destination for mul instruction
 *  rbx, rcx, rdi  - accumulator for multiplication
 *  [rsp+0]        - cx
 *  [rsp+16]       - cy
 *  r8,r9          - zx
 *  r10,r11        - zy
 *  r12,r13        - zx^2
 *  r14,r15        - zy^2 + input to square routine
 *  rsi            - iterations
 *  [rsp+32]       - maxiters
 *  rbp            - sign of zx*zy
 *
 * Registers are written in high,low order (even though memory is low-first)
 */
        .align  4
        .globl SYMBOL(Fixed128_iterate)
SYMBOL(Fixed128_iterate):
        // Registers we must preserve
        push    rbx
        push    rbp
        push    r12
        push    r13
        push    r14
        push    r15
        push    rsi
        push    rdi
        sub             rsp,48
        // Store maxiters in the red zone
        mov     [rsp+32],r8             // [rsp+32] = maxiters
        // Ditto cx/cy
        mov     rax,[rdx]
        mov     rbx,[rdx+8]
        mov     [rsp+0],rax             
        mov     [rsp+8],rbx             // [rsp+0] = cx
        mov     rax,[rcx]
        mov     rbx,[rcx+8]
        mov     [rsp+16],rax
        mov     [rsp+24],rbx            // [rsp+16] = cy
        // Load initial zx/zy
        mov     r9,[rdi]
        mov     r8,[rdi+8]              // r8,r9 = zx
        mov     r11,[rsi]
        mov     r10,[rsi+8]             // r10,r11 = zy
        // Iteration count
        xor     rsi,rsi                 // rsi = iterations = 0
        // Main loop
iterloop:
        // Compute zx^2
        mov     r14,r8
        mov     r15,r9                  // r14,r15 = zx
        movabs  rbp,0x4000000000        // rbp,<missing> = 64.0
        call    square                  // rbx,rcx = zx^2
        mov     r12,rbx
        mov     r13,rcx                 // r12,r13 = zx^2
        
        // Compute zy^2
        mov     r14,r10
        mov     r15,r11                 // r14,r15 = zy
        .att_syntax // hack to work around bizarre bug in apple assembler
        call    square                  // rbx,rcx = zy^2
        .intel_syntax noprefix
        mov     r14,rbx
        mov     r15,rcx                 // r14,r15 = zy^2
        
        // Compute zx^2 + zy^2
        add     rcx,r13
        adc     rbx,r12                 // rbx,rcx = zx^2 + zy^2
        
        // Check for escape
        cmp     rbx,rbp
        jge     escaped                 // if zx^2 + zy^2 >= 64, escaped

        // Compute zx*zy

        // Work out the sign of the result, and make the inputs positive.
        // We don't need the (correct-sign) versions any more so we
        // negate in place.
#if 1
        cmp     r8,0
        setl    bpl                     // bpl = zx < 0 ? 1 : 0
        jge     1f
        not     r9
        add     r9,1
        not     r8
        adc     r8,0                    // Now zx >= 0
1:
        cmp     r10,0
        jge     2f
        not     r11
        add     r11,1
        not     r10
        adc     r10,0                   // Now zy >= 0
        dec     bpl                     // bpl = (zx * zy) < 0 ? ±1 : 0
2:
#else
        // Branch-free version. In fact this (including its elaboration to
        // the final sign fixup and the square subroutine) is substantially
        // slower.
        xor     rbp,rbp
        cmp     r8,rbp
        setl    bpl                     // bpl = zx < 0 ? 1 : 0
        neg     rbp                     // rbp = zx < 0 ? -1 : 0
        xor     r9,rbp
        xor     r8,rbp
        sub     r9,rbp
        sbb     r8,rbp                  // r8,r9 = |zx|

        xor     rax,rax
        cmp     r10,rax
        setl    al                      // al = zy < 0 ? 1 : 0
        neg     rax                     // rax = zy < 0 ? -1 : 0
        xor     r11,rax
        xor     r10,rax
        sub     r11,rax
        sbb     r10,rax                 // r10,r11 = |zy|
        xor     rbp,rax                 // rbp = (zx * zy) < 0 ? -1 : 0
#endif

        // rbx,rcx,rdi are used as the accumulator
#if 1
        mov     rax,r8
        mul     r10                     // rdx,rax = zx.h * zy.h
        mov     rcx,rax
        mov     rbx,rdx                 // rbx,rcx = zx.h * zy.h
        //
        mov     rax,r8
        mul     r11                     // rdx,rax = zx.h * zy.l
        mov     rdi,rax
        add     rcx,rdx
        adc     rbx,0                   // rbx,rcx,rdi = (zx.h * zy.h)<<64 + zx.h * zy.l
        //
        mov     rax,r9
        mul     r10                     // rdx,rax = zx.l * zy.h
        add     rdi,rax
        adc     rcx,rdx
        adc     rbx,0                   // rbx,rcx,rdi = (zx.h * zy.h)<<64 + zx.h * zy.l + zx.l * zy.h
        //
        mov     rax,r9
        mul     r11                     // rdx,rax = zx.l * zy.l
        add     rdi,rdx
        adc     rcx,0
        adc     rbx,0                   // rbx,rcx,rdi = (zx.h * zy.h)<<64 + zx.h * zy.l + zx.l * zy.h + (zx.l * zy.l) >> 64
#else // A bit slower on my computer
        mov     rdx,r8
        mulx    rbx,rcx,r10             // rbx,rcx = zx.h * zy.h
        
        mulx    rdx,rdi,r11             // rdx,rdi = zx.h * zy.l
        add     rcx,rdx
        adc     rbx,0                   // rbx,rcx,rdi = (zx.h * zy.h)<<64 + zx.h * zy.l

        mov     rdx,r9
        mulx    rdx,rax,r10             // rdx,rax = zx.l * zy.h
        add     rdi,rax
        adc     rcx,rdx
        adc     rbx,0                   // rbx,rcx,rdi = (zx.h * zy.h)<<64 + zx.h * zy.l + zx.l * zy.h

        mov     rdx,r9
        mulx    rdx,rax,r11             // rdx,rax = zx.l * zy.l
        add     rdi,rdx
        adc     rcx,0
        adc     rbx,0                   // rbx,rcx,rdi = (zx.h * zy.h)<<64 + zx.h * zy.l + zx.l * zy.h + (zx.l * zy.l) >> 64
#endif
        // Rearrange top 128 bits into two registers
        //
        // The units bit (bits 32 of zx.h, zy.h) has ended up in bit 0 of rbx,
        // so we need to shift everything 32 bits left.
        shld    rbx,rcx,32
        shld    rcx,rdi,32              // rbx,rcx = (top 128 bits of) zx * zy
        // Double and maybe round up
        bt      rdi,31
        adc     rcx,rcx
        adc     rbx,rbx                 // rbx,rcx = 2 * zx * zy
        // Set sign
        shr     bpl,1
        jnc     3f
        not     rcx
        add     rcx,1
        not     rbx
        adc     rbx,0
3:
        // Now we have:
        //                                 r12,r13 = zx^2
        //                                 r14,r15 = zy^2
        //                                 rbx,rcx = 2.zx.zy

        // Compute zx_next = 2.zx.zy+cy
        mov     r11,[rsp+16]
        mov     r10,[rsp+24]            // r10,r11 = cy
        add     r11,rcx
        adc     r10,rbx                 // r10,r11 = 2.zx.zy + cy = zx_next

        // Compute zy_next = zx^2-zy^2+cx
        mov r9,[rsp+0]
        mov r8,[rsp+8]                  // r8,r9 = cx
        add     r9,r13
        adc     r8,r12                  // r8,r9 = zx^2 + cx
        sub     r9,r15
        sbb     r8,r14                  // r8,r9 = zx^2 - zy^2 + cx = zy_next

        // Increment iteration count
        inc     rsi                     // iterations += 1
        // Only go back round the loop if not too big
        cmp     rsi,[rsp+32]
        .att_syntax // hack to work around bizarre bug in apple assembler
        jb      iterloop
        .intel_syntax noprefix
escaped:
        // Retrieve iteration count
        mov     rax,rsi
        // Return r^2 in the first argument (rather idiosyncratic)
        add rsp,48
        pop     rdi
        pop     rsi
        mov     [rdi],rcx               // 'zx' = zx^2 + zy^2
        mov     [rdi+8],rbx
        // Restore registers
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     rbp
        pop     rbx
        ret

        // Let rbx,rcx=(r14,r15)^2
        // rax,rdx are used by the MUL insn
        // rbx,rcx,rdi are used as accumulator
        //
        // In the language of Fixed128_mul, we have r14=Ab=Ef, r15=cd=gh
        .align  4
square:
        // We can throw away sign information as the answer is never -ve
        cmp     r14,0
        jge     1f
        not     r15
        add     r15,1
        not     r14
        adc     r14,0
1:
        // As with Fixed128_mul we start at the most significant end and
        // work our way down
        mov     rax,r14
        mul     r14                     // rdx,rax = h * h
        mov     rcx,rax
        mov     rbx,rdx                 // rbx,rcx = h * h
        //
        mov     rax,r14
        mul     r15                     // rdx,rax = h * l
        mov     rdi,rax
        add     rcx,rdx
        adc     rbx,0                   // rbx,rcx,rdi = (h * h) << 64 + h * l

        // We take advantage of (x+y)^2=(x^2+2xy+y^2) to skip a multiply
        add     rdi,rax
        adc     rcx,rdx
        adc     rbx,0                   // rbx,rcx,rdi = (h * h) << 64 + 2 * h * l

        mov     rax,r15
        mul     r15                     // rdx,rax = l * l
        add     rdi,rdx
        adc     rcx,0
        adc     rbx,0                   // rbx,rcx,rdi = (h * h) << 64 + 2 * h * l + (l * l) >> 64
        // Rearrange top 128 bits into two registers
        shld    rbx,rcx,32
        shld    rcx,rdi,32              // rbx,rcx = (top 128 bits of) (hl * hl)
        // Maybe round up
        bt      rdi,31
        adc     rcx,0
        adc     rbx,0                   // rbx,rcx = hl * hl
        ret
#endif
