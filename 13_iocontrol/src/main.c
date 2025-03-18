#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("HELL THERE");

static int driver_open(struct inode *device_file, struct file *instance) {
    printk(KERN_INFO "Device file opened\n");
    return 0;
}

static int driver_close() {
    printk(KERN_INFO "Device file closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
};

static int __init my_init(void) {
    printk(KERN_INFO "Hello, Kernel\n");

    return 0;
}

static void __exit my_exit(void) { printk(KERN_INFO "Goodbye, Kernel\n"); }

module_init(my_init);
module_exit(my_exit);
