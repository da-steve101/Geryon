#! /bin/bash

emacs ../drivers/buttons.c ../drivers/class-d.c ../../../utils/cmdline.c commands.c data-acq.c \
 ../../../third_party/fatfs/port/dual-disk-driver.c ../../../third_party/fatfs/src/ff.c file.c \
 ../drivers/formike128x128x16.c images.c menu-controls.c menu.c qs-scope.c renderer.c \
startup_gcc.c ../../../utils/uartstdio.c usb_device.c usb_host.c usbdescriptors.c ../../../utils/ustdlib.c \
../../../inc/hw_ints.h ../../../inc/hw_memmap.h ../../../inc/hw_types.h ../../../driverlib/adc.h \
../../../driverlib/debug.h ../../../driverlib/gpio.h ../../../driverlib/interrupt.h ../../../driverlib/pwm.h \
../../../driverlib/rom.h ../../../driverlib/sysctl.h ../../../driverlib/systick.h ../../../driverlib/timer.h \
../../../driverlib/udma.h ../../../grlib/grlib.h ../../../usblib/usblib.h ../../../utils/uartstdio.h \
../../../drivers/buttons.h ../../../drivers/class-d.h commands.h data-acq.h file.h menu.h menu-controls.h \
qs-scope.h renderer.h usb_device.h usb_host.h usb_protocol.h &
