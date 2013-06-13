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
.globl _ios_arm_yuv420_rgb565
.globl ios_arm_yuv420_rgb565
.globl ios_arm_yuv420_rgb565_profile
.align 2
.set dith1,	7
.set dith2,	6
ios_arm_yuv420_rgb565_profile:
_ios_arm_yuv420_rgb565_profile:
const_mask:
.word	0x07e0f81f
const_flags:
.word	0x40080100
ios_arm_yuv420_rgb565:
_ios_arm_yuv420_rgb565:
stmfd	r13!,{r4-r11,r14}
ldr	r8, [r13,#10*4]	
ldr	r10,[r13,#11*4]	
ldr	r9, [r13,#13*4]	
ldr	r14,[r13,#14*4]	
ldr	r11,[r13,#15*4]	
ldr	r4, const_mask
ldr	r5, const_flags
ands	r11,r11,#3
beq	asm0
cmp	r11, #2
beq	asm3
bgt	asm2
asm1:
ldr	r11,[r13,#9*4]
subs	r8, r8, #1
blt	end
beq	trail_row1
yloop1:
sub	r8, r8, r11,lsl #16	
adds	r8, r8, #1<<16	
bge	trail_pair1
xloop1:
ldrb	r11,[r2], #1	
ldrb	r12,[r3], #1	
ldrb	r7, [r1, r10]	
ldrb	r6, [r1], #1	
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]
ldr	r12,[r14,r12,lsl #2]
ldr	r7, [r14,r7, lsl #2]
ldr	r6, [r14,r6, lsl #2]
add	r11,r11,r12	
add	r12,r11,r5, lsr #dith1
add	r7, r7, r12	
add	r6, r6, r12	
add	r7, r7, r5, lsr #dith2	
ands	r12,r7, r5
tsteq	r6, r5
bne	fix101
return101:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0, r9]
ldrb	r12,[r1, r10]
ldrb	r7, [r1], #1
orr	r6, r6, r6, lsr #16
ldr	r12,[r14, r12,lsl #2]
strh	r6, [r0], #2
ldr	r6, [r14, r7, lsl #2]
add	r7, r12,r11		@ r7  = y3 + uv
add	r6, r6, r11		@ r6  = y1 + uv
add	r6, r6, r5, lsr #dith2
ands	r12,r7, r5
tsteq	r6, r5
bne	fix102
return102:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop1
movs	r8, r8, lsl #16	
mov	r8, r8, lsr #16	
bcc	trail_pair1	
end_xloop1:
ldr	r11,[r13,#9*4]	
ldr	r12,[r13,#12*4]	
add	r0, r0, r9, lsl #1
add	r1, r1, r10,lsl #1
sub	r0, r0, r11,lsl #1
sub	r1, r1, r11
sub	r2, r2, r11,lsr #1
sub	r3, r3, r11,lsr #1
add	r2, r2, r12
add	r3, r3, r12
subs	r8, r8, #2
bgt	yloop1
ldmltfd	r13!,{r4-r11,pc}
trail_row1:
sub	r8, r8, r11,lsl #16	
adds	r8, r8, #1<<16	
bge	trail_pix1	
xloop12:
ldrb	r11,[r2], #1
ldrb	r12,[r3], #1
ldrb	r6, [r1], #1	
ldrb	r7, [r1], #1	
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]
ldr	r12,[r14,r12,lsl #2]
ldr	r7, [r14,r7, lsl #2]
ldr	r6, [r14,r6, lsl #2]
add	r11,r11,r12	
add	r6, r6, r11	
add	r7, r7, r11	
add	r6, r6, r5, lsr #dith1
add	r7, r7, r5, lsr #dith2
ands	r12,r7, r5
tsteq	r6, r5
bne	fix104
return104:
and	r6, r4, r6, lsr #3
and	r7, r4, r7, lsr #3
orr	r6, r6, r6, lsr #16
orr	r7, r7, r7, lsr #16
strh	r6, [r0], #2
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop12
movs	r8, r8, lsl #16	
mov	r8, r8, lsr #16	
bcc	trail_pix1	
end:
ldmfd	r13!,{r4-r11,pc}
trail_pix1:
ldrb	r11,[r2], #1	
ldrb	r12,[r3], #1	
ldrb	r6, [r1], #1	
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]
ldr	r12,[r14,r12,lsl #2]
ldr	r6, [r14,r6, lsl #2]
add	r11,r11,r12	
add	r11,r11,r5, lsr #dith1	
add	r6, r6, r11	
ands	r12,r6, r5
bne	fix105
return105:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2
ldmfd	r13!,{r4-r11,pc}
trail_pair1:
ldrb	r11,[r2]	
ldrb	r12,[r3]	
ldrb	r7, [r1, r10]	
ldrb	r6, [r1], #1	
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]
ldr	r12,[r14,r12,lsl #2]
ldr	r7, [r14,r7, lsl #2]
ldr	r6, [r14,r6, lsl #2]
add	r11,r11,r12	
add	r11,r11,r5, lsr #dith1
add	r7, r7, r11	
add	r6, r6, r11		@ r6  = y0 + uv
add	r7, r7, r5, lsr #dith2	@ r7  = y2 + uv + dither3
ands	r12,r7, r5
tsteq	r6, r5
bne	fix103
return103:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
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

@------------------------------------------------------------------------
asm0:
@  dither: 0 3
@          2 1
ldr	r11,[r13,#9*4]		@ r11= width
subs	r8, r8, #1
blt	end
beq	trail_row0
yloop0:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pair0		@    just do 1 column
xloop0:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r7, r7, r11		@ r7  = y2 + uv
add	r6, r6, r11		@ r6  = y0 + uv
add	r7, r7, r5, lsr #dith2	@ r7  = y2 + uv + dither2
ands	r12,r7, r5
tsteq	r6, r5
bne	fix001
return001:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0, r9]
ldrb	r12,[r1, r10]		@ r12 = y3 = y_ptr[stride]
ldrb	r7, [r1], #1		@ r6  = y1 = *y_ptr++
orr	r6, r6, r6, lsr #16
ldr	r12,[r14, r12,lsl #2]	@ r7  = y3 = y_table[y2]
strh	r6, [r0], #2
ldr	r6, [r14, r7, lsl #2]	@ r6  = y1 = y_table[y0]

add	r11,r11,r5, lsr #dith1
add	r7, r12,r11		@ r7  = y3 + uv + dither1
add	r6, r6, r11		@ r6  = y1 + uv + dither1
add	r6, r6, r5, lsr #dith2	@ r6  = y1 + uv + dither3
ands	r12,r7, r5
tsteq	r6, r5
bne	fix002
return002:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop0
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pair0		@ 1 more pixel pair to do
end_xloop0:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r12,[r13,#12*4]		@ r12= uv_stride
add	r0, r0, r9, lsl #1
sub	r0, r0, r11,lsl #1
add	r1, r1, r10,lsl #1
sub	r1, r1, r11
sub	r2, r2, r11,lsr #1
sub	r3, r3, r11,lsr #1
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #2
bgt	yloop0

ldmltfd	r13!,{r4-r11,pc}
trail_row0:
@ we have a row of pixels left to do
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix0		@    just do 1 pixel
xloop02:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r6, r6, r11		@ r6  = y0 + uv
add	r7, r7, r11		@ r7  = y1 + uv
add	r7, r7, r5, lsr #dith1	@ r7  = y1 + uv + dither1
add	r7, r7, r5, lsr #dith2	@ r7  = y1 + uv + dither3
ands	r12,r7, r5
tsteq	r6, r5
bne	fix004
return004:
and	r6, r4, r6, lsr #3
and	r7, r4, r7, lsr #3
orr	r6, r6, r6, lsr #16
orr	r7, r7, r7, lsr #16
strh	r6, [r0], #2
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop02
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix0		@ 1 more pixel pair to do

ldmfd	r13!,{r4-r11,pc}
trail_pix0:
@ we have a single extra pixel to do
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
@ stall (on xscale)
add	r11,r11,r12		@ r11 = uv = u+v

add	r6, r6, r11		@ r6  = y0 + uv
ands	r12,r6, r5
bne	fix005
return005:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2

ldmfd	r13!,{r4-r11,pc}

trail_pair0:
@ we have a pair of pixels left to do
ldrb	r11,[r2]		@ r11 = u  = *u_ptr++
ldrb	r12,[r3]		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r7, r7, r11		@ r7  = y2 + uv
add	r6, r6, r11		@ r6  = y0 + uv
add	r7, r7, r5, lsr #dith2	@ r7  = y2 + uv + dither2
ands	r12,r7, r5
tsteq	r6, r5
bne	fix003
return003:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
b	end_xloop0
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
b	return003
fix004:
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
b	return004
fix005:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return005

@------------------------------------------------------------------------
asm2:
@  dither: 2 1
@          0 3
ldr	r11,[r13,#9*4]		@ r11= width
subs	r8, r8, #1
blt	end
beq	trail_row2
yloop2:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pair2		@    just do 1 column
xloop2:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r7, r7, r11		@ r7  = y2 + uv
add	r6, r6, r11		@ r6  = y0 + uv
add	r6, r6, r5, lsr #dith2	@ r6  = y0 + uv + dither2
ands	r12,r7, r5
tsteq	r6, r5
bne	fix201
return201:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0, r9]
ldrb	r12,[r1, r10]		@ r12 = y3 = y_ptr[stride]
ldrb	r7, [r1], #1		@ r6  = y1 = *y_ptr++
orr	r6, r6, r6, lsr #16
ldr	r12,[r14, r12,lsl #2]	@ r7  = y3 = y_table[y2]
strh	r6, [r0], #2
ldr	r6, [r14, r7, lsl #2]	@ r6  = y1 = y_table[y0]

add	r11,r11,r5, lsr #dith1
add	r7, r12,r11		@ r7  = y3 + uv + dither1
add	r7, r7, r5, lsr #dith2	@ r7  = y3 + uv + dither3
add	r6, r6, r11		@ r6  = y1 + uv + dither1
ands	r12,r7, r5
tsteq	r6, r5
bne	fix202
return202:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop2
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pair2		@ 1 more pixel pair to do
end_xloop2:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r12,[r13,#12*4]		@ r12= uv_stride
add	r0, r0, r9, lsl #1
add	r1, r1, r10,lsl #1
sub	r0, r0, r11,lsl #1
sub	r1, r1, r11
sub	r2, r2, r11,lsr #1
sub	r3, r3, r11,lsr #1
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #2
bgt	yloop2

ldmltfd	r13!,{r4-r11,pc}
trail_row2:
@ we have a row of pixels left to do
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix2		@    just do 1 pixel
xloop22:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
add	r11,r11,r12		@ r11 = uv = u+v

add	r6, r6, r11		@ r6  = y0 + uv
add	r6, r6, r5, lsr #dith2	@ r6  = y0 + uv + dither2
add	r7, r7, r11		@ r7  = y1 + uv
add	r7, r7, r5, lsr #dith1	@ r7  = y1 + uv + dither1
ands	r12,r7, r5
tsteq	r6, r5
bne	fix204
return204:
and	r6, r4, r6, lsr #3
and	r7, r4, r7, lsr #3
orr	r6, r6, r6, lsr #16
orr	r7, r7, r7, lsr #16
strh	r6, [r0], #2
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop22
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix2		@ 1 more pixel pair to do

ldmfd	r13!,{r4-r11,pc}
trail_pix2:
@ we have a single extra pixel to do
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r11,r11,r5, lsr #dith2
add	r6, r6, r11		@ r6  = y0 + uv + dither2
ands	r12,r6, r5
bne	fix205
return205:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2

ldmfd	r13!,{r4-r11,pc}

trail_pair2:
@ we have a pair of pixels left to do
ldrb	r11,[r2]		@ r11 = u  = *u_ptr++
ldrb	r12,[r3]		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r7, r7, r11		@ r7  = y2 + uv
add	r6, r6, r11		@ r6  = y0 + uv
add	r6, r6, r5, lsr #dith2	@ r6  = y0 + uv + 2
ands	r12,r7, r5
tsteq	r6, r5
bne	fix203
return203:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
b	end_xloop2
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
b	return203
fix204:
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
b	return204
fix205:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return205

@------------------------------------------------------------------------
asm3:
@  dither: 3 0
@          1 2
ldr	r11,[r13,#9*4]		@ r11= width
subs	r8, r8, #1
blt	end
beq	trail_row3
yloop3:
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pair3		@    just do 1 column
xloop3:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r12,r11,r5, lsr #dith1
add	r7, r7, r12		@ r7  = y2 + uv + dither1
add	r6, r6, r12		@ r6  = y0 + uv + dither1
add	r6, r6, r5, lsr #dith2	@ r6  = y0 + uv + dither3
ands	r12,r7, r5
tsteq	r6, r5
bne	fix301
return301:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
strh	r7, [r0, r9]
ldrb	r12,[r1, r10]		@ r12 = y3 = y_ptr[stride]
ldrb	r7, [r1], #1		@ r6  = y1 = *y_ptr++
orr	r6, r6, r6, lsr #16
ldr	r12,[r14, r12,lsl #2]	@ r7  = y3 = y_table[y2]
strh	r6, [r0], #2
ldr	r6, [r14, r7, lsl #2]	@ r6  = y1 = y_table[y0]

add	r7, r12,r11		@ r7  = y3 + uv
add	r7, r7, r5, lsr #dith2	@ r7  = y3 + uv + dither2
add	r6, r6, r11		@ r6  = y1 + uv
ands	r12,r7, r5
tsteq	r6, r5
bne	fix302
return302:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
adds	r8, r8, #2<<16
blt	xloop3
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pair3		@ 1 more pixel pair to do
end_xloop3:
ldr	r11,[r13,#9*4]		@ r11= width
ldr	r12,[r13,#12*4]		@ r12= uv_stride
add	r0, r0, r9, lsl #1
sub	r0, r0, r11,lsl #1
add	r1, r1, r10,lsl #1
sub	r1, r1, r11
sub	r2, r2, r11,lsr #1
sub	r3, r3, r11,lsr #1
add	r2, r2, r12
add	r3, r3, r12

subs	r8, r8, #2
bgt	yloop3

ldmltfd	r13!,{r4-r11,pc}
trail_row3:
@ we have a row of pixels left to do
sub	r8, r8, r11,lsl #16	@ r8 = height-(width<<16)
adds	r8, r8, #1<<16		@ if (width == 1)
bge	trail_pix3		@    just do 1 pixel
xloop32:
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
ldrb	r7, [r1], #1		@ r7  = y1 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y1 = y_table[y1]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r6, r6, r11		@ r6  = y0 + uv
add	r6, r6, r5, lsr #dith1	@ r6  = y0 + uv + dither1
add	r7, r7, r11		@ r7  = y1 + uv
add	r6, r6, r5, lsr #dith2	@ r6  = y0 + uv + dither3
ands	r12,r7, r5
tsteq	r6, r5
bne	fix304
return304:
and	r6, r4, r6, lsr #3
and	r7, r4, r7, lsr #3
orr	r6, r6, r6, lsr #16
orr	r7, r7, r7, lsr #16
strh	r6, [r0], #2
strh	r7, [r0], #2
adds	r8, r8, #2<<16
blt	xloop32
movs	r8, r8, lsl #16		@ clear the top 16 bits of r8
mov	r8, r8, lsr #16		@ if the c bit is clear we still have
bcc	trail_pix3		@ 1 more pixel pair to do

ldmfd	r13!,{r4-r11,pc}
trail_pix3:
@ we have a single extra pixel to do
ldrb	r11,[r2], #1		@ r11 = u  = *u_ptr++
ldrb	r12,[r3], #1		@ r12 = v  = *v_ptr++
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r11,r11,r5, lsr #dith1
add	r11,r11,r5, lsr #dith2
add	r6, r6, r11		@ r6  = y0 + uv + dither3
ands	r12,r6, r5
bne	fix305
return305:
and	r6, r4, r6, lsr #3
orr	r6, r6, r6, lsr #16
strh	r6, [r0], #2

ldmfd	r13!,{r4-r11,pc}

trail_pair3:
@ we have a pair of pixels left to do
ldrb	r11,[r2]		@ r11 = u  = *u_ptr++
ldrb	r12,[r3]		@ r12 = v  = *v_ptr++
ldrb	r7, [r1, r10]		@ r7  = y2 = y_ptr[stride]
ldrb	r6, [r1], #1		@ r6  = y0 = *y_ptr++
add	r11,r11,#256
add	r12,r12,#512
ldr	r11,[r14,r11,lsl #2]	@ r11 = u  = u_table[u]
ldr	r12,[r14,r12,lsl #2]	@ r12 = v  = v_table[v]
ldr	r7, [r14,r7, lsl #2]	@ r7  = y2 = y_table[y2]
ldr	r6, [r14,r6, lsl #2]	@ r6  = y0 = y_table[y0]
add	r11,r11,r12		@ r11 = uv = u+v

add	r12,r11,r5, lsr #dith1
add	r7, r7, r12		@ r7  = y2 + uv + dither1
add	r6, r6, r12		@ r6  = y0 + uv + dither1
add	r6, r6, r5, lsr #dith2	@ r6  = y0 + uv + dither3
ands	r12,r7, r5
tsteq	r6, r5
bne	fix303
return303:
and	r7, r4, r7, lsr #3
and	r6, r4, r6, lsr #3
orr	r7, r7, r7, lsr #16
orr	r6, r6, r6, lsr #16
strh	r7, [r0, r9]
strh	r6, [r0], #2
b	end_xloop3
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
b	return303
fix304:
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
b	return304
fix305:
@ r6 is the value, which has has overflowed
@ r12 = r7 & mask = .s......s......s......
sub	r12,r12,r12,lsr #8	@ r12 = ..ssssss.ssssss.ssssss
orr	r6, r6, r12		@ r6 |= ..ssssss.ssssss.ssssss
bic	r12,r5, r6, lsr #1	@ r12 = .o......o......o......
add	r6, r6, r12,lsr #8	@ r6  = fixed value
b	return305

@ end
