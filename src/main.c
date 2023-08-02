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

static bool hhgd_led_green  = 0;
static bool hhgd_led_red    = 0;
static bool hhgd_relay_in1  = 0;
static bool hhgd_relay_in2  = 0;
static bool hhgd_relay_in3  = 0;
static bool hhgd_relay_in4  = 0;
static char hhgd_lcd[34]    = { [0 ... 33] = 0 };

//MODULE

static short gpio_led_green     = -1;
static short gpio_led_red       = -1;
static short gpio_relay_in1     = -1;
static short gpio_relay_in2     = -1;
static short gpio_relay_in3     = -1;
static short gpio_relay_in4     = -1;
static short gpio_button_next   = -1;
static short gpio_button_before = -1;
static short gpio_lcd_power     = -1;


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


module_param(gpio_led_green, short, 0660);
MODULE_PARM_DESC(gpio_led_green, "GPIO LED GREEN");

module_param(gpio_led_red, short, 0660);
MODULE_PARM_DESC(gpio_led_red, "GPIO LED RED");

module_param(gpio_relay_in1, short, 0660);
MODULE_PARM_DESC(gpio_relay_in1, "GPIO RELAY IN1 - Releay ingres 1");

module_param(gpio_relay_in2, short, 0660);
MODULE_PARM_DESC(gpio_relay_in2, "GPIO RELAY IN2 - Releay ingres 2");

module_param(gpio_relay_in3, short, 0660);
MODULE_PARM_DESC(gpio_relay_in3, "GPIO RELAY IN3 - Releay ingres 3");

module_param(gpio_relay_in4, short, 0660);
MODULE_PARM_DESC(gpio_relay_in4, "GPIO RELAY IN4 - Releay ingres 4");

module_param(gpio_button_next, short, 0660);
MODULE_PARM_DESC(gpio_button_next, "GPIO BUTTON NEXT");

module_param(gpio_button_before, short, 0660);
MODULE_PARM_DESC(gpio_button_before, "GPIO BUTTON BEFORE");

module_param(gpio_lcd_power, short, 0660);
MODULE_PARM_DESC(gpio_lcd_power, "GPIO LCD POWER - Switch on/of the lcd");


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
    char msg[READ_BUFF_LEN] = { [ 0 ... READ_BUFF_LEN - 1] = 0};
    memset(msg, '\0', READ_BUFF_LEN);

    msg_len = snprintf(msg,
                    sizeof(msg), 
                    "HHGD_LED_GREEN:\t%u\n"
                    "HHGD_LED_RED:\t%u\n"
                    "HHGD_RELAY_IN1:\t%u\n"
                    "HHGD_RELAY_IN2:\t%u\n"
                    "HHGD_RELAY_IN3:\t%u\n"
                    "HHGD_RELAY_IN4:\t%u\n"
                    "HHGD_LCD:\t%s\n",
                    hhgd_led_green,
                    hhgd_led_red,
                    hhgd_relay_in1,
                    hhgd_relay_in2,
                    hhgd_relay_in3,
                    hhgd_relay_in4,
                    hhgd_lcd
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
*  echo "HHGD_RELAY_IN1 1" > /dev/hhgd
*/
ssize_t hhgd_ioctl_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
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

    struct hhgd_parser parsed;
    if (!hhgd_parser_params(params, len, &parsed))
    {
        pr_err("Parsing error");
        kfree(params);
        return -ENOEXEC;
    }
    
    kfree(params);


    switch (parsed.type)
    {
    case HHGD_LED_GREEN:
        hhgd_led_green = parsed.status;
        break;
    case HHGD_LED_RED:
        hhgd_led_red = parsed.status;
        break;
    case HHGD_RELAY_IN1:
        hhgd_relay_in1 = parsed.status;
        break;
    case HHGD_RELAY_IN2:
        hhgd_relay_in2 = parsed.status;
        break;
    case HHGD_RELAY_IN3:
        hhgd_relay_in3 = parsed.status;
        break;
    case HHGD_RELAY_IN4:
        hhgd_relay_in4 = parsed.status;
        break;
    case HHGD_LCD:  
        strncpy(hhgd_lcd, parsed.buff, sizeof(hhgd_lcd));
        break;
    default:
        break;
    }

    len++;
    return len;
}

long hhgd_ioctl( struct file *p_file, unsigned int ioctl_command, unsigned long arg)
{
    
    const void* arg_ptr = (const void *)arg;

	if(arg_ptr == NULL) 
    {
		printk( KERN_DEBUG "Invalid argument for klcd IOCTL \n");
		return -EINVAL;
	}

    switch (ioctl_command)
    {
    case HHGD_BUTTON_NEXT ... HHGD_BUTTON_BEFORE:
        {
            pr_info("No action for buttons");
        }
        break;
    case HHGD_RELAY_IN1:
        {
            if( copy_from_user(&hhgd_relay_in1, arg_ptr, sizeof(hhgd_relay_in1)) )
            {
                pr_err("Failed to copy from user space buffer");
                return -EFAULT;
            }
        }
        break;
    case HHGD_RELAY_IN2:
        {
            if( copy_from_user(&hhgd_relay_in2, arg_ptr, sizeof(hhgd_relay_in2)) )
            {
                pr_err("Failed to copy from user space buffer");
                return -EFAULT;
            }
        }
        break;
    case HHGD_RELAY_IN3:
        {
            if( copy_from_user(&hhgd_relay_in3, arg_ptr, sizeof(hhgd_relay_in3)) )
            {
                pr_err("Failed to copy from user space buffer");
                return -EFAULT;
            }
        }
        break;
    case HHGD_RELAY_IN4:
        {
            if( copy_from_user(&hhgd_relay_in4, arg_ptr, sizeof(hhgd_relay_in4)) )
            {
                pr_err("Failed to copy from user space buffer");
                return -EFAULT;
            }
        }
        break;
    case HHGD_LED_GREEN:
        {
            if( copy_from_user(&hhgd_led_green, arg_ptr, sizeof(hhgd_led_green)) )
            {
                pr_err("Failed to copy from user space buffer");
                return -EFAULT;
            }
        }
        break;
    case HHGD_LED_RED:
        {
            if( copy_from_user(&hhgd_led_red, arg_ptr, sizeof(hhgd_led_red)) )
            {
                pr_err("Failed to copy from user space buffer");
                return -EFAULT;
            }
        }
        break;
    case HHGD_LCD:
        {
            if( copy_from_user(hhgd_lcd, arg_ptr, sizeof(hhgd_lcd)) )
            {
                pr_err("Failed to copy from user space buffer");
                return -EFAULT;
            }
        }
        break;
    }

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
