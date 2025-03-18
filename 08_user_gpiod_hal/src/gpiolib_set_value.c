#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_LINE 17

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int value;

    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        perror("Open gpio /dev/gpiochip0 failed");
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
        return -1;
    }

    printf("setting gpio value: 1");
    gpiod_line_set_value(line, 1);

    /* value = gpiod_line_get_value(line); */
    /* printf("gpio line value: %d", value); */

    printf("setting gpio value: 0");
    gpiod_line_set_value(line, 0);

    /* value = gpiod_line_get_value(line); */
    /* printf("gpio line value: %d", value); */

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
