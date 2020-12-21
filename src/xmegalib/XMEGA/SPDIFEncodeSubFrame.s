
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

	// preamble
	st		X+, r24

	ldi		r20, 0b11001100					; 0000 bi-phase mark
	ldi		r24, 0x0F						; nibble mask
	ldi		r25, 0x01						; last bit mask
	ldi		r18, lo8(-(nibbleCodeTable))	; code table address (lo8)
	ldi		r19, hi8(-(nibbleCodeTable))	; code table address (hi8)

	// AUX (0000)
	st		X+, r20

	// audio data 00-03
	st		X+, r20

	// audio data 04-07
	mov		r30, r22
	and		r30, r24
	lsl		r30
	clr		r31
	sub		r30, r18
	sbc		r31, r19
	lpm		r31, Z
	st		X+, r31

	// audio data 08-0b
	mov		r30, r22
	swap	r30
	and		r30, r24
	and		r31, r25
	lsr		r31
	rol		r30
	sub		r30, r18
	sbc		r31, r19
	lpm		r31, Z
	st		X+, r31

	// audio data 0c-0f
	mov		r30, r23
	and		r30, r24
	and		r31, r25
	lsr		r31
	rol		r30
	sub		r30, r18
	sbc		r31, r19
	lpm		r31, Z
	st		X+, r31

	// audio data 10-13
	mov		r30, r23
	swap	r30
	and		r30, r24
	and		r31, r25
	lsr		r31
	rol		r30
	sub		r30, r18
	sbc		r31, r19
	lpm		r31, Z
	st		X+, r31

	// Validity, User, Status, Parity
	sbrc	r31, 0
	ldi		r20, 0b00110010
	st		X, r20
	ret
.endfunc

nibbleCodeTable:
	.byte	0b11001100, 0b00110011		// 0000
	.byte	0b10110011, 0b01001100		// 1000
	.byte	0b11010011, 0b00101100		// 0100
	.byte	0b10101100, 0b01010011		// 1100
	.byte	0b11001011, 0b00110100		// 0010
	.byte	0b10110100, 0b01001011		// 1010
	.byte	0b11010100, 0b00101011		// 0110
	.byte	0b10101011, 0b01010100		// 1110
	.byte	0b11001101, 0b00110010		// 0001
	.byte	0b10110010, 0b01001101		// 1001
	.byte	0b11010010, 0b00101101		// 0101
	.byte	0b10101101, 0b01010010		// 1101
	.byte	0b11001010, 0b00110101		// 0011
	.byte	0b10110101, 0b01001010		// 1011
	.byte	0b11010101, 0b00101010		// 0111
	.byte	0b10101010, 0b01010101		// 1111
