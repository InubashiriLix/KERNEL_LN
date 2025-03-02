import os
import logging

logging.basicConfig(level=logging.DEBUG)


def create_project_struct(module_name):
    logging.info(f"Creating module: {module_name}")
    dirs = [
        f"{module_name}/src",
        f"{module_name}/include",
    ]

    for dir in dirs:
        os.makedirs(dir, exist_ok=True)

    create_makefile(module_name)
    create_main_file(module_name)
    create_clangd_config(module_name)


def create_makefile(module_name):
    logging.info(f"Creating makefile for module: {module_name}")
    makefile = f"""
ifneq ($(KERNELRELEASE),)
obj-m := {module_name}.o
{module_name}-objs := src/main.o
else
    KDIR := /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)
    SRC_DIR := $(PWD)/src
    OBJ_DIR := $(PWD)/obj
    MODULE := $(PWD)/{module_name}.ko

all:
	@echo "Building {module_name} module..."
	mkdir -p $(OBJ_DIR)
	make -C $(KDIR) M=$(PWD) modules

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -I$(PWD)/include -c $< -o $(OBJ_DIR)/$@

clean:
	rm -rf $(OBJ_DIR)/*.o $(MODULE) *.mod.* *.symvers *.cmd *.mode.c *.order
endif
"""
    with open(f"{module_name}/Makefile", "w") as f:
        f.write(makefile)


def create_main_file(module_name):
    logging.info("Creating main file for module: main")
    main_content = """
#include <linux/init.h>
#include <linux/module.h>

static int __init my_init(void) {
\tprintk(KERN_INFO "Hello, Kernel\\n");
\treturn 0;
}

static void __exit my_exit(void) {
\tprintk(KERN_INFO "Goodbye, Kernel\\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("HELL THERE");
"""
    with open(f"{module_name}/src/main.c", "w") as f:
        f.write(main_content)


def create_clangd_config(module_name):
    logging.info("Creating clangd config file")
    clangd_content = """
CompileFlags:
  Add:
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/include/"
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/include/asm-generic/"
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/arch/arm/include/"
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/arch/arm/include/asm/"
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/arch/arm64/include/"
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/arch/arm64/include/asm/"
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/arch/arm64/include/generated/asm/"
    - "-I/usr/src/linux-headers-6.1.43-rockchip-rk3588/"
    - "-D__KERNEL__"
    - "-DMODULE"
"""

    with open(f"{module_name}/.clangd", "w") as f:
        f.write(clangd_content)


if __name__ == "__main__":
    enter_module_name = input("Enter the module name: \n")
    confirm = ""

    if os.path.exists(enter_module_name):
        logging.warning(f"THE MODULE DIR ALREADY EXISTS")
        while confirm not in ("y", "n"):
            confirm = input("Do you want to overwrite the existing folder? (y/n): ")

    if confirm == "n":
        logging.info("Exiting")
        exit(0)

    create_project_struct(enter_module_name)
    exit(0)
