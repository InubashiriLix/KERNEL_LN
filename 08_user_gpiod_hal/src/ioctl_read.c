#include <fcntl.h>
#include <linux/gpio.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    int fd;
    struct gpiochip_info chip_info;

    fd = open("/dev/gpiochip0", O_RDONLY);
    if (fd < 0) {
        perror("open gpio chip failed");
        return -1;
    }

    if (ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &chip_info) < 0) {
        perror("ioctl get chip info failed");
        close(fd);
        return -1;
    }

    printf("GPIO Chip: %s, %s, Lines: %d\n", chip_info.name, chip_info.label, chip_info.lines);
    close(fd);
    return 0;
}
