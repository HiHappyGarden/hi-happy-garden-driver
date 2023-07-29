/*
 * This file is part of the Happy GardenPI distribution (https://github.com/HappyGardenPI/happy-gardenpi-driver).
 * Copyright (c) 2022-23 Antonio Salsi.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h> //copy_to/from_user()

#include "constants.h"
#include "error.h"
#include "parser.h"

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) HHGD_DRIVER_NAME ": " fmt
#endif

#define READ_BUFF_LEN (256)
#define NOT_DEF (3)


bool hhgd_led = 0;
bool hhgd_relay_1 = 0;
bool hhgd_relay_2 = 0;
bool hhgd_relay_3 = 0;
bool hhgd_relay_4 = 0;
bool hhgd_button = 0;
bool hhgd_lcd = 0;



//MODULE

// data
static dev_t hhgd_dev = 0;
static struct class *hhgd_class = NULL;
static struct cdev hhgd_cdev;
static atomic_t device_busy = ATOMIC_INIT(0);

// static decl
static int __init hhgd_driver_init(void);
static void __exit hhgd_driver_exit(void);

// static decl
static int hhgd_ioctl_open(struct inode *inode, struct file *file);
static int hhgd_ioctl_release(struct inode *inode, struct file *file);
static ssize_t hhgd_ioctl_read(struct file *filp, char __user *buff, size_t len, loff_t *off);
static ssize_t hhgd_ioctl_write(struct file *filp, const char *buff, size_t len, loff_t *off);
static long hhgd_ioctl( struct file *p_file, unsigned int ioctl_command, unsigned long arg);

static int hhgd_uevent(struct device *dev, struct kobj_uevent_env *env);

// File operation structure
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = hhgd_ioctl_read,
    .write = hhgd_ioctl_write,
    .open = hhgd_ioctl_open,
    .release = hhgd_ioctl_release,
    .unlocked_ioctl = hhgd_ioctl
};

/*
** This function will be called when we open the Device file
*/
int hhgd_ioctl_open(struct inode *inode, struct file *file)
{
    if (atomic_read(&device_busy) > 0)
    {
        pr_err("device busy");
        return -EBUSY;
    }

    atomic_inc(&device_busy);

    pr_info("Device open:%u\n", atomic_read(&device_busy));
    return 0;
}

/*
** This function will be called when we close the Device file
*/
int hhgd_ioctl_release(struct inode *inode, struct file *file)
{

    // hhgd_button_release();

    atomic_sub(1, &device_busy);

    pr_info("Device release:%u\n", atomic_read(&device_busy));
    return 0;
}

/*
** This function will be called when we read the Device file
*/
ssize_t hhgd_ioctl_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
    u32 msg_len = 0;
    char msg[READ_BUFF_LEN];
    memset(msg, '\0', READ_BUFF_LEN);

    msg_len = snprintf(msg,
                    sizeof(msg), 
                    "HHGD_LED:\t%u\n"
                    "HHGD_BUTTON:\t%u\n"
                    "HHGD_RELAY_1:\t%u\n"
                    "HHGD_RELAY_2:\t%u\n"
                    "HHGD_RELAY_3:\t%u\n"
                    "HHGD_RELAY_4:\t%u\n",
                    hhgd_led,
                    99, 
                    hhgd_relay_1,
                    hhgd_relay_2,
                    hhgd_relay_3,
                    hhgd_relay_4
    );

    pr_info("%s", msg);

    if (count > msg_len)
    {
        count = msg_len;
    }

    return simple_read_from_buffer(buff, count, off, msg, msg_len);
}

/*
** This function will be called when we write the Device file
*/
ssize_t hhgd_ioctl_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
    pr_info("--->1\n");
    len--;
    if (len > HHGD_PARSER_BUFF_MAX)
    {
        return -EINVAL;
    }

    char *params = (char *)kmalloc(len, GFP_KERNEL);
    if (params == NULL)
    {
        return -ENOMEM;
    }
    memset(params, '\0', len);

    if (copy_from_user(params, buff, len) != 0)
    {
        kfree(params);
        return -EINVAL;
    }

    kfree(params);


 pr_info("--->2\n");
    struct hhgd_parser parsed;
    if (!hhgd_parser_params(params, len, &parsed))
    {

        pr_err("Parsing error");
        return ENOEXEC;
    }

    switch (parsed.type)
    {
    case HHGD_LED:
        pr_info("HHGD_LED: %u\n", parsed.status);
        //hhgd_led_set_state(parsed.status);
        return len;
    case HHGD_BUTTON:
        return len;
    case HHGD_RELAY_1:
    case HHGD_RELAY_2:
    case HHGD_RELAY_3:
    case HHGD_RELAY_4:
        pr_info("HHGD_RELAY_%u: %u\n", parsed.type, parsed.status);
        //hhgd_relay_set_state(parsed.type, parsed.status);
        return len;
    case HHGD_LCD:
        pr_info("HHGD_RELAY_%u: %u\n", parsed.type, parsed.status);
        return len;
    }

    return -EINVAL;
}

long hhgd_ioctl( struct file *p_file, unsigned int ioctl_command, unsigned long arg)
{

    return 0;
}

/**
 * @brief Change access permission in user space
 */
int hhgd_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

/*
** Module Init function
*/
int __init hhgd_driver_init(void)
{

    /*Allocating Major number*/
    if ((alloc_chrdev_region(&hhgd_dev, HHGD_MAJOR_NUM_START, HHGD_MINOR_NUM_COUNT, HHGD_DRIVER_NAME)) < 0)
    {
        pr_err("Cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("Major = %d Minor = %d \n", MAJOR(hhgd_dev), MINOR(hhgd_dev));

    /*Creating cdev structure*/
    cdev_init(&hhgd_cdev, &fops);

    /*Adding character device to the system*/
    if ((cdev_add(&hhgd_cdev, hhgd_dev, 1)) < 0)
    {
        pr_err("Cannot add the device to the system\n");
        goto r_del;
    }

    /*Creating struct class*/
    if ((hhgd_class = class_create(THIS_MODULE, HHGD_CLASS_NAME)) == NULL)
    {
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }
    hhgd_class->dev_uevent = hhgd_uevent;

    /*Creating device*/
    if ((device_create(hhgd_class, NULL, hhgd_dev, NULL, HHGD_DRIVER_NAME)) == NULL)
    {
        pr_err("Cannot create the Device \n");
        goto r_device;
    }

    return 0;

r_device:
    device_destroy(hhgd_class, hhgd_dev);
r_class:
    class_destroy(hhgd_class);
r_del:
    cdev_del(&hhgd_cdev);
r_unreg:
    unregister_chrdev_region(hhgd_dev, 1);

    return -1;
}

/*
** Module exit function
*/
static void __exit hhgd_driver_exit(void)
{
    device_destroy(hhgd_class, hhgd_dev);
    class_destroy(hhgd_class);
    cdev_del(&hhgd_cdev);
    unregister_chrdev_region(hhgd_dev, 1);
    pr_info("Removed");
}

module_init(hhgd_driver_init);
module_exit(hhgd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Hi Happy Garden driver to SIMULATE access to hardware resources");
MODULE_INFO(intree, "Y");
MODULE_VERSION("0.10.0");
