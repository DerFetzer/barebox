// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2009 Matthias Kaehlcke <matthias@kaehlcke.net>

/* PLL register values for Cirrus edb93xx boards */

#include <config.h>
#include <mach/ep93xx/ep93xx-regs.h>

#if defined(CONFIG_MACH_EDB9301)
/*
 * fclk_div: 2, nbyp1: 1, hclk_div: 5, pclk_div: 2
 * pll1_x1: 294912000.000000, pll1_x2ip: 36864000.000000,
 * pll1_x2: 331776000.000000, pll1_out: 331776000.000000
 */
#define CLKSET1_VAL	(7 << SYSCON_CLKSET_PLL_X2IPD_SHIFT |      \
			8 << SYSCON_CLKSET_PLL_X2FBD2_SHIFT |	   \
			19 << SYSCON_CLKSET_PLL_X1FBD1_SHIFT |	   \
			1 << SYSCON_CLKSET1_PCLK_DIV_SHIFT |	   \
			3 << SYSCON_CLKSET1_HCLK_DIV_SHIFT |	   \
			SYSCON_CLKSET1_NBYP1 |			   \
			1 << SYSCON_CLKSET1_FCLK_DIV_SHIFT)
#elif defined(CONFIG_MACH_EDB9302) || defined(CONFIG_MACH_EDB9302A) ||	\
	defined(CONFIG_MACH_EDB9307) || defined(CONFIG_MACH_EDB9307A) || \
	defined CONFIG_MACH_EDB9312 || defined(CONFIG_MACH_EDB9315) ||\
	defined(CONFIG_MACH_EDB9315A)
/*
 * fclk_div: 2, nbyp1: 1, hclk_div: 4, pclk_div: 2
 * pll1_x1: 3096576000.000000, pll1_x2ip: 129024000.000000,
 * pll1_x2: 3999744000.000000, pll1_out: 1999872000.000000
 */
#define CLKSET1_VAL	(23 << SYSCON_CLKSET_PLL_X2IPD_SHIFT | \
			30 << SYSCON_CLKSET_PLL_X2FBD2_SHIFT | \
			20 << SYSCON_CLKSET_PLL_X1FBD1_SHIFT | \
			1 << SYSCON_CLKSET1_PCLK_DIV_SHIFT |   \
			2 << SYSCON_CLKSET1_HCLK_DIV_SHIFT |   \
			SYSCON_CLKSET1_NBYP1 |		       \
			1 << SYSCON_CLKSET1_FCLK_DIV_SHIFT)
#else
#error "Undefined board"
#endif

/*
 * usb_div: 4, nbyp2: 1, pll2_en: 1
 * pll2_x1: 368640000.000000, pll2_x2ip: 15360000.000000,
 * pll2_x2: 384000000.000000, pll2_out: 192000000.000000
 */
#define CLKSET2_VAL	(23 << SYSCON_CLKSET_PLL_X2IPD_SHIFT | \
			24 << SYSCON_CLKSET_PLL_X2FBD2_SHIFT | \
			24 << SYSCON_CLKSET_PLL_X1FBD1_SHIFT | \
			1 << SYSCON_CLKSET_PLL_PS_SHIFT |      \
			SYSCON_CLKSET2_PLL2_EN |	       \
			SYSCON_CLKSET2_NBYP2 |		       \
			3 << SYSCON_CLKSET2_USB_DIV_SHIFT)
