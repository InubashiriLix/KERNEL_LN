#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/hrtimer.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/module.h>

// META SIGNS
MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("timer test");

static struct gpio_desc *led_pwm, *led_test;

#define PWM_FREQ 1000
int pwm_duty = 50;
#define PWM_DUTY_MAX 100
u64 period = 1000000000 / PWM_FREQ;
static ktime_t high_time, low_time, step_time;

uint8_t led_pwm_state, increasing;

#define GPIO_PWM_LED 135
#define GPIO_LED_TEST 134
#define TIMER_BREATH_HELPER_INTERVEL 50
#define TIMER_BREATH_HELPRE_STEP 10

// hr timer
static struct hrtimer pwm_timer, pwm_breath_timer;
u64 start_t;

static enum hrtimer_restart pwm_timer_handler(struct hrtimer *timer) {
    u64 now_t = ktime_get();
    led_pwm_state = !led_pwm_state;
    gpiod_set_value(led_pwm, led_pwm_state);
    // timer set
    hrtimer_forward_now(timer, led_pwm_state ? high_time : low_time);
    return HRTIMER_RESTART;
}

static enum hrtimer_restart pwm_breath_timer_handler(struct hrtimer *timer) {
    /* printk(KERN_INFO "breath timer handler"); */
    u64 now_t = ktime_get();
    if (increasing) {
        pwm_duty += TIMER_BREATH_HELPRE_STEP;
        if (pwm_duty >= PWM_DUTY_MAX) {
            pwm_duty = PWM_DUTY_MAX;
            increasing = 0;
        }
    } else {
        pwm_duty -= TIMER_BREATH_HELPRE_STEP;
        if (pwm_duty <= TIMER_BREATH_HELPRE_STEP) {
            pwm_duty = 0;
            increasing = 1;
        }
    }
    high_time = ns_to_ktime(period * pwm_duty / 100);
    low_time = ns_to_ktime(period - ktime_to_ns(high_time));

    hrtimer_forward_now(timer, step_time);
    return HRTIMER_RESTART;
}

static int __init hire_timer_module_init(void) {
    printk(KERN_INFO "Hello, Kernel\n");

    printk(KERN_INFO "initializing led_pwm");
    led_pwm = gpio_to_desc(GPIO_PWM_LED);
    if (!led_pwm) {
        printk(KERN_CRIT "gpio led_pwm id: %d, failed", GPIO_PWM_LED);
        return -ENODEV;
    }
    printk(KERN_INFO "setting led_pwm output value 0");
    int status_pwm_led = gpiod_direction_output(led_pwm, 0);
    if (status_pwm_led) {
        printk(KERN_CRIT "gpio led_pwm: set direction output 1 failed");
        return -1;
    }
    gpiod_set_value(led_pwm, 0);

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

    // initialzie the pwm
    high_time = ns_to_ktime(period * pwm_duty / 100);
    low_time = ns_to_ktime(period - ktime_to_ns(high_time));

    // set up the timer
    printk(KERN_INFO "setting up the timer");
    hrtimer_init(&pwm_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    pwm_timer.function = pwm_timer_handler;
    hrtimer_start(&pwm_timer, high_time, HRTIMER_MODE_REL);

    // set up breathing led timer
    printk(KERN_INFO "setting up the breathing helper timer");
    step_time = ns_to_ktime(TIMER_BREATH_HELPER_INTERVEL * 1000000);
    hrtimer_init(&pwm_breath_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    pwm_breath_timer.function = pwm_breath_timer_handler;
    hrtimer_start(&pwm_breath_timer, step_time, HRTIMER_MODE_REL);

    printk(KERN_INFO "all done");
    return 0;
}

static void __exit hiri_timer_module_exit(void) {
    printk(KERN_INFO "remove the timer");
    hrtimer_cancel(&pwm_timer);
    hrtimer_cancel(&pwm_breath_timer);
    printk(KERN_INFO "set led 0 value");
    gpiod_set_value(led_test, 0);
    gpiod_set_value(led_pwm, 0);
    printk(KERN_INFO "Goodbye, Kernel\n");
}

module_init(hire_timer_module_init);
module_exit(hiri_timer_module_exit);
