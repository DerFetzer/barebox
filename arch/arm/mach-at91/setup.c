/*
 * Copyright (C) 2007 Atmel Corporation.
 * Copyright (C) 2011 Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 *
 * Under GPLv2
 */

#include <common.h>
#include <io.h>
#include <init.h>
#include <restart.h>
#include <linux/clk.h>

#include <mach/at91/hardware.h>
#include <mach/at91/cpu.h>
#include <mach/at91/at91_dbgu.h>
#include <mach/at91/at91_rstc.h>
#include <mach/at91/board.h>

#include "generic.h"

/* function called by at91_detect() - if assigned */
void __initdata (*at91_boot_soc)(void);

struct at91_socinfo at91_soc_initdata;
EXPORT_SYMBOL(at91_soc_initdata);

static void __init dbgu_soc_detect(u32 dbgu_base)
{
	u32 cidr, socid;

	cidr = readl(dbgu_base + AT91_DBGU_CIDR);
	socid = cidr & ~AT91_CIDR_VERSION;

	/* sub version of soc */
	at91_soc_initdata.exid = readl(dbgu_base + AT91_DBGU_EXID);

	switch (socid) {
	case ARCH_ID_AT91RM9200:
		at91_soc_initdata.type = AT91_SOC_RM9200;
		if (at91_soc_initdata.subtype == AT91_SOC_SUBTYPE_NONE)
			at91_soc_initdata.subtype = AT91_SOC_RM9200_BGA;
		break;

	case ARCH_ID_AT91SAM9260:
		at91_soc_initdata.type = AT91_SOC_SAM9260;
		break;

	case ARCH_ID_AT91SAM9261:
		at91_soc_initdata.type = AT91_SOC_SAM9261;
		break;

	case ARCH_ID_AT91SAM9263:
		at91_soc_initdata.type = AT91_SOC_SAM9263;
		break;

	case ARCH_ID_AT91SAM9G20:
		at91_soc_initdata.type = AT91_SOC_SAM9G20;
		break;

	case ARCH_ID_AT91SAM9G45:
		at91_soc_initdata.type = AT91_SOC_SAM9G45;
		if (cidr == ARCH_ID_AT91SAM9G45ES)
			at91_soc_initdata.subtype = AT91_SOC_SAM9G45ES;
		break;

	case ARCH_ID_AT91SAM9RL64:
		at91_soc_initdata.type = AT91_SOC_SAM9RL;
		break;

	case ARCH_ID_AT91SAM9X5:
		at91_soc_initdata.type = AT91_SOC_SAM9X5;
		break;

	case ARCH_ID_AT91SAM9N12:
		at91_soc_initdata.type = AT91_SOC_SAM9N12;
		break;

	case ARCH_ID_SAMA5:
		if (at91_soc_initdata.exid & ARCH_EXID_SAMA5D3) {
			at91_soc_initdata.type = AT91_SOC_SAMA5D3;
		} else {
			if (at91_soc_initdata.exid & ARCH_EXID_SAMA5D4) {
				at91_soc_initdata.type = AT91_SOC_SAMA5D4;
			}
		}
		break;
	}

	/* at91sam9g10 */
	if ((socid & ~AT91_CIDR_EXT) == ARCH_ID_AT91SAM9G10) {
		at91_soc_initdata.type = AT91_SOC_SAM9G10;
	}
	/* at91sam9xe */
	else if ((cidr & AT91_CIDR_ARCH) == ARCH_FAMILY_AT91SAM9XE) {
		at91_soc_initdata.type = AT91_SOC_SAM9260;
		at91_soc_initdata.subtype = AT91_SOC_SAM9XE;
	}

	if (!at91_soc_is_detected())
		return;

	at91_soc_initdata.cidr = cidr;

	if (at91_soc_initdata.type == AT91_SOC_SAM9G45) {
		switch (at91_soc_initdata.exid) {
		case ARCH_EXID_AT91SAM9M10:
			at91_soc_initdata.subtype = AT91_SOC_SAM9M10;
			break;
		case ARCH_EXID_AT91SAM9G46:
			at91_soc_initdata.subtype = AT91_SOC_SAM9G46;
			break;
		case ARCH_EXID_AT91SAM9M11:
			at91_soc_initdata.subtype = AT91_SOC_SAM9M11;
			break;
		}
	}

	if (at91_soc_initdata.type == AT91_SOC_SAM9X5) {
		switch (at91_soc_initdata.exid) {
		case ARCH_EXID_AT91SAM9G15:
			at91_soc_initdata.subtype = AT91_SOC_SAM9G15;
			break;
		case ARCH_EXID_AT91SAM9G35:
			at91_soc_initdata.subtype = AT91_SOC_SAM9G35;
			break;
		case ARCH_EXID_AT91SAM9X35:
			at91_soc_initdata.subtype = AT91_SOC_SAM9X35;
			break;
		case ARCH_EXID_AT91SAM9G25:
			at91_soc_initdata.subtype = AT91_SOC_SAM9G25;
			break;
		case ARCH_EXID_AT91SAM9X25:
			at91_soc_initdata.subtype = AT91_SOC_SAM9X25;
			break;
		}
	}

	if (at91_soc_initdata.type == AT91_SOC_SAM9N12) {
		switch (at91_soc_initdata.exid) {
		case ARCH_EXID_AT91SAM9N12:
			at91_soc_initdata.subtype = AT91_SOC_SAM9N12;
			break;
		case ARCH_EXID_AT91SAM9CN11:
			at91_soc_initdata.subtype = AT91_SOC_SAM9CN11;
			break;
		case ARCH_EXID_AT91SAM9CN12:
			at91_soc_initdata.subtype = AT91_SOC_SAM9CN12;
			break;
		}
	}

	if (at91_soc_initdata.type == AT91_SOC_SAMA5D3) {
		switch (at91_soc_initdata.exid) {
		case ARCH_EXID_SAMA5D31:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D31;
			break;
		case ARCH_EXID_SAMA5D33:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D33;
			break;
		case ARCH_EXID_SAMA5D34:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D34;
			break;
		case ARCH_EXID_SAMA5D35:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D35;
			break;
		case ARCH_EXID_SAMA5D36:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D36;
			break;
		}
	}

	if (at91_soc_initdata.type == AT91_SOC_SAMA5D4) {
		switch (at91_soc_initdata.exid) {
		case ARCH_EXID_SAMA5D41:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D41;
			break;
		case ARCH_EXID_SAMA5D42:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D42;
			break;
		case ARCH_EXID_SAMA5D43:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D43;
			break;
		case ARCH_EXID_SAMA5D44:
			at91_soc_initdata.subtype = AT91_SOC_SAMA5D44;
			break;
		}
	}
}

static void __init chipid_soc_detect(u32 chipid_base)
{
	u32 cidr, socid;

	cidr = readl(chipid_base);
	socid = cidr & AT91_CIDR_ARCH;

	if (!(cidr & AT91_CIDR_EXT))
		return;

	if (socid == (ARCH_ID_SAMA5 & AT91_CIDR_ARCH)) {
		at91_soc_initdata.exid = readl(chipid_base + 4);

		switch (at91_soc_initdata.exid) {
			case ARCH_EXID_SAMA5D21CU:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D21CU;
				break;
			case ARCH_EXID_SAMA5D225C_D1M:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D225C_D1M;
				break;
			case ARCH_EXID_SAMA5D22CU:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D22CU;
				break;
			case ARCH_EXID_SAMA5D22CN:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D22CN;
				break;
			case ARCH_EXID_SAMA5D23CU:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D23CU;
				break;
			case ARCH_EXID_SAMA5D24CX:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D24CX;
				break;
			case ARCH_EXID_SAMA5D24CU:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D24CU;
				break;
			case ARCH_EXID_SAMA5D26CU:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D26CU;
				break;
			case ARCH_EXID_SAMA5D27C_D1G:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D27C_D1G;
				break;
			case ARCH_EXID_SAMA5D27C_D5M:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D27C_D5M;
				break;
			case ARCH_EXID_SAMA5D27CU:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D27CU;
				break;
			case ARCH_EXID_SAMA5D27CN:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D27CN;
				break;
			case ARCH_EXID_SAMA5D28C_D1G:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D28C_D1G;
				break;
			case ARCH_EXID_SAMA5D28CU:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D28CU;
				break;
			case ARCH_EXID_SAMA5D28CN:
				at91_soc_initdata.type = AT91_SOC_SAMA5D2;
				at91_soc_initdata.subtype = AT91_SOC_SAMA5D28CN;
				break;
		}
	}
}

static const char *soc_name[] = {
	[AT91_SOC_RM9200]	= "at91rm9200",
	[AT91_SOC_SAM9260]	= "at91sam9260",
	[AT91_SOC_SAM9261]	= "at91sam9261",
	[AT91_SOC_SAM9263]	= "at91sam9263",
	[AT91_SOC_SAM9G10]	= "at91sam9g10",
	[AT91_SOC_SAM9G20]	= "at91sam9g20",
	[AT91_SOC_SAM9G45]	= "at91sam9g45",
	[AT91_SOC_SAM9RL]	= "at91sam9rl",
	[AT91_SOC_SAM9X5]	= "at91sam9x5",
	[AT91_SOC_SAM9N12]	= "at91sam9n12",
	[AT91_SOC_SAMA5D2]	= "sama5d2",
	[AT91_SOC_SAMA5D3]	= "sama5d3",
	[AT91_SOC_SAMA5D4]	= "sama5d4",
	[AT91_SOC_NONE]		= "Unknown"
};

const char *at91_get_soc_type(struct at91_socinfo *c)
{
	return soc_name[c->type];
}
EXPORT_SYMBOL(at91_get_soc_type);

static const char *soc_subtype_name[] = {
	[AT91_SOC_RM9200_BGA]	= "at91rm9200 BGA",
	[AT91_SOC_RM9200_PQFP]	= "at91rm9200 PQFP",
	[AT91_SOC_SAM9XE]	= "at91sam9xe",
	[AT91_SOC_SAM9G45ES]	= "at91sam9g45es",
	[AT91_SOC_SAM9M10]	= "at91sam9m10",
	[AT91_SOC_SAM9G46]	= "at91sam9g46",
	[AT91_SOC_SAM9M11]	= "at91sam9m11",
	[AT91_SOC_SAM9G15]	= "at91sam9g15",
	[AT91_SOC_SAM9G35]	= "at91sam9g35",
	[AT91_SOC_SAM9X35]	= "at91sam9x35",
	[AT91_SOC_SAM9G25]	= "at91sam9g25",
	[AT91_SOC_SAM9X25]	= "at91sam9x25",
	[AT91_SOC_SAM9N12]	= "at91sam9n12",
	[AT91_SOC_SAM9CN11]	= "at91sam9cn11",
	[AT91_SOC_SAM9CN12]	= "at91sam9cn12",
	[AT91_SOC_SAMA5D21CU]	= "sama5d21cu",
	[AT91_SOC_SAMA5D225C_D1M]	= "sama5d225c_d1m",
	[AT91_SOC_SAMA5D22CU]	= "sama5d22cu",
	[AT91_SOC_SAMA5D22CN]	= "sama5d22cn",
	[AT91_SOC_SAMA5D23CU]	= "sama5d23cu",
	[AT91_SOC_SAMA5D24CX]	= "sama5d24cx",
	[AT91_SOC_SAMA5D24CU]	= "sama5d24cu",
	[AT91_SOC_SAMA5D26CU]	= "sama5d26cu",
	[AT91_SOC_SAMA5D27C_D1G]	= "sama5d27c_d1g",
	[AT91_SOC_SAMA5D27C_D5M]	= "sama5d27c_d5m",
	[AT91_SOC_SAMA5D27CU]	= "sama5d27cu",
	[AT91_SOC_SAMA5D27CN]	= "sama5d27cn",
	[AT91_SOC_SAMA5D28C_D1G]	= "sama5d28c_d1g",
	[AT91_SOC_SAMA5D28CU]	= "sama5d28cu",
	[AT91_SOC_SAMA5D28CN]	= "sama5d28cn",
	[AT91_SOC_SAMA5D31]	= "sama5d31",
	[AT91_SOC_SAMA5D33]	= "sama5d33",
	[AT91_SOC_SAMA5D34]	= "sama5d34",
	[AT91_SOC_SAMA5D35]	= "sama5d35",
	[AT91_SOC_SAMA5D36]	= "sama5d36",
	[AT91_SOC_SAMA5D41]	= "sama5d41",
	[AT91_SOC_SAMA5D42]	= "sama5d42",
	[AT91_SOC_SAMA5D43]	= "sama5d43",
	[AT91_SOC_SAMA5D44]	= "sama5d44",
	[AT91_SOC_SUBTYPE_NONE]	= "Unknown"
};

const char *at91_get_soc_subtype(struct at91_socinfo *c)
{
	return soc_subtype_name[c->subtype];
}
EXPORT_SYMBOL(at91_get_soc_subtype);

static int at91_detect(void)
{
	at91_soc_initdata.type = AT91_SOC_NONE;
	at91_soc_initdata.subtype = AT91_SOC_SUBTYPE_NONE;

	dbgu_soc_detect(AT91_BASE_DBGU0);
	if (!at91_soc_is_detected())
		dbgu_soc_detect(AT91_BASE_DBGU1);
	if (!at91_soc_is_detected())
		dbgu_soc_detect(AT91_BASE_DBGU2);
	if (!at91_soc_is_detected())
		chipid_soc_detect(0xfc069000);

	if (!at91_soc_is_detected())
		panic("AT91: Impossible to detect the SOC type");

	pr_info("AT91: Detected soc type: %s\n",
		at91_get_soc_type(&at91_soc_initdata));
	pr_info("AT91: Detected soc subtype: %s\n",
		at91_get_soc_subtype(&at91_soc_initdata));

	/* Init clock subsystem */
	at91_clock_init();

	if (at91_boot_soc != NULL)
		at91_boot_soc();

	return 0;
}
postcore_initcall(at91_detect);

static int at91_soc_device(void)
{
	struct device *dev;

	dev = add_generic_device_res("soc", DEVICE_ID_SINGLE, NULL, 0, NULL);
	dev_add_param_fixed(dev, "name", (char*)at91_get_soc_type(&at91_soc_initdata));
	dev_add_param_fixed(dev, "subname", (char*)at91_get_soc_subtype(&at91_soc_initdata));

	return 0;
}
coredevice_initcall(at91_soc_device);

void at91sam_phy_reset(void __iomem *rstc_base)
{
	unsigned long rstc;
	struct clk *clk = clk_get(NULL, "macb_clk");

	clk_enable(clk);

	rstc = readl(rstc_base + AT91_RSTC_MR) & AT91_RSTC_ERSTL;

	/* Need to reset PHY -> 500ms reset */
	writel(AT91_RSTC_KEY | (AT91_RSTC_ERSTL & (0x0d << 8)) | AT91_RSTC_URSTEN,
		rstc_base + AT91_RSTC_MR);

	writel(AT91_RSTC_KEY | AT91_RSTC_EXTRST, rstc_base + AT91_RSTC_CR);

	/* Wait for end hardware reset */
	while (!(readl(rstc_base + AT91_RSTC_SR) & AT91_RSTC_NRSTL))
		;

	/* Restore NRST value */
	writel(AT91_RSTC_KEY | (rstc) | AT91_RSTC_URSTEN, rstc_base + AT91_RSTC_MR);
}
