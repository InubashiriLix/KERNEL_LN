
ifneq ($(KERNELRELEASE),)
obj-m := 03_files.o
03_files-objs := src/main.o
else
    KDIR := /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)
    SRC_DIR := $(PWD)/src
    OBJ_DIR := $(PWD)/obj
    MODULE := $(PWD)/03_files.ko

all:
	@echo "Building 03_files module..."
	mkdir -p $(OBJ_DIR)
	make -C $(KDIR) M=$(PWD) modules

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -I$(PWD)/include -c $< -o $(OBJ_DIR)/$@

clean:
	rm -rf $(OBJ_DIR)/*.o $(MODULE) *.mod.* *.symvers *.cmd *.mode.c *.order
endif
