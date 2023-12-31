/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __ASM_MMU_H
#define __ASM_MMU_H

#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <xfuncs.h>

struct arm_memory;

void mmu_disable(void);
static inline void arm_create_section(unsigned long virt, unsigned long phys, int size_m,
		unsigned int flags)
{
}

static inline void setup_dma_coherent(unsigned long offset)
{
}

#ifdef CONFIG_MMU
#define ARCH_HAS_REMAP
#define MAP_ARCH_DEFAULT MAP_CACHED
int arch_remap_range(void *virt_addr, phys_addr_t phys_addr, size_t size, unsigned flags);
void *map_io_sections(unsigned long physaddr, void *start, size_t size);
#else
#define MAP_ARCH_DEFAULT MAP_UNCACHED
static inline void *map_io_sections(unsigned long phys, void *start, size_t size)
{
	return (void *)phys;
}

#endif

#ifdef CONFIG_CACHE_L2X0
void __init l2x0_init(void __iomem *base, __u32 aux_val, __u32 aux_mask);
#else
static inline void __init l2x0_init(void __iomem *base, __u32 aux_val, __u32 aux_mask)
{
}
#endif

struct outer_cache_fns {
	void (*inv_range)(unsigned long, unsigned long);
	void (*clean_range)(unsigned long, unsigned long);
	void (*flush_range)(unsigned long, unsigned long);
	void (*flush_all)(void);
	void (*disable)(void);
};

#ifdef __PBL__
/* Existing platforms with non-architected outer cache initialize it
 * outside PBL and new ones will likely only have architected caches,
 * so we provide a dummy here
 */
static __maybe_unused struct outer_cache_fns outer_cache;
#else
extern struct outer_cache_fns outer_cache;
#endif

void __dma_clean_range(unsigned long, unsigned long);
void __dma_flush_range(unsigned long, unsigned long);
void __dma_inv_range(unsigned long, unsigned long);

void mmu_early_enable(unsigned long membase, unsigned long memsize);
void mmu_early_disable(void);

#endif /* __ASM_MMU_H */
