
/*
 * SPDIFEncodeSubFrame.s
 *
 * Created: 2020/12/16 0:12:35
 *  Author: ain
 */ 

.section	.text

; SPDIFEncodeSubFrame(uint8_t preamble, int audio, SPDIFSubFrame& subFrame)
.global	SPDIFEncodeSubFrame
.func	SPDIFEncodeSubFrame
 
 SPDIFEncodeSubFrame:
	movw	r26, r20
	st		X+, r24			; preamble
	ldi		r20, 0b11001100
	st		X+, r20			; AUX (0000)
	st		X+, r20			; audio data 00-03 (0000)

	// audio data 04-07
	mov		r30, r22
	andi	r30, 0x0F
	clr		r31
	subi	r30, lo8(-(nibbleCodeTable))
	sbci	r31, hi8(-(nibbleCodeTable))
	lpm		r21, Z
	st		X+, r21
	andi	r21, 0x01
	neg		r21

	// audio data 08-0b
	mov		r30, r22
	swap	r30
	andi	r30, 0x0F
	clr		r31
	subi	r30, lo8(-(nibbleCodeTable))
	sbci	r31, hi8(-(nibbleCodeTable))
	lpm		r20, Z
	eor		r21, r20
	st		X+, r21
	andi	r21, 0x01
	neg		r21

	// audio data 0c-0f
	mov		r30, r23
	andi	r30, 0x0F
	clr		r31
	subi	r30, lo8(-(nibbleCodeTable))
	sbci	r31, hi8(-(nibbleCodeTable))
	lpm		r20, Z
	eor		r21, r20
	st		X+, r21
	andi	r21, 0x01
	neg		r21

	// audio data 10-13
	mov		r30, r23
	swap	r30
	andi	r30, 0x0F
	clr		r31
	subi	r30, lo8(-(nibbleCodeTable))
	sbci	r31, hi8(-(nibbleCodeTable))
	lpm		r20, Z
	eor		r21, r20
	st		X+, r21

	// Validity, User, Status, Parity
	ldi		r20, 0b00110010
	sbrs	r21, 0
	ldi		r20, 0b11001100 
	st		X, r20
	ret
.endfunc

nibbleCodeTable:
	.byte	0xcc, 0xb3, 0xd3, 0xac, 0xcb, 0xb4, 0xd4, 0xab,	0xcd, 0xb2, 0xd2, 0xad, 0xca, 0xb5, 0xd5, 0xaa
