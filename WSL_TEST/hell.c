#include <linux/init.h>
#include <linux/module.h>

static int __init my_init(void) {
    printk("hello - Hello, Kernel\n");
    return 0;
}

static void __exit my_exit(void) {
    printk("goodbye - Leaving Kernel\n");
    return;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
