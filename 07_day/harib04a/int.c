/* ���荞�݊֌W */

#include "bootpack.h"
#include <stdio.h>

void init_pic(void)
/* PIC�̏����� */
{
	io_out8(PIC0_IMR,  0xff  ); /* �S�Ă̊��荞�݂��󂯕t���Ȃ� */
	io_out8(PIC1_IMR,  0xff  ); /* �S�Ă̊��荞�݂��󂯕t���Ȃ� */

	io_out8(PIC0_ICW1, 0x11  ); /* �G�b�W�g���K���[�h */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7�́AINT20-27�Ŏ󂯂� */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1��IRQ2�ɂĐڑ� */
	io_out8(PIC0_ICW4, 0x01  ); /* �m���o�b�t�@���[�h */

	io_out8(PIC1_ICW1, 0x11  ); /* �G�b�W�g���K���[�h */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15�́AINT28-2f�Ŏ󂯂� */
	io_out8(PIC1_ICW3, 2     ); /* PIC1��IRQ2�ɂĐڑ� */
	io_out8(PIC1_ICW4, 0x01  ); /* �m���o�b�t�@���[�h */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1�ȊO�͑S�ċ֎~ */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 �S�Ă̊��荞�݂��󂯕t���Ȃ� */

	return;
}

#define PORT_KEYDAT		0x0060

void inthandler21(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	unsigned char data, s[4];
	io_out8(PIC0_OCW2, 0x61);	/* IRQ-01��t������PIC�ɒʒm */
	data = io_in8(PORT_KEYDAT);

	sprintf(s, "%02X", data);
	boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);

	return;
}

void inthandler2c(int *esp)
/* PS/2�}�E�X����̊��荞�� */
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
	for (;;) {
		io_hlt();
	}
}