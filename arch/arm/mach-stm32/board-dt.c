// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) Maxime Coquelin 2015
 * Copyright (C) STMicroelectronics 2017
 * Author:  Maxime Coquelin <mcoquelin.stm32@gmail.com>
 */

#include <linux/kernel.h>
#include <asm/mach/arch.h>
#ifdef CONFIG_ARM_SINGLE_ARMV7M
#include <asm/v7m.h>
#endif
#include <linux/phy.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_net.h>

static const char *const stm32_compat[] __initconst = {
	"st,stm32f429",
	"st,stm32f469",
	"st,stm32f746",
	"st,stm32f769",
	"st,stm32h743",
	"st,stm32h750",
	"st,stm32mp131",
	"st,stm32mp133",
	"st,stm32mp135",
	"st,stm32mp151",
	"st,stm32mp153",
	"st,stm32mp157",
	NULL
};

#define PHY_ID_AR8031   0x004dd074
static int ar8031_phy_fixup(struct phy_device *dev)
{
	u16 val;

	if (of_machine_is_compatible("emcraft,stm32mp1-som")) {
		/* enable rgmii rxc skew and phy mode select to RGMII copper */
		phy_write(dev, 0x1d, 0x1f);
		phy_write(dev, 0x1e, 0x8);

		phy_write(dev, 0x1d, 0x05);
		phy_write(dev, 0x1e, 0x100);

		/* Make the phy generate 125MHz clock on CLK_25M output clock */
		phy_write(dev, 0xd, 0x0007);
		phy_write(dev, 0xe, 0x8016);
		phy_write(dev, 0xd, 0x4007);
		val = phy_read(dev, 0xe);
		phy_write(dev, 0xe, val | 0x18);
	}
	return 0;
}


static void __init stm32_init_machine(void)
{
	phy_register_fixup_for_uid(PHY_ID_AR8031, 0xffffffff,
				   ar8031_phy_fixup);
}

DT_MACHINE_START(STM32DT, "STM32 (Device Tree Support)")
	.dt_compat = stm32_compat,
#ifdef CONFIG_ARM_SINGLE_ARMV7M
	.restart = armv7m_restart,
#endif
	.init_machine	= stm32_init_machine,
MACHINE_END
