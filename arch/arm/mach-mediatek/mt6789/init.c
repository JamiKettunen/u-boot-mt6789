// SPDX-License-Identifier: GPL-2.0

#include <fdtdec.h>
#include <init.h>
#include <asm/armv8/mmu.h>
#include <asm/system.h>
#include <asm/global_data.h>
#include <linux/sizes.h>

DECLARE_GLOBAL_DATA_PTR;

int print_cpuinfo(void)
{
	printk("CPU:   MediaTek Helio G99 (MT6789)\n");
	return 0;
}

int dram_init(void)
{
	int ret;

	ret = fdtdec_setup_mem_size_base();
	if (ret)
		return ret;

	// Fix ram_size to 2GB, above that things start to break
	gd->ram_size = get_ram_size((void *)gd->ram_base, SZ_2G);

	return 0;
}

void reset_cpu(void)
{
	psci_system_reset();
}

static struct mm_region vicky_mem_map[] = {
	{
		/* Peripheral region */
		.virt = 0x00000000UL,
		.phys = 0x00000000UL,
		.size = 0x40000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
		PTE_BLOCK_NON_SHARE |
		PTE_BLOCK_PXN | PTE_BLOCK_UXN,
	},
	{
		/* DRAM region 0 */
		.virt = 0x040000000UL,
		.phys = 0x040000000UL,
		.size = 0x026f00000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_OUTER_SHARE
	},
	{
		/* TEE Reserved 0 */
		.virt = 0x70000000UL,
		.phys = 0x70000000UL,
		.size = 0x04600000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
		PTE_BLOCK_NON_SHARE |
		PTE_BLOCK_PXN | PTE_BLOCK_UXN,
	},
	{
		/* DRAM region 1 */
		.virt = 0x70000000UL,
		.phys = 0x70000000UL,
		.size = 0x08000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_OUTER_SHARE
	},
	{
		/* TEE Reserved 1 */
		.virt = 0x7de00000UL,
		.phys = 0x7de00000UL,
		.size = 0x00800000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
		PTE_BLOCK_NON_SHARE |
		PTE_BLOCK_PXN | PTE_BLOCK_UXN,
	},
	{
		/* DRAM region 2 */
		.virt = 0x7ffff000UL,
		.phys = 0x7ffff000UL,
		.size = 0x7e0d1000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_OUTER_SHARE
	},
	{
		/* Framebuffer Reserved */
		.virt = 0xfe0d0000UL,
		.phys = 0xfe0d0000UL,
		.size = 0x01df0000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL_NC) | PTE_BLOCK_INNER_SHARE |PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},
	{
		/* DRAM region 3 */
		.virt = 0xfecd0000UL,
		.phys = 0xfecd0000UL,
		.size = 0x4132f000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_OUTER_SHARE
	},
	{
		/* DRAM Controller 0 Reserved */
		.virt = 0x03ffff000UL,
		.phys = 0x03ffff000UL,
		.size = 0x000001000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
		PTE_BLOCK_NON_SHARE |
		PTE_BLOCK_PXN | PTE_BLOCK_UXN,
	},
	{
		/* DRAM region 4 */
		.virt = 0x140000000UL,
		.phys = 0x140000000UL,
		.size = 0x0c0000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_OUTER_SHARE
	},
	{
		0,
	}
};

struct mm_region *mem_map = vicky_mem_map;
