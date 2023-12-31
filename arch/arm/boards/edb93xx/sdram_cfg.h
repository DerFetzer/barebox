// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2009 Matthias Kaehlcke <matthias@kaehlcke.net>
// SPDX-FileCopyrightText: 2006 Dominic Rath <Dominic.Rath@gmx.de>

#include <config.h>
#include <mach/ep93xx/ep93xx-regs.h>

#define SDRAM_BASE_ADDR		CONFIG_EP93XX_SDRAM_BANK0_BASE

#ifdef CONFIG_EP93XX_SDCE0_PHYS_OFFSET
#define SDRAM_DEVCFG_REG	devcfg0
#elif defined(CONFIG_EP93XX_SDCE3_SYNC_PHYS_OFFSET)
#define SDRAM_DEVCFG_REG	devcfg3
#else
#error "SDRAM bank configuration"
#endif

#if defined(CONFIG_MACH_EDB9301) || defined(CONFIG_MACH_EDB9302) ||\
	defined(CONFIG_MACH_EDB9302A)
/*
 * 1x Samsung K4S561632C-TC/L75 4M x 16bit x 4 banks SDRAM
 *
 * CLK cycle time min:
 *	@ CAS latency = 3: 7.5ns
 *	@ CAS latency = 2: 10ns
 * We're running at 66MHz (EDB9301) / 100Mhz (EDB9302(a)) external
 * bus speed (HCLK), with a cycle time of 15ns / 10ns, so it's safe
 * to use CAS latency = 2
 *
 * RAS-to-CAS delay min:
 *	20ns
 * At 15ns/10ns cycle time, we use RAS-to-CAS delay = 2
 *
 * SROMLL = 1: Swap BA[1:0] with A[13:12], making the SDRAM appear
 * as four blocks of 8MB size, instead of eight blocks of 4MB size:
 *
 * EDB9301/EDB9302:
 *
 * 0x00000000 - 0x007fffff
 * 0x01000000 - 0x017fffff
 * 0x04000000 - 0x047fffff
 * 0x05000000 - 0x057fffff
 *
 *
 * EDB9302a:
 *
 * 0xc0000000 - 0xc07fffff
 * 0xc1000000 - 0xc17fffff
 * 0xc4000000 - 0xc47fffff
 * 0xc5000000 - 0xc57fffff
 *
 * BANKCOUNT = 1: This is a device with four banks
 */

#define SDRAM_DEVCFG_VAL (SDRAM_DEVCFG_BANKCOUNT |			\
				SDRAM_DEVCFG_SROMLL |			\
				SDRAM_DEVCFG_CASLAT_2 |			\
				SDRAM_DEVCFG_RASTOCAS_2 |		\
				SDRAM_DEVCFG_EXTBUSWIDTH)

/*
 * 16 bit ext. bus
 *
 * A[22:09] is output as SYA[13:0]
 * CAS latency: 2
 * Burst type: sequential
 * Burst length: 8 (required for 16 bit ext. bus)
 * SYA[13:0] = 0x0023
 */
#define SDRAM_MODE_REG_VAL	0x4600

#define SDRAM_BANK_SEL_0	0x00000000 /* A[22:21] = b00 */
#define SDRAM_BANK_SEL_1	0x00200000 /* A[22:21] = b01 */
#define SDRAM_BANK_SEL_2	0x00400000 /* A[22:21] = b10 */
#define SDRAM_BANK_SEL_3	0x00600000 /* A[22:21] = b11 */

#elif defined(CONFIG_MACH_EDB9307) || defined(CONFIG_MACH_EDB9307A) ||\
	defined CONFIG_MACH_EDB9312 || defined(CONFIG_MACH_EDB9315) ||\
	defined(CONFIG_MACH_EDB9315A)
/*
 * 2x Samsung K4S561632C-TC/L75 4M x 16bit x 4 banks SDRAM
 *
 * CLK cycle time min:
 *	@ CAS latency = 3: 7.5ns
 *	@ CAS latency = 2: 10ns
 * We're running at 100MHz (10ns cycle time) external bus speed (HCLK),
 * so it's safe to use CAS latency = 2
 *
 * RAS-to-CAS delay min:
 *	20ns
 * At 10ns cycle time, we use RAS-to-CAS delay = 2
 *
 * EDB9307, EDB9312, EDB9315:
 *
 * 0x00000000 - 0x01ffffff
 * 0x04000000 - 0x05ffffff
 *
 *
 * EDB9307a, EDB9315a:
 *
 * 0xc0000000 - 0xc1ffffff
 * 0xc4000000 - 0xc5ffffff
 */

#define SDRAM_DEVCFG_VAL	(SDRAM_DEVCFG_BANKCOUNT |		\
				SDRAM_DEVCFG_SROMLL |			\
				SDRAM_DEVCFG_CASLAT_2 |			\
				SDRAM_DEVCFG_RASTOCAS_2)

/*
 * 32 bit ext. bus
 *
 * A[23:10] is output as SYA[13:0]
 * CAS latency: 2
 * Burst type: sequential
 * Burst length: 4
 * SYA[13:0] = 0x0022
 */
#define SDRAM_MODE_REG_VAL	0x8800

#define SDRAM_BANK_SEL_0	0x00000000 /* A[23:22] = b00 */
#define SDRAM_BANK_SEL_1	0x00400000 /* A[23:22] = b01 */
#define SDRAM_BANK_SEL_2	0x00800000 /* A[23:22] = b10 */
#define SDRAM_BANK_SEL_3	0x00c00000 /* A[23:22] = b11 */
#endif
