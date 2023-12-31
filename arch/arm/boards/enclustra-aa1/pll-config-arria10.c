// SPDX-License-Identifier: GPL-2.0-only

#include <mach/socfpga/arria10-clock-manager.h>

static struct arria10_mainpll_cfg mainpll_cfg = {
	.cntr15clk_cnt	   = 900,
	.cntr2clk_cnt	   = 900,
	.cntr3clk_cnt	   = 900,
	.cntr4clk_cnt	   = 900,
	.cntr5clk_cnt	   = 900,
	.cntr6clk_cnt	   = 7,
	.cntr7clk_cnt	   = 15,
	.cntr7clk_src	   = 0,
	.cntr8clk_cnt	   = 7,
	.cntr9clk_cnt	   = 900,
	.cntr9clk_src	   = 0,
	.mpuclk_cnt	   = 0,
	.mpuclk_src	   = 0,
	.nocclk_cnt	   = 0,
	.nocclk_src	   = 0,
	.nocdiv_csatclk	   = 0,
	.nocdiv_cspdbgclk  = 1,
	.nocdiv_cstraceclk = 0,
	.nocdiv_l4mainclk  = 0,
	.nocdiv_l4mpclk	   = 1,
	.nocdiv_l4spclk	   = 2,
	.vco0_psrc	   = 0,
	.vco1_denom	   = 32,
	.vco1_numer	   = 1584,
	.mpuclk		   = 0x3840001,
	.nocclk		   = 0x3840007,
};

static struct arria10_perpll_cfg perpll_cfg = {
	.cntr2clk_cnt	   = 5,
	.cntr2clk_src	   = 1,
	.cntr3clk_cnt	   = 900,
	.cntr3clk_src	   = 1,
	.cntr4clk_cnt	   = 14,
	.cntr4clk_src	   = 1,
	.cntr5clk_cnt	   = 374,
	.cntr5clk_src	   = 1,
	.cntr6clk_cnt	   = 900,
	.cntr6clk_src	   = 0,
	.cntr7clk_cnt	   = 900,
	.cntr8clk_cnt	   = 900,
	.cntr8clk_src	   = 0,
	.cntr9clk_cnt	   = 900,
	.emacctl_emac0sel  = 0,
	.emacctl_emac1sel  = 0,
	.emacctl_emac2sel  = 0,
	.gpiodiv_gpiodbclk = 32000,
	.vco0_psrc	   = 0,
	.vco1_denom	   = 32,
	.vco1_numer	   = 1485,
};
