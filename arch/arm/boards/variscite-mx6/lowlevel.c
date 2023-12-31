// SPDX-License-Identifier: GPL-2.0-or-later

/*
 * Copyright (C) 2013 Michael Burkey
 * Based on code by Sascha Hauer <s.hauer@pengutronix.de>
 */

#include <debug_ll.h>
#include <mach/imx/debug_ll.h>
#include <common.h>
#include <linux/sizes.h>
#include <io.h>
#include <asm/barebox-arm-head.h>
#include <asm/barebox-arm.h>
#include <asm/sections.h>
#include <asm/cache.h>
#include <asm/mmu.h>
#include <mach/imx/imx6.h>

static inline void setup_uart(void)
{
	void __iomem *iomuxbase = (void *)MX6_IOMUXC_BASE_ADDR;

	writel(0x03, iomuxbase + 0x0280);
	writel(0x03, iomuxbase + 0x0284);

	imx6_ungate_all_peripherals();
	imx6_uart_setup_ll();

	putc_ll('>');
}
extern char __dtb_imx6q_var_custom_start[];

ENTRY_FUNCTION(start_variscite_custom, r0, r1, r2)
{
	void *fdt;

	imx6_cpu_lowlevel_init();

	arm_setup_stack(0x00920000);

	if (IS_ENABLED(CONFIG_DEBUG_LL))
	    setup_uart();

	fdt = __dtb_imx6q_var_custom_start + get_runtime_offset();

	barebox_arm_entry(0x10000000, SZ_1G, fdt);
}
