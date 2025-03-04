ifneq ($(KERNELRELEASE),)
obj-m:=hell.o
else 
	KDIR := /lib/modules/$(shell uname -r)/build
	# KDIR := /home/inubashiri/.kernel/WSL2-Linux-Kernel-linux-msft-wsl-5.15.167.4/build
	PWD := $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.cmd *.mode.c *.order
endif
