#include <linux/init.h>
#include <linux/module.h>

static int __init my_init(void) {
    // tset for kernel log levels
    printk(KERN_INFO "kernel info test\n");
    printk(KERN_WARNING "kernel wargning test\n");
    printk(KERN_CRIT "kernel crit test\n");
    printk(KERN_EMERG "kernel emerg test\n");
    printk(KERN_DEBUG "kernel debug test\n");
    printk(KERN_DEFAULT "kernel debug test\n");
    printk(KERN_INFO "Hello, Kernel\n");
    return 0;
}

static void __exit my_exit(void) {
    // kernel log functin test

    printk(KERN_INFO "Goodbye, Kernel\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("HELL THERE");
