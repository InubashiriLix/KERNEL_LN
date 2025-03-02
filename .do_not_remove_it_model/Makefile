ifneq ($(KERNELRELEASE),)
obj-m:=hell.o
else 
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.cmd *.mode.c *.order
endif
