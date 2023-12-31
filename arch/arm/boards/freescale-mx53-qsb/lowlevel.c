// SPDX-License-Identifier: GPL-2.0-only

#include <common.h>
#include <mach/imx/imx53-regs.h>
#include <mach/imx/esdctl.h>
#include <mach/imx/generic.h>
#include <asm/barebox-arm-head.h>
#include <asm/barebox-arm.h>
#include <image-metadata.h>

extern char __dtb_imx53_qsb_start[];

ENTRY_FUNCTION(start_imx53_loco, r0, r1, r2)
{
	void *fdt;

	imx5_cpu_lowlevel_init();
	arm_setup_stack(MX53_IRAM_BASE_ADDR + MX53_IRAM_SIZE);

	fdt = __dtb_imx53_qsb_start + get_runtime_offset();

	imx53_barebox_entry(fdt);
}

extern char __dtb_imx53_qsrb_start[];

ENTRY_FUNCTION(start_imx53_loco_r, r0, r1, r2)
{
	void *fdt;

	imx5_cpu_lowlevel_init();
	arm_setup_stack(MX53_IRAM_BASE_ADDR + MX53_IRAM_SIZE);

	fdt = __dtb_imx53_qsrb_start + get_runtime_offset();

	imx53_barebox_entry(fdt);
}
