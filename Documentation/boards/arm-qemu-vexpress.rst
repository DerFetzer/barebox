QEMU vexpress
=============

ARM Qemu vexpress
-----------------

Running barebox on QEMU vexpress machine
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Images for the vexpress platform are built as part of the
multi_v7_defconfig.

Usage::

	$ qemu-system-arm -m 1024M \
		-machine vexpress-a9 -cpu cortex-a9 \
		-nographic -no-reboot \
		-kernel images/barebox-vexpress-ca9.img

	$ qemu-system-arm -m 1024M \
		-machine vexpress-a15 -cpu cortex-a15 \
		-nographic -no-reboot \
		-kernel images/barebox-vexpress-ca15.img
