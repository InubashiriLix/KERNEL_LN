#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/module.h>

static struct gpio_desc *led, *button;

#define GPIO_LED 135
#define GPIO_BUTTON 134

static int __init my_init(void) {
    int status = 0;
    printk(KERN_INFO "Hello, Kernel\n");
    led = gpio_to_desc(GPIO_LED);
    if (!led) {
        printk("gpioctrl - Error getting gpio 135 ");
        return -ENODEV;
    }
    button = gpio_to_desc(GPIO_BUTTON);
    if (!button) {
        printk("gpioctrl - Error getting gpio 134 ");
        return -ENODEV;
    }

    status = gpiod_direction_output(led, 0);
    if (status) {
        printk("gpioctrl - Error setting gpio 135 output");
    }

    status = gpiod_direction_input(button);
    if (status) {
        printk("gptioctrl - Error setting gpio 134 input");
    }

    gpiod_set_value(led, 1);
    printk("gpio ctrl - Button  is %s pressed\n", gpiod_get_value(button) ? "" : "not ");
    return 0;
}

static void __exit my_exit(void) {
    gpiod_set_value(led, 0);
    printk(KERN_INFO "Goodbye, Kernel\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("simple gpio set example");
