// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * This is file is based on
 * repository git.gitorious.org/u-boot-omap3/mainline.git,
 * branch omap3-dev-usb, file drivers/usb/gadget/twl4030_usb.c
 *
 * This is the unique part of its copyright :
 *
 * ------------------------------------------------------------------------
 *
 *  * (C) Copyright 2009 Atin Malaviya (atin.malaviya@gmail.com)
 *
 * Based on: twl4030_usb.c in linux 2.6 (drivers/i2c/chips/twl4030_usb.c)
 * Copyright (C) 2004-2007 Texas Instruments
 * Copyright (C) 2008 Nokia Corporation
 * Contact: Felipe Balbi <felipe.balbi@nokia.com>
 *
 * Author: Atin Malaviya (atin.malaviya@gmail.com)
 *
 * ------------------------------------------------------------------------
 */

#include <mfd/twl4030.h>
#include <linux/usb/twl4030.h>
#include <clock.h>

static int twl4030_usb_write(u8 address, u8 data)
{
	int ret;
	struct twl4030 *twl4030 = twl4030_get();

	ret = twl4030_reg_write(twl4030, address, data);
	if (ret != 0)
		printf("TWL4030:USB:Write[0x%x] Error %d\n", address, ret);

	return ret;
}

static int twl4030_usb_read(u8 address)
{
	u8 data;
	int ret;
	struct twl4030 *twl4030 = twl4030_get();

	ret = twl4030_reg_read(twl4030, address, &data);
	if (ret == 0)
		ret = data;
	else
		printf("TWL4030:USB:Read[0x%x] Error %d\n", address, ret);

	return ret;
}

static void twl4030_power_init(void)
{
	unsigned char byte;
	struct twl4030 *twl4030 = twl4030_get();

	/* set VAUX3 to 2.8V */
	byte = TWL4030_PM_RECEIVER_DEV_GRP_P1;
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VAUX3_DEV_GRP, byte);
	byte = TWL4030_PM_RECEIVER_VAUX3_VSEL_28;
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VAUX3_DEDICATED, byte);

	/* set VPLL2 to 1.8V */
	byte = TWL4030_PM_RECEIVER_DEV_GRP_ALL;
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VPLL2_DEV_GRP, byte);
	byte = TWL4030_PM_RECEIVER_VPLL2_VSEL_18;
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VPLL2_DEDICATED, byte);

	/* set VDAC to 1.8V */
	byte = TWL4030_PM_RECEIVER_DEV_GRP_P1;
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VDAC_DEV_GRP, byte);
	byte = TWL4030_PM_RECEIVER_VDAC_VSEL_18;
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VDAC_DEDICATED, byte);
}

static void twl4030_usb_ldo_init(void)
{
	struct twl4030 *twl4030 = twl4030_get();
	/* Enable writing to power configuration registers */
	twl4030_reg_write(twl4030, TWL4030_PM_MASTER_PROTECT_KEY, 0xC0);
	twl4030_reg_write(twl4030, TWL4030_PM_MASTER_PROTECT_KEY, 0x0C);

	/* put VUSB3V1 LDO in active state */
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB_DEDICATED2, 0x00);

	/* input to VUSB3V1 LDO is from VBAT, not VBUS */
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB_DEDICATED1, 0x14);

	/* turn on 3.1V regulator */
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB3V1_DEV_GRP, 0x20);
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB3V1_TYPE, 0x00);

	/* turn on 1.5V regulator */
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB1V5_DEV_GRP, 0x20);
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB1V5_TYPE, 0x00);

	/* turn on 1.8V regulator */
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB1V8_DEV_GRP, 0x20);
	twl4030_reg_write(twl4030, TWL4030_PM_RECEIVER_VUSB1V8_TYPE, 0x00);

	/* disable access to power configuration registers */
	twl4030_reg_write(twl4030, TWL4030_PM_MASTER_PROTECT_KEY, 0x0);

	/* FIXME *need to enable LED to get USB power? */
	twl4030_reg_write(twl4030, TWL4030_BASEADD_LED, 0x33);
}

static void twl4030_phy_power(void)
{
	u8 pwr, clk;
	/* Power the PHY */
	pwr = twl4030_usb_read(TWL4030_USB_PHY_PWR_CTRL);
	pwr &= ~PHYPWD;
	twl4030_usb_write(TWL4030_USB_PHY_PWR_CTRL, pwr);
	/* Enable clocks */
	clk = twl4030_usb_read(TWL4030_USB_PHY_CLK_CTRL);
	clk |= CLOCKGATING_EN | CLK32K_EN;
	twl4030_usb_write(TWL4030_USB_PHY_CLK_CTRL, clk);
}

/*
 * Initiaze the ULPI interface
 * ULPI : Universal Transceiver Macrocell Low Pin Interface
 * An interface between the USB link controller like musb and the
 * the PHY or transceiver that drives the actual bus.
 */
int twl4030_usb_ulpi_init(void)
{
	u8 clk, sts, pwr;
	uint64_t start;
	long timeout = 1000 * 1000; /* 1 sec */;

	/* power up the twl4030 */
	twl4030_power_init();

	/* twl4030 ldo init */
	twl4030_usb_ldo_init();

	/* Enable the twl4030 phy */
	twl4030_phy_power();

	/* Enable DPLL to access PHY registers over I2C */
	clk = twl4030_usb_read(TWL4030_USB_PHY_CLK_CTRL);
	clk |= REQ_PHY_DPLL_CLK;
	twl4030_usb_write(TWL4030_USB_PHY_CLK_CTRL, clk);

	/* Check if the PHY DPLL is locked */
	sts = twl4030_usb_read(TWL4030_USB_PHY_CLK_CTRL_STS);
	start = get_time_ns();
	while (!(sts & PHY_DPLL_CLK)) {
		udelay(10);
		sts = twl4030_usb_read(TWL4030_USB_PHY_CLK_CTRL_STS);
		printf("Error:TWL4030:USB Timeout setting PHY DPLL clock\n");
		if (is_timeout(start, timeout * USECOND)) {
			return -ETIMEDOUT;
		}
	}

	/*
	 * There are two circuit blocks attached to the PHY,
	 * Carkit and USB OTG.  Disable Carkit and enable USB OTG
	 */
	twl4030_usb_write(TWL4030_USB_IFC_CTRL_CLR, CARKITMODE);
	pwr = twl4030_usb_read(TWL4030_USB_POWER_CTRL);
	pwr |= OTG_ENAB;
	twl4030_usb_write(TWL4030_USB_POWER_CTRL_SET, pwr);

	/* Clear the opmode bits to ensure normal encode */
	twl4030_usb_write(TWL4030_USB_FUNC_CTRL_CLR, OPMODE_MASK);

	/* Clear the xcvrselect bits to enable the high speed transeiver */
	twl4030_usb_write(TWL4030_USB_FUNC_CTRL_CLR, XCVRSELECT_MASK);

	/* Let ULPI control the DPLL clock */
	clk = twl4030_usb_read(TWL4030_USB_PHY_CLK_CTRL);
	clk &= ~REQ_PHY_DPLL_CLK;
	twl4030_usb_write(TWL4030_USB_PHY_CLK_CTRL, clk);

	return 0;
}
