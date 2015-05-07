#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h> 
#include <linux/errno.h> 
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h> 

#include "gpio.h"

#define R_PIN 22
#define G_PIN 10
#define B_PIN 9

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matti Raudasoja");
MODULE_DESCRIPTION("Simple RGB led driver");

static int rgb_major = 60; // Major number

void rgb_exit(void);
int rgb_init(void);
int rgb_open(struct inode *inode, struct file *filp);
int rgb_release(struct inode *inode, struct file *filp);
ssize_t rgb_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

struct file_operations rgb_fops = {
    write : rgb_write,
    open : rgb_open,
};

int rgb_init(void) {
    int result = register_chrdev(rgb_major, "rgb", &rgb_fops);
    
    if (result < 0) {
        printk("[RGB] cannot obtain major number %d\n", rgb_major);
        return result;
    }

    printk("[RGB] Inserting rgb module\n");
    return 0;
}

void rgb_exit(void) {
    unregister_chrdev(rgb_major, "rgb");
    printk("[RGB] Removing module\n");
}

int rgb_open(struct inode *inode, struct file *filp) {
    printk("[RGB] Device file opened\n");
    return 0;
}

ssize_t rgb_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
    return 0;
}

module_init(rgb_init);
module_exit(rgb_exit);