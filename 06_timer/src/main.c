#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct gpio_desc *led_pwn, *led_test;
static struct timer_list pwm_timer;

uint8_t led_pwm_value = 0;

#define GPIO_PWM_LED 135
#define GPIO_LED_TEST 134

#define TIMER_INTERVAL 100

// META SIGNS
MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("timer test");

void timer_callback(struct timer_list* data) {
    led_pwm_value = (led_pwm_value + 1) % 2;
    printk(KERN_INFO "timer callback: curr led value: %d", led_pwm_value);
    gpiod_set_value(led_pwn, led_pwm_value);
    mod_timer(&pwm_timer, jiffies + msecs_to_jiffies(TIMER_INTERVAL));
}

static int __init my_init(void) {
    printk(KERN_INFO "Hello, Kernel\n");

    printk(KERN_INFO "initializing led_pwm");
    led_pwn = gpio_to_desc(GPIO_PWM_LED);
    if (!led_pwn) {
        printk(KERN_CRIT "gpio led_pwn id: %d, failed", GPIO_PWM_LED);
        return -ENODEV;
    }
    printk(KERN_INFO "setting led_pwm output value 0");
    int status_pwm_led = gpiod_direction_output(led_pwn, 0);
    if (status_pwm_led) {
        printk(KERN_CRIT "gpio led_pwm: set direction output 1 failed");
        return -1;
    }
    gpiod_set_value(led_pwn, 0);

    printk(KERN_INFO "setting led_test");
    led_test = gpio_to_desc(GPIO_LED_TEST);
    if (!led_test) {
        printk(KERN_CRIT "gpio led_test id: %d, failed", GPIO_LED_TEST);
        return -ENODEV;
    }
    printk(KERN_INFO "setting led_test output mode with value 1");
    int status = gpiod_direction_output(led_test, 1);
    if (status) {
        printk(KERN_CRIT "gpio led_test: set direction output 1 failed");
        return -1;
    }
    gpiod_set_value(led_test, 1);

    // set up the timer
    printk(KERN_INFO "setting up timer");
    timer_setup(&pwm_timer, timer_callback, 0);
    mod_timer(&pwm_timer, jiffies + msecs_to_jiffies(TIMER_INTERVAL));

    printk(KERN_INFO "all done");
    return 0;
}

static void __exit my_exit(void) {
    printk(KERN_INFO "remove the timer");
    del_timer_sync(&pwm_timer);
    printk(KERN_INFO "Goodbye, Kernel\n");
}

module_init(my_init);
module_exit(my_exit);
