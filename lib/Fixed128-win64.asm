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

; void Fixed128_shl_unsigned(struct Fixed128 *a=rcx)
Fixed128_shl_unsigned:
	shl	qword ptr [rcx],1
	rcl	qword ptr [rcx+8],1
	ret

; void Fixed128_shr_unsigned(struct Fixed128 *a=rcx)
Fixed128_shr_unsigned:
	shr	qword ptr [rcx+8],1
	rcr	qword ptr [rcx],1
	ret

; void Fixed128_add(struct Fixed128 *r=rcx,
;                   const struct Fixed128 *a=rdx,
;                   const struct Fixed128 *b=r8)
Fixed128_add:
	mov	rax,[rdx]
	mov	r10,[rdx+8]
	add	rax,[r8]
	mov	[rcx],rax
	adc	r10,[r8+8]
	mov	[rcx+8],r10
	ret

; void Fixed128_sub(struct Fixed128 *r=rcx,
;                   const struct Fixed128 *a=rdx,
;                   const struct Fixed128 *b=r8)
Fixed128_sub:
	mov	rax,[rdx]
	mov	r10,[rdx+8]
	sub	rax,[r8]
	mov	[rcx],rax
	sbb	r10,[r8+8]
	mov	[rcx+8],r10
	ret

; int Fixed128_neg(struct Fixed128 *r=rcx, const struct Fixed128 *a=rdx)
Fixed128_neg:
	mov	r8,[rdx]
	mov	rax,[rdx+8]
	mov	rdx,rax		; save for overflow detection
	not	r8
	add	r8,1		; we need the carry so we cannot use incq
	mov	[rcx],r8
	not	rax
	adc	rax,0
	mov	[rcx+8],rax
	xor	rax,rdx	        ; yields 1... if the top bit has changed
	not	rax		; yields 1... if the top bit has NOT changed
	shr	rax,63		; 0 for success, 1 for overflow
	ret

; int Fixed128_mul(struct Fixed128 *r=rcx,
;                  const struct Fixed128 *a=rdx,
;                  const struct Fixed128 *b=r8)
;
; We have, representing each byte with a letter:
;
;        AAAA.bbbb cccc dddd  (r11, r10)
;      * EEEE.ffff gggg hhhh  (r9, r13)
;
;   ==== ==== ==== ====                      Ab * Ef
;             ==== ==== ==== ====            Ab * gh
;             ==== ==== ==== ====            cd * Ef
;                       ==== ==== ==== ====  cd * gh
;   <- rsi -> <- rbx -> <- r12 -> <- rax ->    --> TODO
;
Fixed128_mul:
	push	rbx
        push    rsi
        push    rdi
	push	r12
        push    r13
        mov     rdi,rcx         ; save result pointer
	; Get the values into registers
	mov	r9,[r8+8]	; Ef
	mov	r11,[rdx+8]	; Ab
	mov	r13,[r8]	; gh
	mov	r10,[rdx]	; cd
	mov	cl,0		; sign of result
	; TODO could we stick the sign in some other register and save a push?
	; Make everything +ve but remember the intended sign of the result
	cmp	r9,0
	jge	mul_bpositive
	not	r13
	add	r13,1
	not	r9
	adc	r9,0
	not	cl
mul_bpositive:
	cmp	r11,0
	jge	mul_apositive
	not	r10
	add	r10,1
	not	r11
	adc	r11,0
	not	cl
mul_apositive:
	; Do the multiplies.
	; All multiplies are rax * something -> rdx:rax
	; Result will be accumulated in rsi:rbx:r12:<junk>
	mov	rax,r11		; Ab
	mul	r9		; Ab * Ef -> rdx:rax
	mov	rsi,rdx
	mov	rbx,rax
	;
	mov	rax,r11		; Ab
	mul	r13		; Ab * gh -> rdx:rax
	mov	r12,rax
	add	rbx,rdx
	adc	rsi,0
	;
	mov	rax,r10		; cd
	mul	r9		; cd * Ef -> rdx:rax
	add	r12,rax
	adc	rbx,rdx
	adc	rsi,0
	;
	mov	rax,r10		; cd
	mul	r13		; cd * fg -> rdx:rax
	add	r12,rdx
	adc	rbx,0
	adc	rsi,0
	; Check for overflow
	xor	rax,rax
	cmp	rsi,2147483647
	jbe	mul_no_overflow
	mov	rax,1
mul_no_overflow:
	; Rearrange the top 128 bits of the answer into rsi/rbx
	shld	rsi,rbx,32
	shld	rbx,r12,32
	; Maybe round up
	bt	r12,31
	adc	rbx,0
	adc	rsi,0
	; Set the right sign
	shr	cl,1
	jnc	mul_no_negate
	not	rbx
	add	rbx,1
	not	rsi
	adc	rsi,0
mul_no_negate:
	mov	[rdi+8],rsi
	mov	[rdi],rbx
        pop     r13
	pop	r12
        pop     rdi
        pop     rsi
	pop	rbx
	ret

; int Fixed128_iterate(struct Fixed128 *zx=rcx, struct Fixed128 *zy=rdx,
;                      const struct Fixed128 *cx=r8, const struct Fixed128 *cy=r9,
;                      int64_t maxiters);
;
; Outline register allocation (see code for more detail):
;  rax, rdx       - destination for mul instruction
;  rbx, rcx, rdi  - accumulator for multiplication
;  r8,r9          - zx
;  r10,r11        - zy
;  r12,r13        - zx^2
;  r14,r15        - zy^2 + input to square routine
;  rsi            - iterations
;  rbp            - sign of zx*zy
; Stack:
;  [rsp + 0]      - cx
;  [rsp + 10h]    - cy
;  [rsp + 30h]    - 8 saved registers
;  [rsp + 70h]    - return address
;  [rsp + 78h]    - home for 1st arg (rcx)
;  [rsp + 80h]    - home for three more args (rdx/r8/r9); not used
;  [rsp + 98h]    - maxiters
;
	align	10h
Fixed128_iterate:
	; Registers we must preserve
	push	rbx
	push	rbp
	push	rsi
	push	rdi
	push	r12
	push	r13
	push	r14
	push	r15
        sub     rsp,30h
	; Store maxiters on the stack
	; Store cx/cy on the stack
	mov	rax,[r8]
	mov	[rsp],rax
	mov	rax,[r8+8]
	mov	[rsp+8h],rax
	mov	rax,[r9]
	mov	[rsp+10h],rax
	mov	rax,[r9+8]
	mov	[rsp+18h],rax
	; Load initial zx/zy
	mov	r9,[rcx]
	mov	r8,[rcx+8]
	mov	r11,[rdx]
	mov	r10,[rdx+8]
        ; Stack zx for later
        mov     [rsp+78h],rcx
	; Iteration count
	xor	rsi,rsi
	; Main loop
        align   10h
iterate_loop:
	; Compute zx^2
	mov	r14,r8
	mov	r15,r9
	mov	rbp,4000000000h
	call	square
	; Compute zy^2
	mov	r14,r10
	mov	r15,r11
	mov	r12,rbx		; r12:r13 = zx^2
	mov	r13,rcx
	call	square
	mov	r14,rbx
	mov	r15,rcx
	; Add them up and compare against the limit
	add	rcx,r13
	adc	rbx,r12
	cmp	rbx,rbp
	jge	iterate_escaped
	; Compute zx*zy
	; First figure out what the sign will be
	cmp	r8,0
	setl	bpl
	jge	iterate_zx_positive
	not	r9
	add	r9,1
	not	r8
	adc	r8,0
iterate_zx_positive:
	cmp	r10,0
	jge	iterate_zy_positive
	not	r11
	add	r11,1
	not	r10
	adc	r10,0
	dec	bpl
iterate_zy_positive:
	; rbx,rcx,rdi are used as the accumulator
	mov	rax,r8
	mul	r10
	mov	rcx,rax
	mov	rbx,rdx
	;
	mov	rax,r8
	mul	r11
	mov	rdi,rax
	add	rcx,rdx
	adc	rbx,0
	;
	mov	rax,r9
	mul	r10
	add	rdi,rax
	adc	rcx,rdx
	adc	rbx,0
	;
	mov	rax,r9
	mul	r11
	add	rdi,rdx
	adc	rcx,0
	adc	rbx,0
	; Rearrange top 128 bits into two registers
	shld	rbx,rcx,32
	shld	rcx,rdi,32
	; Double and maybe round up
	bt	rdi,31
	adc	rcx,rcx
	adc	rbx,rbx
	; Set sign
	shr	bpl,1
	jnc	iterate_no_negate
	not	rcx
	add	rcx,1
	not	rbx
	adc	rbx,0
iterate_no_negate:
	; Now we have rbx,rcx = 2.zx.zy
	; Let zy=r10,r11=2.zx.zy+cy
	mov	r11,[rsp+10h]
	mov	r10,[rsp+18h]
	add	r11,rcx
	adc	r10,rbx
	; Let zy=r8,r9=zx^2-zy^2+cx
        mov     r9,[rsp]
        mov     r8,[rsp+8h]
	add	r9,r13
	adc	r8,r12
	sub	r9,r15
	sbb	r8,r14
	; Increment iteration count
	inc	rsi
	; Only go back round the loop if not too big
	cmp	rsi,[rsp+98h]
	jb	iterate_loop
iterate_escaped:
        ; Retrieve iteration count
	mov	rax,rsi
	; Return r^2 in the first argument (rather idiosyncratic)
        mov     rdi,[rsp+78h]
	mov	[rdi],rcx
	mov	[rdi+8],rbx
	; Restore registers and stack
        add     rsp,30h
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	rdi
	pop	rsi
	pop	rbp
	pop	rbx
	ret

; Let rbx,rcx=(r14,r15)^2
; rax,rdx are used by the MUL insn
; rbx,rcx,rdi are used as accumulator
;
; In the language of Fixed128_mul, we have r14=Ab=Ef, r15=cd=gh
	align	10h
square:
	; We can throw away sign information as the answer is never -ve
	cmp	r14,0
	jge	square_positive
	not	r15
	add	r15,1
	not	r14
	adc	r14,0
square_positive:
	; As with Fixed128_mul we start at the most significant end and
	; work our way down
	mov	rax,r14
	mul	r14
	mov	rcx,rax
	mov	rbx,rdx
	;
	mov	rax,r14
	mul	r15
	mov	rdi,rax
	add	rcx,rdx
	adc	rbx,0
	;
	; We take advantage of (x+y)^2=(x^2+2xy+y^2) to skip a multiply
	add	rdi,rax
	adc	rcx,rdx
	adc	rbx,0
	;
	mov	rax,r15
	mul	r15
	add	rdi,rdx
	adc	rcx,0
	adc	rbx,0
	; Rearrange top 128 bits into two registers
	shld	rbx,rcx,32
	shld	rcx,rdi,32
	; Maybe round up
	bt	rdi,31
	adc	rcx,0
	adc	rbx,0
	ret

        PUBLIC Fixed128_shl_unsigned, Fixed128_shr_unsigned
        PUBLIC Fixed128_add, Fixed128_sub, Fixed128_mul
        PUBLIC Fixed128_neg
        PUBLIC Fixed128_iterate

        END