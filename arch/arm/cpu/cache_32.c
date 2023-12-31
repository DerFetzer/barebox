// SPDX-License-Identifier: GPL-2.0-only

#include <common.h>
#include <init.h>
#include <asm/mmu.h>
#include <asm/cache.h>
#include <asm/system_info.h>

#include "mmu_32.h"

struct cache_fns {
	void (*dma_clean_range)(unsigned long start, unsigned long end);
	void (*dma_flush_range)(unsigned long start, unsigned long end);
	void (*dma_inv_range)(unsigned long start, unsigned long end);
	void (*mmu_cache_on)(void);
	void (*mmu_cache_off)(void);
	void (*mmu_cache_flush)(void);
};

#define DEFINE_CPU_FNS(arch) \
	void arch##_dma_clean_range(unsigned long start, unsigned long end);	\
	void arch##_dma_flush_range(unsigned long start, unsigned long end);	\
	void arch##_dma_inv_range(unsigned long start, unsigned long end);	\
	void arch##_mmu_cache_on(void);						\
	void arch##_mmu_cache_off(void);					\
	void arch##_mmu_cache_flush(void);					\
										\
	static struct cache_fns __maybe_unused cache_fns_arm##arch = {		\
		.dma_clean_range = arch##_dma_clean_range,			\
		.dma_flush_range = arch##_dma_flush_range,			\
		.dma_inv_range = arch##_dma_inv_range,				\
		.mmu_cache_on = arch##_mmu_cache_on,				\
		.mmu_cache_off = arch##_mmu_cache_off,				\
		.mmu_cache_flush = arch##_mmu_cache_flush,			\
	};

DEFINE_CPU_FNS(v4)
DEFINE_CPU_FNS(v5)
DEFINE_CPU_FNS(v6)
DEFINE_CPU_FNS(v7)

static struct cache_fns *cache_functions(void)
{
	static struct cache_fns *cache_fns;

	if (cache_fns)
		return cache_fns;

	switch (cpu_architecture()) {
#ifdef CONFIG_CPU_32v4T
	case CPU_ARCH_ARMv4T:
		cache_fns = &cache_fns_armv4;
		break;
#endif
#ifdef CONFIG_CPU_32v5
	case CPU_ARCH_ARMv5:
	case CPU_ARCH_ARMv5T:
	case CPU_ARCH_ARMv5TE:
	case CPU_ARCH_ARMv5TEJ:
		cache_fns = &cache_fns_armv5;
		break;
#endif
#ifdef CONFIG_CPU_32v6
	case CPU_ARCH_ARMv6:
		cache_fns = &cache_fns_armv6;
		break;
#endif
#ifdef CONFIG_CPU_32v7
	case CPU_ARCH_ARMv7:
		cache_fns = &cache_fns_armv7;
		break;
#endif
	default:
		while(1);
	}

	return cache_fns;
}

void __dma_clean_range(unsigned long start, unsigned long end)
{
	cache_functions()->dma_clean_range(start, end);
}

void __dma_flush_range(unsigned long start, unsigned long end)
{
	cache_functions()->dma_flush_range(start, end);
}

void __dma_inv_range(unsigned long start, unsigned long end)
{
	cache_functions()->dma_inv_range(start, end);
}

#ifdef CONFIG_MMU

void __mmu_cache_on(void)
{
	cache_functions()->mmu_cache_on();
}

void __mmu_cache_off(void)
{
	cache_functions()->mmu_cache_off();
}

void __mmu_cache_flush(void)
{
	cache_functions()->mmu_cache_flush();
	if (outer_cache.flush_all)
		outer_cache.flush_all();
}

#endif

/*
 * Early function to flush the caches. This is for use when the
 * C environment is not yet fully initialized.
 */
void arm_early_mmu_cache_flush(void)
{
	switch (arm_early_get_cpu_architecture()) {
#ifdef CONFIG_CPU_32v4T
	case CPU_ARCH_ARMv4T:
		v4_mmu_cache_flush();
		return;
#endif
#ifdef CONFIG_CPU_32v5
	case CPU_ARCH_ARMv5:
	case CPU_ARCH_ARMv5T:
	case CPU_ARCH_ARMv5TE:
	case CPU_ARCH_ARMv5TEJ:
		v5_mmu_cache_flush();
		return;
#endif
#ifdef CONFIG_CPU_32v6
	case CPU_ARCH_ARMv6:
		v6_mmu_cache_flush();
		return;
#endif
#ifdef CONFIG_CPU_32v7
	case CPU_ARCH_ARMv7:
		v7_mmu_cache_flush();
		return;
#endif
	}
}

void v7_mmu_cache_invalidate(void);

void arm_early_mmu_cache_invalidate(void)
{
	switch (arm_early_get_cpu_architecture()) {
#if __LINUX_ARM_ARCH__ <= 7
	case CPU_ARCH_ARMv4T:
	case CPU_ARCH_ARMv5:
	case CPU_ARCH_ARMv5T:
	case CPU_ARCH_ARMv5TE:
	case CPU_ARCH_ARMv5TEJ:
	case CPU_ARCH_ARMv6:
		asm volatile("mcr p15, 0, %0, c7, c6, 0\n" : : "r"(0));
		return;
#ifdef CONFIG_CPU_32v7
	case CPU_ARCH_ARMv7:
		v7_mmu_cache_invalidate();
		return;
#endif
#endif
	}
}
