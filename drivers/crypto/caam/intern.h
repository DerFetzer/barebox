/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * CAAM/SEC 4.x driver backend
 * Private/internal definitions between modules
 *
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
 *
 */

#ifndef INTERN_H
#define INTERN_H

/* Currently comes from Kconfig param as a ^2 (driver-required) */
#define JOBR_DEPTH (1 << CONFIG_CRYPTO_DEV_FSL_CAAM_RINGSIZE)

/*
 * Storage for tracking each in-process entry moving across a ring
 * Each entry on an output ring needs one of these
 */
struct caam_jrentry_info {
	void (*callbk)(struct device *dev, u32 *desc, u32 status, void *arg);
	void *cbkarg;	/* Argument per ring entry */
	u32 *desc_addr_virt;	/* Stored virt addr for postprocessing */
	dma_addr_t desc_addr_dma;	/* Stored bus addr for done matching */
	u32 desc_size;	/* Stored size for postprocessing, header derived */
};

/* Private sub-storage for a single JobR */
struct caam_drv_private_jr {
	struct list_head	list_node;	/* Job Ring device list */
	struct device		*dev;
	int ridx;
	struct caam_job_ring __iomem *rregs;	/* JobR's register space */
	int irq;			/* One per queue */

	/* Number of scatterlist crypt transforms active on the JobR */
	int tfm_count;

	/* Job ring info */
	int ringsize;	/* Size of rings (assume input = output) */
	struct caam_jrentry_info *entinfo;	/* Alloc'ed 1 per ring entry */
	spinlock_t inplock; /* Input ring index lock */
	int inp_ring_write_index;	/* Input index "tail" */
	int head;			/* entinfo (s/w ring) head index */
	dma_addr_t *inpring;	/* Base of input ring, alloc DMA-safe */
	spinlock_t outlock; /* Output ring index lock */
	int out_ring_read_index;	/* Output index "tail" */
	int tail;			/* entinfo (s/w ring) tail index */
	struct jr_outentry *outring;	/* Base of output ring, DMA-safe */
};

/*
 * Driver-private storage for a single CAAM block instance
 */
struct caam_drv_private {

	struct device **jrpdev; /* Alloc'ed array per sub-device */
	struct device *pdev;

	/* Physical-presence section */
	struct caam_ctrl __iomem *ctrl; /* controller region */
	struct caam_deco __iomem *deco; /* DECO/CCB views */
	struct caam_assurance __iomem *assure;
	struct caam_queue_if __iomem *qi; /* QI control region */
	struct caam_job_ring __iomem *jr[4];	/* JobR's register space */
	struct snvs_full __iomem *snvs;	/* SNVS HP+LP register space */
	dma_addr_t __iomem *sm_base;	/* Secure memory storage base */
	u32 sm_size;

	/*
	 * Detected geometry block. Filled in from device tree if powerpc,
	 * or from register-based version detection code
	 */
	u8 total_jobrs;		/* Total Job Rings in device */
	u8 qi_present;		/* Nonzero if QI present in device */
	int secvio_irq;		/* Security violation interrupt number */
	int virt_en;		/* Virtualization enabled in CAAM */

#define	RNG4_MAX_HANDLES 2
	/* RNG4 block */
	u32 rng4_sh_init;	/* This bitmap shows which of the State
				   Handles of the RNG4 block are initialized
				   by this driver */

	struct clk *caam_ipg;
	struct clk *caam_mem;
	struct clk *caam_aclk;
	struct clk *caam_emi_slow;
};

int caam_rng_probe(struct device *dev, struct device *jrdev);
int caam_blob_gen_probe(struct device *dev, struct device *jrdev);
int caam_jr_probe(struct device *dev);
#endif /* INTERN_H */
