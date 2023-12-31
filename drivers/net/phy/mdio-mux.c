// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2017 Zodiac Inflight Innovation
 * Author: Andrey Smirnov <andrew.smirnov@gmail.com>
 *
 * Based on analogous code from Linux kernel
 *
 * Copyright (C) 2011, 2012 Cavium, Inc.
 */
#include <common.h>
#include <linux/mdio-mux.h>
#include <linux/phy.h>

struct mdio_mux_parent_bus {
	struct mii_bus *mii_bus;
	int current_child;
	int parent_id;
	void *switch_data;
	int (*switch_fn)(int current_child, int desired_child, void *data);
};

struct mdio_mux_child_bus {
	struct mii_bus mii_bus;
	struct mdio_mux_parent_bus *parent;
	struct mdio_mux_child_bus *next;
	int bus_number;
};

static int mdio_mux_read_or_write(struct mii_bus *bus, int phy_id,
		       int regnum, u16 *val)
{
	struct mdio_mux_child_bus *cb = bus->priv;
	struct mdio_mux_parent_bus *pb = cb->parent;
	int r;

	r = pb->switch_fn(pb->current_child, cb->bus_number, pb->switch_data);
	if (!r) {
		pb->current_child = cb->bus_number;
		if (val)
			r = mdiobus_write (pb->mii_bus, phy_id,
					       regnum, *val);
		else
			r = mdiobus_read(pb->mii_bus, phy_id, regnum);
	}
	return r;
}

static int mdio_mux_read(struct mii_bus *bus, int phy_id, int regnum)
{
	return mdio_mux_read_or_write(bus, phy_id, regnum, NULL);
}

static int mdio_mux_write(struct mii_bus *bus, int phy_id,
			  int regnum, u16 val)
{
	return mdio_mux_read_or_write(bus, phy_id, regnum, &val);
}

int mdio_mux_init(struct device *dev,
		  struct device_node *mux_node,
		  int (*switch_fn)(int cur, int desired, void *data),
		  void *data,
		  struct mii_bus *mux_bus)
{
	static int parent_count = 0;

	struct device_node *parent_bus_node;
	struct device_node *child_bus_node;
	struct mdio_mux_parent_bus *pb;
	struct mdio_mux_child_bus *cb;
	struct mii_bus *parent_bus;
	int r;

	if (!mux_node)
		return -ENODEV;

	if (!mux_bus) {
		parent_bus_node = of_parse_phandle(mux_node,
						   "mdio-parent-bus", 0);

		if (!parent_bus_node)
			return -ENODEV;

		parent_bus = of_mdio_find_bus(parent_bus_node);

		if (!parent_bus)
			return -EPROBE_DEFER;
	} else {
		parent_bus_node = NULL;
		parent_bus = mux_bus;
	}

	pb = xzalloc(sizeof(*pb));

	pb->switch_data   = data;
	pb->switch_fn     = switch_fn;
	pb->current_child = -1;
	pb->parent_id     = parent_count++;
	pb->mii_bus       = parent_bus;

	for_each_available_child_of_node(mux_node, child_bus_node) {
		int v;

		r = of_property_read_u32(child_bus_node, "reg", &v);
		if (r) {
			dev_err(dev,
				"Error: Failed to find reg for child %pOF\n",
				child_bus_node);
			continue;
		}

		cb = xzalloc(sizeof(*cb));
		cb->bus_number = v;
		cb->parent     = pb;

		cb->mii_bus.priv   = cb;
		cb->mii_bus.parent = dev;
		cb->mii_bus.read   = mdio_mux_read;
		cb->mii_bus.write  = mdio_mux_write;
		cb->mii_bus.dev.of_node = child_bus_node;

		r = mdiobus_register(&cb->mii_bus);
		if (r) {
			dev_err(dev,
				"Error: Failed to register MDIO bus for child %pOF\n",
				child_bus_node);
		}
	}

	parent_bus->is_multiplexed = true;
	return 0;
}
EXPORT_SYMBOL_GPL(mdio_mux_init);
