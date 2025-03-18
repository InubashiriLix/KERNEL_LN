#include <fcntl.h>
#include <linux/gpio.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define GPIO_LINE 17

int main() {
    int fd;
    struct gpiochip_info chip_info;
    struct gpiohandle_request gpio_request;
    struct gpiohandle_data gpio_data;

    fd = open("/dev/gpiochip0", O_RDONLY);
    if (fd < 0) {
        perror("open gpio chip failed");
        return -1;
    }

    gpio_request.lineoffsets[0] = GPIO_LINE;
    gpio_request.flags = GPIOHANDLE_REQUEST_OUTPUT;
    gpio_request.lines = 1;
    gpio_request.default_values[0] = 0;
    if (ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &gpio_request) < 0) {
        perror("ioctl get line handle failed");
        close(fd);
        return -1;
    }

    gpio_data.values[0] = 1;
    if (ioctl(gpio_request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &gpio_data) < 0) {
        perror("Failed to set GPIO high");
    }
    printf("GPIO %d set HIGH\n", GPIO_LINE);
    sleep(1);

    gpio_data.values[0] = 0;
    if (ioctl(gpio_request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &gpio_data) < 0) {
        perror("Failed to set GPIO high");
    }
    printf("GPIO %d set LOW\n", GPIO_LINE);
    sleep(1);

    printf("GPIO Chip: %s, %s, Lines: %d\n", chip_info.name, chip_info.label, chip_info.lines);
    close(gpio_request.fd);
    close(fd);
    return 0;
}
