
ifneq ($(KERNELRELEASE),)
obj-m += pwm.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
endif
load:
	- sudo rmmod -f pwm
	sudo insmod pwm.ko
