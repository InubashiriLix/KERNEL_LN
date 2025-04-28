#include <linux/init.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("InubashiriLix InubashiriLix@gmail.com");
MODULE_DESCRIPTION("SPI DEV USING BMP280");

#define BUS_NUM 0  // we use the spi 0 on the board

static struct spi_device *bmp280_spi;
static struct spi_master *master;
// load the spi info of bmp280
static struct spi_board_info bmp280_spi_info = {
    .modalias = "bmp280",
    .max_speed_hz = 1000000,
    .bus_num = BUS_NUM,
    .chip_select = 0,
    .mode = 3,
};

/**
 * @breif: init the spi device
 * @param[in] void
 * @param[out] int
 */
static int __init my_init(void) {
    printk(KERN_INFO "Hello, Kernel\n");

    printk(KERN_INFO "get access to the spi device");
    master = spi_busnum_to_master(BUS_NUM);
    if (!master) {
        printk(KERN_CRIT "there is no spi bus with Nr. %d\n", BUS_NUM);
        return -1;
    }

    bmp280_spi = spi_new_device(master, &bmp280_spi_info);
    if (!bmp280_spi) {
        printk(KERN_CRIT "the bmp280 spi device init failed");
        return -1;
    }

    bmp280_spi->bits_per_word = 8;

    // setup the bus for device parameter
    if (spi_setup(bmp280_spi) != 0) {
        printk(KERN_CRIT "Could not change bus setup");
        return -1;
    }

    u8 id = spi_w8r8(bmp280_spi, 0xD0);
    printk(KERN_INFO "ID: 0x%x\n", id);

    u8 val[2] = {0x75, 0x40};
    spi_write(bmp280_spi, &val, 2);
    id = spi_w8r8(bmp280_spi, 0xF5);
    printk(KERN_INFO "config reg. value: 0x%x\n", id);

    return 0;
}

static void __exit my_exit(void) {
    printk(KERN_INFO "unregister the device");
    if (bmp280_spi) {
        spi_unregister_device(bmp280_spi);
    }

    printk(KERN_INFO "Goodbye, Kernel\n");
}

module_init(my_init);
module_exit(my_exit);
