; Copyright © 2010 Richard Kettlewell.
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;

; See http://msdn.microsoft.com/en-us/library/ms235286.aspx for ABI
;
; Registers:
;   rax   return
;   rbx   preserve
;   rcx   first integer arg
;   rdx   second integer arg
;   rsi   preserve
;   rdi   preserve
;   rsp
;   rbp   preserve
;   r8    third integer arg
;   r9    fourth integer arg
;   r10   smash
;   r11   smash
;   r12   preserve
;   r13   preserve
;   r14   preserve
;   r15   preserve
;   xmm0-5   smash
;   xmm6-16  preserve

	.code
; Fixed64 Fixed64_mul(Fixed64 a[rcx], Fixed64 b[rdx])
Fixed64_mul:
	mov	rax,rdx
	imul	rcx		; gives result in rdx:rax
	shrd	rax,rdx,56
	adc	rax,0		; round up
	ret

;Fixed64 Fixed64_mul_unsigned(Fixed64 a[rcx], Fixed64 b[rdx])
Fixed64_mul_unsigned:
	mov	rax,rdx
	mul	rcx		; result in rdx:rax
	shrd	rax,rdx,56
	adc	rax,0		; round up
	ret

; int Fixed64_iterate(Fixed64 zx[rcx], Fixed64 zy[rdx],
;                     Fixed64 cx[r8], Fixed64 cy[r9],
;                     double *r2p[rsp + 28h + pushes],
;                     int maxiters[rsp + 30h + pushes);
;
; pushes = 6 * 8 = 30h so the last 2 args are at rsp + 58h, 60h.
;
; Register allocation:
; rax,rdx	Accumulator for multiplications
; rbx		iterations
; rcx		zx
; rsi		zy
; rdi		constants
; r8 		cx
; r9 		cy
; r10,r11	zx^2
; r12,r13	zy^2
; r14		maxiters

Fixed64_iterate:
	push	rbx
        push    rsi
        push    rdi
	push	r12
	push	r13
	push	r14
        mov     rsi,rdx
        mov     eax,[rsp + 60h]
        mov     r14,rax
	mov	rbx,0
	mov	rdi,0040000000000000h
	align	10h
iterate_loop:
	; We keep z^2 in 16.112 format, since it may overflow 8.56
	mov	rax,rcx
	imul	rcx		; rdx:rax = zx^2
	mov	r11,rax
	mov	r10,rdx		; r10:r11 = zx^2
	mov	rax,rsi
	imul	rsi		; rdx:rax = zy^2
	mov	r13,rax
	mov	r12,rdx		; r12:r13 = zy^2
	add	rax,r11
	adc	rdx,r10		; rdx:rax = zx^2+zy^2
	cmp	rdx,rdi
	jge	iterate_escaped
	shrd	r11,r10,56	; r11 = zx^2
	mov	rax,rcx
	shrd	r13,r12,56	; r13 = zy^2
	imul	rsi		; rdx:rax = zx * zy
	shrd	rax,rdx,56	; rax = zx * zy
	lea	rsi,[r9+rax*2]	; zy = 2 * zx * zy + cy
	sub	r11,r13		; r11 = zx^2 - zy^2
	lea	rcx,[r11+r8]	; zx = zx^2 - zy^2 + cx
	inc	rbx
	cmp	rbx,r14
	jb	iterate_loop
	; Breached iteration limit
	jmp	iterate_done
iterate_escaped:
	; Escaped.  We can't return r^2 in an 8.56 since it may have
	; overflowed.  We can return it in a double however (and in fact
	; that's more useful to the caller).
	cvtsi2sd	xmm0,rdx
	cvtsi2sd	xmm1,rax
	; rdx had units at bit 2*56-64=48, so it needs to be
	; "shifted right" by 48 bits.
	mov	rdi,3CF0000000000000h
	movd	xmm2,rdi
	mulsd	xmm0,xmm2
	; rax is a further 64 bits down
	mov	rdi,38F0000000000000h
	movd	xmm2,rdi
	mulsd	xmm1,xmm2
	addsd	xmm0,xmm1
        mov     rsi,[rsp+58h]
	movd	qword ptr [rsi],xmm0
iterate_done:
	mov	rax,rbx		; Return iteration count
	pop	r14
	pop	r13
	pop	r12
        pop     rdi
        pop     rsi
	pop	rbx
	ret

        PUBLIC Fixed64_iterate,Fixed64_mul,Fixed64_mul_unsigned
        END
