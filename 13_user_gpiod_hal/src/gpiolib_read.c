#include <gpiod.h>
#include <stdio.h>

#define GPIO_LINE 6
#define GPIO_CHIP "/dev/gpiochip4"

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int value;

    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("Open gpiochip failed");
        return -1;
    }

    line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (!line) {
        perror("Get line failed");
        gpiod_chip_close(chip);
        return -1;
    }

    if (gpiod_line_request_input(line, "gpio-example") < 0) {
        perror("failed to request line");
        return -1;
    }

    value = gpiod_line_get_value(line);
    printf("gpio line value: %d", value);

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
