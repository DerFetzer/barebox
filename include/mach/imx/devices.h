/* SPDX-License-Identifier: GPL-2.0-only */

#include <platform_data/eth-fec.h>
#include <input/matrix_keypad.h>
#include <i2c/i2c.h>
#include <mach/imx/spi.h>
#include <mach/imx/imx-nand.h>
#include <platform_data/imxfb.h>
#include <mach/imx/imx-ipu-fb.h>
#include <platform_data/mmc-esdhc-imx.h>
#include <linux/usb/chipidea-imx.h>

struct device *imx_add_fec_imx27(void *base, struct fec_platform_data *pdata);
struct device *imx_add_fec_imx6(void *base, struct fec_platform_data *pdata);
struct device *imx_add_spi_imx27(void *base, int id, struct spi_imx_master *pdata);
struct device *imx_add_spi_imx35(void *base, int id, struct spi_imx_master *pdata);
struct device *imx_add_spi_imx51(void *base, int id, struct spi_imx_master *pdata);
struct device *imx_add_i2c(void *base, int id, struct i2c_platform_data *pdata);
struct device *imx_add_uart_imx1(void *base, int id);
struct device *imx_add_uart_imx21(void *base, int id);
struct device *imx_add_nand(void *base, struct imx_nand_platform_data *pdata);
struct device *imx_add_fb(void *base, struct imx_fb_platform_data *pdata);
struct device *imx_add_ipufb(void *base, struct imx_ipu_fb_platform_data *pdata);
struct device *imx_add_mmc(void *base, int id, void *pdata);
struct device *imx_add_esdhc_imx25(void *base, int id, struct esdhc_platform_data *pdata);
struct device *imx_add_esdhc_imx5(void *base, int id, struct esdhc_platform_data *pdata);
struct device *imx_add_kpp(void *base, struct matrix_keymap_data *pdata);
struct device *imx_add_pata(void *base);
struct device *imx_add_usb(void *base, int id, struct imxusb_platformdata *pdata);
