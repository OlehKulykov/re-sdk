@
@ Realization of REYUVtoRGB methods using code provided by 2008-11 Robin Watts <theorarm@wss.co.uk>
@ You can ignore this code using config flag __RE_CORE_NO_YUV_TO_RGB_CONVERSION__
@ Content of 'COPYING', 'README' file and 'HEADER TEXT' listed below.
@
@
@
@ HEADER TEXT
@
@ YUV-> RGB conversion code.
@
@ Copyright (C) 2011 Robin Watts (robin@wss.co.uk) for Pinknoise
@ Productions Ltd.
@
@ Licensed under the BSD license. See 'COPYING' for details of
@ (non-)warranty.
@
@
@
@
@ COPYING
@
@ This YUV2RGB code is Copyright (C) 2008-11 Robin Watts
@ <theorarm@wss.co.uk>.
@
@ The software is released under the BSD license.
@
@ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
@ ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
@ LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
@ A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION
@ OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
@ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
@ LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
@ DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
@ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
@ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@
@ In particular, I warrant absolutely nothing about how patent free
@ this method is. It is your responsibility to ensure that this code
@ does not infringe any patents that apply in your area before you
@ ship it.
@
@
@
@ README
@
@ The code contained here is Copyright (C) 2008-2009 Robin Watts
@ <robin@wss.co.uk>
@
@ This code provides optimised ARM code (and the C equivalents) for fast
@ conversion of YUV420, YUV422 and YUV444 code to RGB565.
@
@ For ARM platforms, use the ARM code, as this will result in significantly
@ faster results (due to no register spillage in the loops, improved
@ scheduling etc).
@
@ For other platforms, use the C code - this is deliberately kept as close
@ as possible to the ARM code.
@
@ yuv2rgb.h is the overall header. #include this.
@
@ yuv2rgb16tab.c contains the definition of the standard table used by
@ all the routines (and passed in as a parameter). You can use this, or
@ define your own table.
@
@ Future versions of this software may include routines to generate tables
@ with given weights.
@
@ The latest version of this software should always be available from
@ <http://www.wss.co.uk/pinknoise/yuv2rgb>
@


.text
.globl _ios_arm_yuv420_rgb8888
.globl ios_arm_yuv420_rgb8888
.align 2
const_flags:
.word	0x40080100
_ios_arm_yuv420_rgb8888:
ios_arm_yuv420_rgb8888:
stmfd	r13!,{r4-r11,r14}
ldr	r8, [r13,#10*4]		@ r8 = height
ldr	r10,[r13,#11*4]		@ r10= y_span
ldr	r9, [r13,#13*4]		@ r9 = dst_span
ldr	r14,[r13,#14*4]		@ r14= y_table
ldr	r5, const_flags
ldr	r11,[r13,#9*4]		@ r11= width
add	r4, r14, #256*4
subs	r8, r8, #1
blt	end
beq	trail_row1
yloop1:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pair1		@    just do 1 column
xloop1:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r7, r7, r11		@ r7  = y2 + uv
add	r6, r6, r11		@ r6  = y0 + uv
ands	r12,r7, r5
tsteq	r6, r5
bne	fix101
return101:
@ store the bottom one first
add	r12,r0, r9
strb	r7,[r12],#1		@ store r
mov	r7, r7, ror #22
strb	r7,[r12],#1		@ store g
mov	r7, r7, ror #21
strb	r7,[r12],#1		@ store b
strb	r5,[r12]

@ then store the top one
strb	r6,[r0], #1		@ store r
mov	r6, r6, ror #22
strb	r6,[r0], #1		@ store g

ldrb	r7, [r1, r10]		@ r7 = y3 = y_ptr[stride]
ldrb	r12,[r1], #1		@ r12= y1 = *y_ptr++
mov	r6, r6, ror #21
ldr	r7, [r14, r7, lsl #2]	@ r7 = y3 = y_table[y2]
ldr	r12,[r14, r12,lsl #2]	@ r12= y1 = y_table[y0]
strb	r6,[r0], #1		@ store b
strb	r5,[r0], #1

add	r7, r7, r11		@ r7  = y3 + uv
add	r6, r12,r11		@ r6  = y1 + uv
ands	r12,r7, r5
tsteq	r6, r5
bne	fix102
return102:
@ store the bottom one first
add	r12,r0, r9
strb	r7,[r12],#1		@ store r
mov	r7, r7, ror #22
strb	r7,[r12],#1		@ store g
mov	r7, r7, ror #21
strb	r7,[r12],#1		@ store b
strb	r5,[r12]

@ then store the top one
strb	r6,[r0], #1		@ store r
mov	r6, r6, ror #22
strb	r6,[r0], #1		@ store g
mov	r6, r6, ror #21
strb	r6,[r0], #1		@ store b
strb	r5,[r0], #1

adds	r8, r8, #2<<16
blt	xloop1
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pair1		@ 1 more pixel pair to do
end_xloop1:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r12,[r13,#12*4]		@ r12= uv_stride
add	r0, r0, r9, lsl #1
sub	r0, r0, r11,lsl #2
add	r1, r1, r10,lsl #1
sub	r1, r1, r11
sub	r2, r2, r11,lsr #1
sub	r3, r3, r11,lsr #1
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #2
bgt	yloop1

ldmltfd	r13!,{r4-r11,pc}
trail_row1:
@ we have a row of pixels left to do
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix1		@    just do 1 pixel
xloop12:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r6, r6, r11		@ r6  = y0 + uv
add	r7, r7, r11		@ r7  = y1 + uv
ands	r12,r7, r5
tsteq	r6, r5
bne	fix104
return104:
@ store the bottom one first
strb	r6,[r0], #1		@ store r
mov	r6, r6, ror #22
strb	r6,[r0], #1		@ store g
mov	r6, r6, ror #21
strb	r6,[r0], #1		@ store b
strb	r5,[r0], #1

@ then store the top one
strb	r7,[r0], #1		@ store r
mov	r7, r7, ror #22
strb	r7,[r0], #1		@ store g
mov	r7, r7, ror #21
strb	r7,[r0], #1		@ store b
strb	r5,[r0], #1

adds	r8, r8, #2<<16
blt	xloop12
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix1		@ 1 more pixel pair to do
end:
ldmfd	r13!,{r4-r11,pc}
trail_pix1:
@ we have a single extra pixel to do
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r6, r6, r11		@ r6  = y0 + uv
ands	r12,r6, r5
bne	fix105
return105:
strb	r6,[r0], #1		@ store r
mov	r6, r6, ror #22
strb	r6,[r0], #1		@ store g
mov	r6, r6, ror #21
strb	r6,[r0], #1		@ store b
strb	r5,[r0], #1

ldmfd	r13!,{r4-r11,pc}

trail_pair1:
@ we have a pair of pixels left to do
ldrb	r11,[r2]		@ r11 = u  = *u_ptr++
ldrb	r12,[r3]		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r7, r7, r11		@ r7  = y2 + uv
add	r6, r6, r11		@ r6  = y0 + uv
ands	r12,r7, r5
tsteq	r6, r5
bne	fix103
return103:
@ store the bottom one first
add	r12,r0, r9
strb	r7,[r12],#1		@ store r
mov	r7, r7, ror #22
strb	r7,[r12],#1		@ store g
mov	r7, r7, ror #21
strb	r7,[r12],#1		@ store b
strb	r5,[r12]

@ then store the top one
strb	r6,[r0], #1		@ store r
mov	r6, r6, ror #22
strb	r6,[r0], #1		@ store g
mov	r6, r6, ror #21
strb	r6,[r0], #1		@ store b
strb	r5,[r0], #1
b	end_xloop1
fix101:
@ r7 and r6 are the values, at least one of which has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r7, r7, r12		@ r7 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r7, lsr #1	@ r12 = .o......o......o......
add	r7, r7, r12,lsr #8	@ r7  = fixed value

and	r12, r6, r5		@ r12 = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return101
fix102:
@ r7 and r6 are the values, at least one of which has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r7, r7, r12		@ r7 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r7, lsr #1	@ r12 = .o......o......o......
add	r7, r7, r12,lsr #8	@ r7  = fixed value

and	r12, r6, r5		@ r12 = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss..sssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss..sssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return102
fix103:
@ r7 and r6 are the values, at least one of which has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r7, r7, r12		@ r7 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r7, lsr #1	@ r12 = .o......o......o......
add	r7, r7, r12,lsr #8	@ r7  = fixed value

and	r12, r6, r5		@ r12 = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return103
fix104:
@ r7 and r6 are the values, at least one of which has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r7, r7, r12		@ r7 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r7, lsr #1	@ r12 = .o......o......o......
add	r7, r7, r12,lsr #8	@ r7  = fixed value

and	r12, r6, r5		@ r12 = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return104
fix105:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return105

@ end
