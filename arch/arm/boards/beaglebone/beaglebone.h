/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __BOARD_BEAGLEBONE_H
#define __BOARD_BEAGLEBONE_H

#include <mach/omap/am33xx-generic.h>

static inline int is_beaglebone_black(void)
{
	return am33xx_get_cpu_rev() != AM335X_ES1_0;
}

#endif /* __BOARD_BEAGLEBONE_H */
