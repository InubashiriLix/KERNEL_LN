#include <gpiod.h>
#include <stdio.h>
#include <time.h>

void my_sleep(int seconds) {
    struct timespec ts;
    ts.tv_sec = seconds;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);
}

#define GPIO_LINE 6  // gpio 134, gpiochip4

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int value;

    chip = gpiod_chip_open("/dev/gpiochip4");
    if (!chip) {
        perror("Open gpio /dev/gpiochip4 failed");
        return -1;
    }

    line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (!line) {
        perror("Get line failed");
        gpiod_chip_close(chip);
        return -1;
    }

    /* if (!gpiod_line_request_input(line, "gpio-example")) { */
    /*     perror("failed to request line"); */
    /*     return -1; */
    /* } */

    if (gpiod_line_request_output(line, "gpio-example", 0) < 0) {
        perror("failed to request line");
        gpiod_chip_close(chip);
        return -1;
    }

    printf("setting gpio value: 1\n");
    gpiod_line_set_value(line, 1);
    my_sleep(1);

    /* value = gpiod_line_get_value(line); */
    /* printf("gpio line value: %d", value); */

    printf("setting gpio value: 0\n");
    gpiod_line_set_value(line, 0);
    my_sleep(1);

    /* value = gpiod_line_get_value(line); */
    /* printf("gpio line value: %d", value); */

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
