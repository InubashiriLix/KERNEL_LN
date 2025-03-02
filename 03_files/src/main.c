#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>

#define GPIO_LED 135
#define GPIO_BUTTON 134

static int major;

// ssize  -> long
// definition of read function
static ssize_t my_read(struct file *file, char __user *u, size_t l, loff_t *o) {
    printk(KERN_INFO "Read from device\n");
    return 0;
}

// define the read function
static struct file_operations fops = {.read = my_read};

static int __init my_init(void) {
    printk(KERN_INFO "Hello, Kernel\n");

    // allocate the device number
    // use 0 to allocate a decice number dynamically
    // the second argument is a label
    // the fops is a pointer to a file_operations struct like read, write, open, close
    major = register_chrdev(0, "hello_cdev", &fops);
    // if the major is not 0,then return 0 if not error
    // if the major is 0 then return the allocated major number
    // if the return value < 0, then an erorr happend

    if (major < 0) {
        printk("hello_cdev: Error registering device number\n");
        return major;
    }

    printk("hello_cdev: Registered device number %d\n", major);
    return 0;
}

static void __exit my_exit(void) {
    unregister_chrdev(major, "hello_cdev");
    printk(KERN_INFO "Goodbye, Kernel\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix < InubashiriLix@gmail.com >");
MODULE_DESCRIPTION("A sampler driver for registering a character device");
