	.syntax unified
	.cpu cortex-m4
	.thumb
.text
	.global GPIO_init
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIOA_BASE, 0x48000000
	.equ GPIO_BSRR_OFFSET, 0x18
	.equ GPIO_BRR_OFFSET, 0x28
	.equ GPIOA_MODER, 0x48000000
	.equ GPIOA_OTYPER, 0x48000004
	.equ GPIOA_OSPEEDR, 0x48000008
	.equ GPIOA_PUPDR, 0x4800000C
	.equ GPIOA_ODR, 0x48000014
	.equ DECODE_MODE,	0x09
	.equ SHUTDOWN,	0xC
	.equ INTENSITY,	0xA
	.equ SCAN_LIMIT, 0xB
	.equ DISPLAY_TEST, 0xF
GPIO_init:
	//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK
	movs 	r0, #0x1
	ldr		r1, =RCC_AHB2ENR
	str		r0,[r1]
	movs	r0, #0x5400
	ldr  	r1, =GPIOA_MODER
	ldr		r2, [r1]
	and		r2, #0xFFFF03FF
	and		r2, #0xFFFFFF00
	orrs	r2,r2,r0
	movs	r0, #0x55
	orrs	r2,r2,r0
	str		r2,[r1]
	movs	r0, #0xA800
	movs	r2, #0xAA
	orrs	r0,r0,r2
	ldr		r1, =GPIOA_OSPEEDR
	strh	r0,[r1]
	BX LR
