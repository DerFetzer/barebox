# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for the CAAM backend and dependent components
#
obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM) += ctrl.o error.o jr.o
obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM_RNG) += caamrng.o
obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM) += rng_self_test.o
obj-$(CONFIG_BLOBGEN) += caam-blobgen.o
pbl-$(CONFIG_FSL_CAAM_RNG_PBL_INIT) += pbl-init.o
