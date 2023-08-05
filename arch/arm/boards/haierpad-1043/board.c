// SPDX-License-Identifier: GPL-2.0-only
// SPDX-FileCopyrightText: 2014 Beniamino Galvani <b.galvani@gmail.com>

#include <common.h>
#include <init.h>
#include <io.h>
#include <envfs.h>
#include <i2c/i2c.h>
#include <i2c/i2c-gpio.h>
#include <mach/rockchip/rk3188-regs.h>
#include <asm/armlinux.h>

static int devices_init(void)
{
	if (!of_machine_is_compatible("haierpad,1043"))
		return 0;

	armlinux_set_architecture(3066);

	/* Map SRAM to address 0, kernel relies on this */
	writel((RK_SOC_CON0_REMAP << 16) | RK_SOC_CON0_REMAP,
	    RK_GRF_BASE + RK_GRF_SOC_CON0);

	defaultenv_append_directory(defaultenv_haierpad_1043);

	return 0;
}
device_initcall(devices_init);
