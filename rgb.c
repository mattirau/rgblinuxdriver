#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/fs.h> 
#include <linux/errno.h> 
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h> 

/*
 
 Based on:
 https://github.com/wendlers/rpi-kmod-samples/blob/master/modules/kmod-gpio_output/gpiomod_output.c
 
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matti Raudasoja");
MODULE_DESCRIPTION("Simple RGB led driver");

static struct gpio leds[] = {
    { 22, GPIOF_OUT_INIT_HIGH, "LED R"},
    { 10, GPIOF_OUT_INIT_HIGH, "LED G"},
    { 9, GPIOF_OUT_INIT_HIGH, "LED B"}
};

int led_status[ARRAY_SIZE(leds)] = {0};

static int rgb_major = 60; // Major number

void setup_io(void);
void rgb_exit(void);
int rgb_init(void);
int rgb_open(struct inode *inode, struct file * filp);
int rgb_release(struct inode *inode, struct file * filp);
ssize_t rgb_write(struct file *filp, const char *buf, size_t count, loff_t * f_pos);

struct file_operations rgb_fops = {
    write : rgb_write,
    open : rgb_open,
};

int rgb_init(void) {
    int ret = 0;
    int i;

    ret = register_chrdev(rgb_major, "rgb", &rgb_fops);

    if (ret < 0) {
        printk("[RGB] cannot obtain major number %d\n", rgb_major);
        return ret;
    }

    // register LED GPIOs, turn LEDs on
    ret = gpio_request_array(leds, ARRAY_SIZE(leds));

    if (ret) {
        printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
    }

    // turn all LEDs off
    for (i = 0; i < ARRAY_SIZE(leds); i++) {
        gpio_set_value(leds[i].gpio, 0);
    }

    printk("[RGB] Inserting rgb module\n");
    return 0;
}

void rgb_exit(void) {
    int i;
    unregister_chrdev(rgb_major, "rgb");

    // turn all LEDs off
    for (i = 0; i < ARRAY_SIZE(leds); i++) {
        gpio_set_value(leds[i].gpio, 1);
    }

    // unregister all GPIOs
    gpio_free_array(leds, ARRAY_SIZE(leds));

    printk("[RGB] Removing module\n");
}

int rgb_open(struct inode *inode, struct file * filp) {
    int i;

    // turn all LEDs off
    for (i = 0; i < ARRAY_SIZE(leds); i++) {
        gpio_set_value(leds[i].gpio, 0);
    }

    printk("[RGB] Device file opened\n");
    return 0;
}

ssize_t rgb_write(struct file *filp, const char *buf, size_t count, loff_t * f_pos) {
    int led = 0;
    int valid = 1;

    printk("[RGB] Data received '%s'\n", buf);

    switch (buf[0]) {
        case 'r':
        case '1':
            led = 0;
            break;
        case 'g':
        case '2':
            led = 1;
            break;
        case 'b':
        case '3':
            led = 2;
            break;
        default:
            valid = 0;
            break;
    }

    if (valid) {
        printk("[RGB] Led %d set %s\n", led, (led_status[led]) ? "On" : "Off");

        led_status[led] = !led_status[led];
        gpio_set_value(leds[led].gpio, led_status[led]);
    } else {
        printk("[RGB] Data invalid\n");
    }

    return 0;
}

module_init(rgb_init);
module_exit(rgb_exit);