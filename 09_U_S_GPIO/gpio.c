#include <fcntl.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    int fb;
    struct gpiohandle_request led, button;
    struct gpiohandle_data data;
}
