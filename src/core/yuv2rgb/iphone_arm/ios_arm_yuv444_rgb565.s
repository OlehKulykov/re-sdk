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
.globl _ios_arm_yuv444_rgb565
.globl ios_arm_yuv444_rgb565
.align 2
.set dith1,	7
.set dith2,	6

const_mask:
.word	0x07e0f81f
const_flags:
.word	0x40080100
ios_arm_yuv444_rgb565:
_ios_arm_yuv444_rgb565:

stmfd	r13!,{r4-r11,r14}

ldr	r11,[r13,#9*4]		@ r11= width
ldr	r8, [r13,#10*4]		@ r8 = height
ldr	r14,[r13,#14*4]		@ r14= y_table
ldr	r6, [r13,#15*4]		@ r11= dither
ldr	r4, const_mask
ldr	r5, const_flags
add	r9, r14,#256*4		@ r9 = u_table
add	r10,r14,#512*4		@ r10= v_table
ands	r6, r6, #3
beq	asm0
cmp	r6, #2
beq	asm3
bgt	asm2
asm1:
@  dither: 1 3
@          2 0
yloop1:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix10		@    just do 1 column
xloop10:
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u  = u_table[u0]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v  = v_table[v0]
add	r6, r6, r11
add	r6, r6, r12
add	r6, r6, r5, lsr #dith1	@ r6 = y0 + u0 + v0 + dither1

ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u1 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v1 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u1 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v1 = v_table[v1]
add	r7, r7, r5, lsr #dith1
add	r7, r7, r11
add	r7, r7, r12
add	r7, r7, r5, lsr #dith2	@ r7 = y0 + u0 + v0 + dither3

ands	r12,r7, r5
tsteq	r6, r5
bne	fix101
return101:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop10
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix10		@ 1 more pixel to do
trail_pix10ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
beq	end

@ now we do the second row
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix11		@    just do 1 column
xloop11:
ldrb	r7, [r1], #1		@ r6  = y2 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u2 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v2 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r6  = y2 = y_table[y2]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u2 = u_table[u2]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v2 = v_table[v2]
ldrb	r6, [r1], #1		@ r6  = y3 = *y_ptr++
add	r7, r7, r11
add	r7, r7, r12		@ r7 = y2 + u2 + v2 + dither2

ldrb	r11,[r2], #1		@ r11 = u3 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v3 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y3 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u3 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v3 = v_table[v1]
add	r7, r7, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12

ands	r12,r7, r5
tsteq	r6, r5
bne	fix102
return102:
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop11
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix11		@ 1 more pixel to do
trail_pix11ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
bne	yloop1
end:
ldmfd	r13!,{r4-r11,pc}
trail_pix10:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
add	r6, r6, r5, lsr #dith1
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv + dither1
ands	r12,r6, r5
bne	fix103
return103:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix10ret
trail_pix11:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
add	r6, r6, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv + dither2

ands	r12,r6, r5
bne	fix104
return104:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix11ret

ldmfd	r13!,{r4-r11,pc}

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
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return103
fix104:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return104

@------------------------------------------------------------------------
asm0:
@  dither: 0 2
@          3 1
yloop0:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix00		@    just do 1 column
xloop00:
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u  = u_table[u0]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v  = v_table[v0]
ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
add	r6, r6, r11
add	r6, r6, r12

ldrb	r11,[r2], #1		@ r11 = u1 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v1 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u1 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v1 = v_table[v1]
add	r7, r7, r5, lsr #dith2
add	r7, r7, r11
add	r7, r7, r12		@ r7 = y0 + u0 + v0 + dither2

ands	r12,r7, r5
tsteq	r6, r5
bne	fix001
return001:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop00
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix00		@ 1 more pixel to do
trail_pix00ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
beq	end0

@ now we do the second row
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix01		@    just do 1 column
xloop01:
ldrb	r7, [r1], #1		@ r6  = y2 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u2 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v2 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r6  = y2 = y_table[y2]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u2 = u_table[u2]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v2 = v_table[v2]
add	r7, r7, r5, lsr #dith1
add	r7, r7, r5, lsr #dith2
add	r7, r7, r11
add	r7, r7, r12		@ r7 = y2 + u2 + v2 + dither3

ldrb	r6, [r1], #1		@ r6  = y3 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u3 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v3 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y3 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u3 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v3 = v_table[v1]
add	r6, r6, r5, lsr #dith1
add	r6, r6, r11
add	r6, r6, r12		@ r6 = y3 + u3 + v3 + dither1

ands	r12,r7, r5
tsteq	r6, r5
bne	fix002
return002:
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop01
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix01		@ 1 more pixel to do
trail_pix01ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
bne	yloop0
end0:
ldmfd	r13!,{r4-r11,pc}
trail_pix00:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
@ stall on xscale
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv
ands	r12,r6, r5
bne	fix003
return003:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix00ret
trail_pix01:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
add	r6, r6, r5, lsr #dith1
add	r6, r6, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv + dither3

ands	r12,r6, r5
bne	fix004
return004:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix01ret

ldmfd	r13!,{r4-r11,pc}

fix001:
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
b	return001
fix002:
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
b	return002
fix003:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return003
fix004:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return004
@------------------------------------------------------------------------
asm2:
@  dither: 2 0
@          1 3
yloop2:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix20		@    just do 1 column
xloop20:
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u  = u_table[u0]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v  = v_table[v0]
ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
add	r6, r6, r11
add	r6, r6, r12

ldrb	r11,[r2], #1		@ r11 = u1 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v1 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u1 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v1 = v_table[v1]
add	r6, r6, r5, lsr #dith2	@ r6 = y0 + u0 + v0 + dither2
add	r7, r7, r11
add	r7, r7, r12

ands	r12,r7, r5
tsteq	r6, r5
bne	fix201
return201:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop20
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix20		@ 1 more pixel to do
trail_pix20ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
beq	end2

@ now we do the second row
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix21		@    just do 1 column
xloop21:
ldrb	r7, [r1], #1		@ r6  = y2 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u2 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v2 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r6  = y2 = y_table[y2]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u2 = u_table[u2]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v2 = v_table[v2]
add	r7, r7, r5, lsr #dith1
add	r7, r7, r11
add	r7, r7, r12		@ r7 = y2 + u2 + v2 + dither1

ldrb	r6, [r1], #1		@ r6  = y3 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u3 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v3 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y3 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u3 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v3 = v_table[v1]
add	r6, r6, r5, lsr #dith1
add	r6, r6, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12		@ r6 = y3 + u3 + v3 + dither3

ands	r12,r7, r5
tsteq	r6, r5
bne	fix202
return202:
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop21
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix21		@ 1 more pixel to do
trail_pix21ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
bne	yloop2
end2:
ldmfd	r13!,{r4-r11,pc}
trail_pix20:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
add	r6, r6, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv + dither2
ands	r12,r6, r5
bne	fix203
return203:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix20ret
trail_pix21:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
add	r6, r6, r5, lsr #dith1
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv + dither1

ands	r12,r6, r5
bne	fix204
return204:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix21ret

ldmfd	r13!,{r4-r11,pc}

fix201:
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
b	return201
fix202:
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
b	return202
fix203:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return203
fix204:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return204
@------------------------------------------------------------------------
asm3:
@  dither: 3 1
@          0 2
yloop3:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix30		@    just do 1 column
xloop30:
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u  = u_table[u0]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v  = v_table[v0]
add	r6, r6, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12
add	r6, r6, r5, lsr #dith1	@ r6 = y0 + u0 + v0 + dither3

ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u1 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v1 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u1 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v1 = v_table[v1]
add	r7, r7, r5, lsr #dith1	@ r7 = y1 + u1 + v1 + dither1
add	r7, r7, r11
add	r7, r7, r12

ands	r12,r7, r5
tsteq	r6, r5
bne	fix301
return301:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop30
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix30		@ 1 more pixel to do
trail_pix30ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
beq	end3

@ now we do the second row
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix31		@    just do 1 column
xloop31:
ldrb	r7, [r1], #1		@ r6  = y2 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u2 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v2 = *v_ptr++
ldr	r7, [r14,r7, lsl #2]	@ r6  = y2 = y_table[y2]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u2 = u_table[u2]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v2 = v_table[v2]
ldrb	r6, [r1], #1		@ r6  = y3 = *y_ptr++
add	r7, r7, r11
add	r7, r7, r12		@ r7 = y2 + u2 + v2

ldrb	r11,[r2], #1		@ r11 = u3 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v3 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y3 = y_table[y1]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u3 = u_table[u1]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v3 = v_table[v1]
add	r6, r6, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12		@ r6 = y3 + u3 + v3 + dither2

ands	r12,r7, r5
tsteq	r6, r5
bne	fix302
return302:
and	r7, r4, r7, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0], #2
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop31
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix31		@ 1 more pixel to do
trail_pix31ret:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r7, [r13,#11*4]		@ r7 = y_span
ldr	r12,[r13,#12*4]		@ r12= uv_stride
ldr	r6, [r13,#13*4]		@ r6 = dst_span
sub	r0, r0, r11,lsl #1
add	r1, r1, r7
add	r0, r0, r6
sub	r1, r1, r11
sub	r2, r2, r11
sub	r3, r3, r11
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #1
bne	yloop3
end3:
ldmfd	r13!,{r4-r11,pc}
trail_pix30:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
add	r6, r6, r5, lsr #dith1
add	r6, r6, r5, lsr #dith2
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv + dither3
ands	r12,r6, r5
bne	fix303
return303:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix30ret
trail_pix31:
@ we have a single extra pixel to do
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r11,[r2], #1		@ r11 = u0 = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v0 = *v_ptr++
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r11,[r9, r11,lsl #2]	@ r11 = u0 = u_table[u]
ldr	r12,[r10,r12,lsl #2]	@ r12 = v0 = v_table[v]
@ stall on xscale
add	r6, r6, r11
add	r6, r6, r12		@ r6  = y0 + uv

ands	r12,r6, r5
bne	fix304
return304:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
b	trail_pix31ret

ldmfd	r13!,{r4-r11,pc}

fix301:
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
b	return301
fix302:
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
b	return302
fix303:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return303
fix304:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return304

@ end
