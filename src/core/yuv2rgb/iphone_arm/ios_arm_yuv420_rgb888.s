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
.globl _ios_arm_yuv420_rgb888
.globl ios_arm_yuv420_rgb888
.align 2
_ios_arm_yuv420_rgb888:
ios_arm_yuv420_rgb888:
ios_arm_yuv420_rgb888_cf1:
.word	0x40080100
stmfd	r13!,{r4-r11,r14}
ldr	r8, [r13,#10*4] 
ldr	r10,[r13,#11*4] 
ldr	r9, [r13,#13*4] 
ldr	r14,[r13,#14*4] 
ldr	r5, ios_arm_yuv420_rgb888_cf1
ldr	r11,[r13,#9*4] 
add	r4, r14, #256*4 
subs	r8, r8, #1 
blt	end
beq	ios_arm_yuv420_rgb888_lbl6
ios_arm_yuv420_rgb888_lbl1:
sub	r8, r8, r11,lsl #16	 
adds	r8, r8, #1<<16	 
bge	ios_arm_yuv420_rgb888_lbl11	 
ios_arm_yuv420_rgb888_lbl2:
ldrb	r11,[r2], #1		 
ldrb	r12,[r3], #1		 
ldrb	r7, [r1, r10]		 
ldrb	r6, [r1], #1		 
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2]	 
ldr	r12,[r14,r12,lsl #2]	
ldr	r7, [r14,r7, lsl #2]	
ldr	r6, [r14,r6, lsl #2]	
add	r11,r11,r12		
add	r7, r7, r11		
add	r6, r6, r11		
ands	r12,r7, r5
tsteq	r6, r5
bne	ios_arm_yuv420_rgb888_lbl13
ios_arm_yuv420_rgb888_lbl3:
add	r12,r0, r9
strb	r7,[r12],#1	
mov	r7, r7, ror #22 
strb	r7,[r12],#1	
mov	r7, r7, ror #21 
strb	r7,[r12],#1	
strb	r6,[r0], #1	
mov	r6, r6, ror #22
strb	r6,[r0], #1	
ldrb	r7, [r1, r10] 
ldrb	r12,[r1], #1 
mov	r6, r6, ror #21
ldr	r7, [r14, r7, lsl #2]	
ldr	r12,[r14, r12,lsl #2]	
strb	r6,[r0], #1		
add	r7, r7, r11		
add	r6, r12,r11		
ands	r12,r7, r5
tsteq	r6, r5
bne	ios_arm_yuv420_rgb888_lbl14
ios_arm_yuv420_rgb888_lbl4:
add	r12,r0, r9
strb	r7,[r12],#1		
mov	r7, r7, ror #22
strb	r7,[r12],#1		
mov	r7, r7, ror #21
strb	r7,[r12],#1		
strb	r6,[r0], #1		
mov	r6, r6, ror #22
strb	r6,[r0], #1		
mov	r6, r6, ror #21
strb	r6,[r0], #1	
adds	r8, r8, #2<<16
blt	ios_arm_yuv420_rgb888_lbl2
movs	r8, r8, lsl #16		
mov	r8, r8, lsr #16		
bcc	ios_arm_yuv420_rgb888_lbl11	
ios_arm_yuv420_rgb888_lbl5:
ldr	r11,[r13,#9*4]		
ldr	r12,[r13,#12*4]		
add	r0, r0, r9, lsl #1
sub	r0, r0, r11,lsl #1
sub	r0, r0, r11
add	r1, r1, r10,lsl #1
sub	r1, r1, r11
sub	r2, r2, r11,lsr #1
sub	r3, r3, r11,lsr #1
add	r2, r2, r12
add	r3, r3, r12
subs	r8, r8, #2
bgt	ios_arm_yuv420_rgb888_lbl1
ldmltfd	r13!,{r4-r11,pc}
ios_arm_yuv420_rgb888_lbl6:
sub	r8, r8, r11,lsl #16	
adds	r8, r8, #1<<16	
bge	ios_arm_yuv420_rgb888_lbl9 
ios_arm_yuv420_rgb888_lbl7:
ldrb	r11,[r2], #1	
ldrb	r12,[r3], #1	
ldrb	r6, [r1], #1	
ldrb	r7, [r1], #1	
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2] 
ldr	r12,[r14,r12,lsl #2] 
ldr	r7, [r14,r7, lsl #2] 
ldr	r6, [r14,r6, lsl #2] 
add	r11,r11,r12	
add	r6, r6, r11	
add	r7, r7, r11	
ands	r12,r7, r5
tsteq	r6, r5
bne	ios_arm_yuv420_rgb888_lbl16
ios_arm_yuv420_rgb888_lbl8:
strb	r6,[r0], #1	
mov	r6, r6, ror #22
strb	r6,[r0], #1	
mov	r6, r6, ror #21
strb	r6,[r0], #1	
strb	r7,[r0], #1	
mov	r7, r7, ror #22
strb	r7,[r0], #1	
mov	r7, r7, ror #21
strb	r7,[r0], #1	
adds	r8, r8, #2<<16
blt	ios_arm_yuv420_rgb888_lbl7
movs	r8, r8, lsl #16	
mov	r8, r8, lsr #16		
bcc	ios_arm_yuv420_rgb888_lbl9	
end:
ldmfd	r13!,{r4-r11,pc}
ios_arm_yuv420_rgb888_lbl9:
ldrb	r11,[r2], #1	
ldrb	r12,[r3], #1	
ldrb	r6, [r1], #1	
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2] 
ldr	r12,[r14,r12,lsl #2] 
ldr	r6, [r14,r6, lsl #2] 
add	r11,r11,r12	
add	r6, r6, r11	
ands	r12,r6, r5
bne	ios_arm_yuv420_rgb888_lbl17
ios_arm_yuv420_rgb888_lbl10:
strb	r6,[r0], #1	
mov	r6, r6, ror #22
strb	r6,[r0], #1	
mov	r6, r6, ror #21
strb	r6,[r0], #1	
ldmfd	r13!,{r4-r11,pc}
ios_arm_yuv420_rgb888_lbl11:
@ We have a pair of pixels left to do
ldrb	r11,[r2] 
ldrb	r12,[r3] 
ldrb	r7, [r1, r10] 
ldrb	r6, [r1], #1 
add	r12,r12,#512
ldr	r11,[r4, r11,lsl #2]
ldr	r12,[r14,r12,lsl #2]
ldr	r7, [r14,r7, lsl #2]
ldr	r6, [r14,r6, lsl #2]
add	r11,r11,r12	
add	r7, r7, r11	
add	r6, r6, r11	
ands	r12,r7, r5
tsteq	r6, r5
bne	ios_arm_yuv420_rgb888_lbl15
ios_arm_yuv420_rgb888_lbl12:
add	r12,r0, r9
strb	r7,[r12],#1	
mov	r7, r7, ror #22
strb	r7,[r12],#1	
mov	r7, r7, ror #21
strb	r7,[r12],#1	
strb	r6,[r0], #1	
mov	r6, r6, ror #22
strb	r6,[r0], #1	
mov	r6, r6, ror #21
strb	r6,[r0], #1	
b	ios_arm_yuv420_rgb888_lbl5
ios_arm_yuv420_rgb888_lbl13:
sub	r12,r12,r12,lsr #8	
orr	r7, r7, r12		
bic	r12,r5, r7, lsr #1	
add	r7, r7, r12,lsr #8	
and	r12, r6, r5		
sub	r12,r12,r12,lsr #8	
orr	r6, r6, r12		
bic	r12,r5, r6, lsr #1	
add	r6, r6, r12,lsr #8	
b	ios_arm_yuv420_rgb888_lbl3
ios_arm_yuv420_rgb888_lbl14:
sub	r12,r12,r12,lsr #8	
orr	r7, r7, r12		
bic	r12,r5, r7, lsr #1	
add	r7, r7, r12,lsr #8	
and	r12, r6, r5		
sub	r12,r12,r12,lsr #8	
orr	r6, r6, r12		
bic	r12,r5, r6, lsr #1	
add	r6, r6, r12,lsr #8	
b	ios_arm_yuv420_rgb888_lbl4
ios_arm_yuv420_rgb888_lbl15:
sub	r12,r12,r12,lsr #8	
orr	r7, r7, r12		
bic	r12,r5, r7, lsr #1	
add	r7, r7, r12,lsr #8	
and	r12, r6, r5	
sub	r12,r12,r12,lsr #8	
orr	r6, r6, r12	
bic	r12,r5, r6, lsr #1	
add	r6, r6, r12,lsr #8	
b	ios_arm_yuv420_rgb888_lbl12
ios_arm_yuv420_rgb888_lbl16:
sub	r12,r12,r12,lsr #8	
orr	r7, r7, r12		
bic	r12,r5, r7, lsr #1	
add	r7, r7, r12,lsr #8	
and	r12, r6, r5	
sub	r12,r12,r12,lsr #8	
orr	r6, r6, r12	
bic	r12,r5, r6, lsr #1	
add	r6, r6, r12,lsr #8	
b	ios_arm_yuv420_rgb888_lbl8
ios_arm_yuv420_rgb888_lbl17:
sub	r12,r12,r12,lsr #8	
orr	r6, r6, r12	
bic	r12,r5, r6, lsr #1	
add	r6, r6, r12,lsr #8	
b	ios_arm_yuv420_rgb888_lbl10


