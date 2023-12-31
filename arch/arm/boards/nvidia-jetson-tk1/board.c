// SPDX-License-Identifier: GPL-2.0-only
// SPDX-FileCopyrightText: 2014 Lucas Stach <l.stach@pengutronix.de>

#include <common.h>
#include <dt-bindings/gpio/tegra-gpio.h>
#include <gpio.h>
#include <i2c/i2c.h>
#include <init.h>
#include <mach/tegra/tegra-bbu.h>

#define AS3722_SD_VOLTAGE(n)	(0x00 + (n))
#define AS3722_GPIO_CONTROL(n)	(0x08 + (n))
#define  AS3722_GPIO_CONTROL_MODE_OUTPUT_VDDH (1 << 0)
#define AS3722_GPIO_SIGNAL_OUT	0x20
#define AS3722_SD_CONTROL	0x4d

static int nvidia_jetson_tk1_fs_init(void)
{
	struct i2c_client client;
	u8 data;

	if (!of_machine_is_compatible("nvidia,jetson-tk1"))
		return 0;

	client.adapter = i2c_get_adapter(4);
	client.addr = 0x40;

	/* AS3722: enable SD4 and set voltage to 1.05v */
	i2c_read_reg(&client, AS3722_SD_CONTROL, &data, 1);
	data |= 1 << 4;
	i2c_write_reg(&client, AS3722_SD_CONTROL, &data, 1);

	data = 0x24;
	i2c_write_reg(&client, AS3722_SD_VOLTAGE(4), &data, 1);

	return 0;
}
fs_initcall(nvidia_jetson_tk1_fs_init);

static int nvidia_jetson_tk1_device_init(void)
{
	if (!of_machine_is_compatible("nvidia,jetson-tk1"))
		return 0;

	barebox_set_hostname("jetson-tk1");

	tegra_bbu_register_emmc_handler("eMMC", "/dev/mmc3.boot0",
					BBU_HANDLER_FLAG_DEFAULT);

	return 0;
}
device_initcall(nvidia_jetson_tk1_device_init);
