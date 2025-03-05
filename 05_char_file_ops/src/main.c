#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>

static int major = 0;

static int f_open(struct inode* mynode, struct file* filep) {
    printk(KERN_INFO "open the char file");
    printk(KERN_INFO "major of file: %ud, minor: %ud", imajor(mynode), iminor(mynode));

    printk(KERN_INFO "file mode: %x", filep->f_mode);
    printk(KERN_INFO "file pos: %lld", filep->f_pos);
    printk(KERN_INFO "file path: %ud", filep->f_flags);
    return 0;
}

static int f_release(struct inode* mynode, struct file* filep) {
    printk(KERN_INFO "close the char file");
    return 0;
}
static struct file_operations fops = {.open = f_open, .release = f_release};

static int __init my_init(void) {
    printk(KERN_INFO "Hello, Kernel\n");
    printk(KERN_INFO "allocate the char file");
    // use 0 to allocate the device automatically
    int major = register_chrdev(0, "test_char", &fops);
    if (major < 0) {
        printk(KERN_ERR "load the char file failed");
        return major;
    }
    printk(KERN_INFO "device major %d", major);

    return 0;
}

static void __exit my_exit(void) { printk(KERN_INFO "Goodbye, Kernel\n"); }

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("HELL THERE");
