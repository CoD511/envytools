/*
 * Copyright (C) 2011 Marcin Kościelnicki <koriakin@0x04.net>
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef BIOS_H
#define BIOS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#define ENVY_BIOS_ERR(fmt, arg...) fprintf(stderr, fmt, ##arg)
#define ENVY_BIOS_WARN(fmt, arg...) fprintf(stderr, fmt, ##arg)

struct envy_bios_part {
	unsigned int start;
	unsigned int length;
	unsigned int pcir_offset;
	uint16_t pcir_vendor;
	uint16_t pcir_device;
	uint16_t pcir_vpd;
	uint16_t pcir_len;
	uint8_t pcir_rev;
	uint8_t pcir_class[3];
	uint16_t pcir_code_rev;
	uint8_t pcir_code_type;
	uint8_t pcir_indi;
	unsigned int init_length;
	int chksum_pass;
};

enum envy_bios_type {
	ENVY_BIOS_TYPE_UNKNOWN = 0,
	ENVY_BIOS_TYPE_NV01,
	ENVY_BIOS_TYPE_NV03,
	ENVY_BIOS_TYPE_NV04,
};

enum envy_bios_gpio_tag {
	ENVY_BIOS_GPIO_PANEL_BACKLIGHT_ON	= 0x00,
	ENVY_BIOS_GPIO_PANEL_POWER		= 0x01,

	ENVY_BIOS_GPIO_VID_0			= 0x04,
	ENVY_BIOS_GPIO_VID_1			= 0x05,
	ENVY_BIOS_GPIO_VID_2			= 0x06,
	ENVY_BIOS_GPIO_HPD_0			= 0x07,
	ENVY_BIOS_GPIO_HPD_1			= 0x08,
	ENVY_BIOS_GPIO_FAN_CONTROL		= 0x09,

	ENVY_BIOS_GPIO_TVDAC_0			= 0x0c,
	/* 0x0d seen, input [NV40] */

	/* 0x0f seen, output [NV50], SPEC NVIO, uses unk40_0 and unk41_4 */

	/* 0x11 seen, neg input [NV40, NV50, NVA0], SPEC NVIO [or not] */
	/* 0x12 seen, input [NV40, NV50] */

	/* 0x18 seen, output [NVA5, NVC0] */

	ENVY_BIOS_GPIO_VID_3			= 0x1a,

	ENVY_BIOS_GPIO_PANEL_BACKLIGHT_LEVEL	= 0x21,

	/* 0x23 seen, output [NV84, NV86 laptop, NVA5], SPEC NVIO... and as input on NVD9 with unk41_line used */

	/* 0x29 seen, input [NV50] */

	ENVY_BIOS_GPIO_TVDAC_1			= 0x2d,
	/* 0x2e seen, output neg [NV86 laptop, NVC0], related to mem reclocking */

	/* 0x30 seen, output or neg input [NVA0, NVC0], *twice* */

	ENVY_BIOS_GPIO_FAN_SENSE		= 0x3d,

	ENVY_BIOS_GPIO_SLI_SENSE_0		= 0x40, /* XXX: uses unk40_0, unk41_4, unk41_line */
	ENVY_BIOS_GPIO_SLI_SENSE_1		= 0x41, /* XXX: uses unk40_0, unk41_4, unk41_line */
	/* 0x42 seen, input [NV50, NV92, NVA0, NVC0], SPEC NVIO -- uses unk40_0, unk40_2 */
	/* 0x43 seen, output [NV50, NVA0], SPEC NVIO [or not] */
	ENVY_BIOS_GPIO_SLI_SENSE_1_ALT		= 0x44, /* used on NV50 instead of 0x41 for some reason */

	/* 0x49 seen, input [NVD9], unk41_line used */
	/* 0x4a related to DP */
	/* 0x4b related to DP */
	ENVY_BIOS_GPIO_ATX_POWER_BAD		= 0x4c,

	ENVY_BIOS_GPIO_HPD_2			= 0x51,
	ENVY_BIOS_GPIO_HPD_3			= 0x52,

	ENVY_BIOS_GPIO_HPD_4			= 0x5e,
	ENVY_BIOS_GPIO_HPD_5			= 0x5f,
	ENVY_BIOS_GPIO_HPD_6			= 0x60,

	ENVY_BIOS_GPIO_VID_4			= 0x73,

	ENVY_BIOS_GPIO_UNUSED			= 0xff,
};

struct envy_bios_gpio_entry {
	uint8_t tag;
	uint8_t line;
	uint8_t log[2];
	uint8_t param;
	uint8_t unk40_0;
	uint8_t unk40_2;
	uint8_t def;
	uint8_t mode;
	uint8_t spec41;
	uint8_t unk41_3_0;
	uint8_t unk41_3_1;
	uint8_t unk41_4; /* same thing as unk40_0 probably... */
};

struct envy_bios_subgunk {
	uint16_t offset;
	uint8_t valid;
	uint8_t version;
	uint8_t hlen;
	uint8_t entriesnum;
	uint8_t rlen;
};

struct envy_bios_gunk {
	uint16_t offset;
	uint8_t valid;
	uint8_t version;
	uint8_t hlen;
	uint8_t entriesnum;
	uint8_t rlen;
	struct envy_bios_subgunk *entries;
};

struct envy_bios_gpio {
	uint16_t offset;
	uint8_t valid;
	uint8_t version;
	uint8_t hlen;
	uint8_t entriesnum;
	uint8_t rlen;
	struct envy_bios_gpio_entry *entries;
	struct envy_bios_gunk gunk;
};

struct envy_bios {
	uint8_t *data;
	unsigned int length;
	unsigned int origlength;

	struct envy_bios_part *parts;
	int partsnum;
	int broken_part;

	enum envy_bios_type type;

	unsigned int bmp_offset;
	unsigned int bmp_length;
	uint8_t bmp_ver_major;
	uint8_t bmp_ver_minor;
	uint16_t bmp_ver;
	uint16_t mode_x86;
	uint16_t init_x86;
	uint16_t init_script;

	unsigned int bit_offset;

	unsigned int hwsq_offset;

	uint16_t dcb_offset;

	struct envy_bios_gpio gpio;
};

static inline int bios_u8(struct envy_bios *bios, unsigned int offs, uint8_t *res) {
	if (offs >= bios->length) {
		*res = 0;
		ENVY_BIOS_ERR("requested OOB u8 at 0x%04x\n", offs);
		return -EFAULT;
	}
	*res = bios->data[offs];
	return 0;
}

static inline int bios_u16(struct envy_bios *bios, unsigned int offs, uint16_t *res) {
	if (offs+1 >= bios->length) {
		*res = 0;
		ENVY_BIOS_ERR("requested OOB u16 at 0x%04x\n", offs);
		return -EFAULT;
	}
	*res = bios->data[offs] | bios->data[offs+1] << 8;
	return 0;
}

static inline int bios_u32(struct envy_bios *bios, unsigned int offs, uint32_t *res) {
	if (offs+3 >= bios->length) {
		*res = 0;
		ENVY_BIOS_ERR("requested OOB u32 at 0x%04x\n", offs);
		return -EFAULT;
	}
	*res = bios->data[offs] | bios->data[offs+1] << 8 | bios->data[offs+2] << 16 | bios->data[offs+3] << 24;
	return 0;
}

#define ENVY_BIOS_PRINT_PCIR	0x00000001
#define ENVY_BIOS_PRINT_BMP_BIT	0x00000002
#define ENVY_BIOS_PRINT_SCRIPTS	0x00000004
#define ENVY_BIOS_PRINT_GPIO	0x00000008
#define ENVY_BIOS_PRINT_ALL	0xffffffff

int envy_bios_parse (struct envy_bios *bios);
void envy_bios_dump_hex (struct envy_bios *bios, FILE *out, unsigned int start, unsigned int length);
void envy_bios_print (struct envy_bios *bios, FILE *out, unsigned mask);

int envy_bios_parse_gpio (struct envy_bios *bios);
void envy_bios_print_gpio (struct envy_bios *bios, FILE *out);

struct enum_val {
	int val;
	const char *str;
};

const char *find_enum(struct enum_val *evals, int val);

#endif
