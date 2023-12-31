/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __MACH_IMX_IMX6_OCOTP_H
#define __MACH_IMX_IMX6_OCOTP_H

#include <mach/imx/ocotp-fusemap.h>

#define IMX6_OCOTP_SI_REV		(OCOTP_WORD(0x430) | OCOTP_BIT(16) | OCOTP_WIDTH(4))
#define IMX6_OCOTP_SATA_RST_SRC		(OCOTP_WORD(0x430) | OCOTP_BIT(24) | OCOTP_WIDTH(1))
#define IMX6_OCOTP_VPU_DISABLE		(OCOTP_WORD(0x440) | OCOTP_BIT(15) | OCOTP_WIDTH(1))
#define IMX6_OCOTP_SPEED_GRADING	(OCOTP_WORD(0x440) | OCOTP_BIT(16) | OCOTP_WIDTH(2))
#define IMX6_OCOTP_DDR3_CONFIG		(OCOTP_WORD(0x460) | OCOTP_BIT(8) | OCOTP_WIDTH(8))
#define IMX6_OCOTP_HDCP			(OCOTP_WORD(0x460) | OCOTP_BIT(16) | OCOTP_WIDTH(1))
#define IMX6_OCOTP_TZASC_ENABLE		(OCOTP_WORD(0x460) | OCOTP_BIT(28) | OCOTP_WIDTH(1))
#define IMX6_OCOTP_SDMMC_HYS_EN		(OCOTP_WORD(0x460) | OCOTP_BIT(29) | OCOTP_WIDTH(1))
#define IMX6_OCOTP_eMMC_RESET_EN	(OCOTP_WORD(0x460) | OCOTP_BIT(30) | OCOTP_WIDTH(1))
#define IMX6_OCOTP_BT_LPB_POLARITY	(OCOTP_WORD(0x470) | OCOTP_BIT(20) | OCOTP_WIDTH(1))
#define IMX6_OCOTP_LPB_BOOT		(OCOTP_WORD(0x470) | OCOTP_BIT(21) | OCOTP_WIDTH(2))
#define IMX6_OCOTP_MMC_DLL_DLY		(OCOTP_WORD(0x470) | OCOTP_BIT(24) | OCOTP_WIDTH(7))
#define IMX6_OCOTP_TEMPERATURE_GRADE	(OCOTP_WORD(0x480) | OCOTP_BIT(6) | OCOTP_WIDTH(2))
#define IMX6_OCOTP_POWER_GATE_CORES	(OCOTP_WORD(0x4d0) | OCOTP_BIT(31) | OCOTP_WIDTH(1))
#define IMX6DQ_OCOTP_TEST_PORT_DISABLE	(OCOTP_WORD(0x6e0) | OCOTP_BIT(1) | OCOTP_WIDTH(1))
#define IMX6SDL_OCOTP_FIELD_RETURN	(OCOTP_WORD(0x6e0) | OCOTP_BIT(0) | OCOTP_WIDTH(1))

#endif /* __MACH_IMX_IMX6_OCOTP_H */
