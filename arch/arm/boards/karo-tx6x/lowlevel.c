// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2014 Steffen Trumtrar, Pengutronix

#include <debug_ll.h>
#include <mach/imx/debug_ll.h>
#include <common.h>
#include <asm/barebox-arm-head.h>
#include <asm/barebox-arm.h>
#include <image-metadata.h>
#include <mach/imx/generic.h>
#include <mach/imx/esdctl.h>
#include <linux/sizes.h>

static inline void setup_uart(void)
{
	void __iomem *iomuxbase = (void *)MX6_IOMUXC_BASE_ADDR;

	writel(0x1, iomuxbase + 0x0314);
	writel(0x1, iomuxbase + 0x0318);
	writel(0x1, iomuxbase + 0x0330);
	writel(0x1, iomuxbase + 0x032c);

	imx6_ungate_all_peripherals();
	imx6_uart_setup_ll();

	putc_ll('>');
}

extern char __dtb_imx6dl_tx6u_start[];

BAREBOX_IMD_TAG_STRING(tx6x_mx6_memsize_512M, IMD_TYPE_PARAMETER, "memsize=512", 0);

ENTRY_FUNCTION(start_imx6dl_tx6x_512m, r0, r1, r2)
{
	void *fdt;

	imx6_cpu_lowlevel_init();

	arm_setup_stack(0x00920000);

	IMD_USED(tx6x_mx6_memsize_512M);

	if (IS_ENABLED(CONFIG_DEBUG_LL))
		setup_uart();

	fdt = __dtb_imx6dl_tx6u_start + get_runtime_offset();

	barebox_arm_entry(0x10000000, SZ_512M, fdt);
}

BAREBOX_IMD_TAG_STRING(tx6x_mx6_memsize_1G, IMD_TYPE_PARAMETER, "memsize=1024", 0);

ENTRY_FUNCTION(start_imx6dl_tx6x_1g, r0, r1, r2)
{
	void *fdt;

	imx6_cpu_lowlevel_init();

	arm_setup_stack(0x00920000);

	IMD_USED(tx6x_mx6_memsize_1G);

	if (IS_ENABLED(CONFIG_DEBUG_LL))
		setup_uart();

	fdt = __dtb_imx6dl_tx6u_start + get_runtime_offset();

	barebox_arm_entry(0x10000000, SZ_1G, fdt);
}

extern char __dtb_imx6q_tx6q_start[];

ENTRY_FUNCTION(start_imx6q_tx6x_1g, r0, r1, r2)
{
	void *fdt;

	imx6_cpu_lowlevel_init();

	arm_setup_stack(0x00920000);

	IMD_USED(tx6x_mx6_memsize_1G);

	if (IS_ENABLED(CONFIG_DEBUG_LL))
		setup_uart();

	fdt = __dtb_imx6q_tx6q_start + get_runtime_offset();

	imx6q_barebox_entry(fdt);
}

BAREBOX_IMD_TAG_STRING(tx6x_mx6_memsize_2G, IMD_TYPE_PARAMETER, "memsize=2048", 0);

ENTRY_FUNCTION(start_imx6q_tx6x_2g, r0, r1, r2)
{
	void *fdt;

	imx6_cpu_lowlevel_init();

	arm_setup_stack(0x00920000);

	IMD_USED(tx6x_mx6_memsize_2G);

	if (IS_ENABLED(CONFIG_DEBUG_LL))
		setup_uart();

	fdt = __dtb_imx6q_tx6q_start + get_runtime_offset();

	imx6q_barebox_entry(fdt);
}
