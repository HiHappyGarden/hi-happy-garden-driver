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

#include "button.h"

#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/gpio.h> //GPIO
#include <linux/ioctl.h>
#include <linux/interrupt.h>
/* Since debounce is not supported in Raspberry pi, I have addded this to disable
** the false detection (multiple IRQ trigger for one interrupt).
** Many other hardware supports GPIO debounce, I don't want care about this even
** if this has any overhead. Our intention is to explain the GPIO interrupt.
** If you want to disable this extra coding, you can comment the below macro.
** This has been taken from : https://raspberrypi.stackexchange.com/questions/8544/gpio-interrupt-debounce
**
** If you want to use Hardaware Debounce, then comment this EN_DEBOUNCE.
**
*/

#include "error.h"
#include "constants.h"
#include "gpio_config.h"

#include <linux/jiffies.h>

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) HHGD_DRIVER_NAME ": " fmt
#endif

#define DIFF_JIFFIES 25
#define MS_TO_MAINTAIN_CLICK 5

extern unsigned long volatile jiffies;
unsigned long old_jiffies = 0;

/** Global variables and defines for userspace app registration */
#define REGISTER_UAPP _IO('R', 'g')
static struct task_struct *task = NULL;

static short* gpio_button_next;
static short* gpio_button_before;

static u32 gpio_irq_number_next;
static u32 gpio_irq_number_before;

static atomic_t thread_busy = ATOMIC_INIT(0);




static irqreturn_t gpio_irq_handler(int irq, void *dev_id);

static irqreturn_t button_next_gpio_interrupt_thread_fn(int irq, void *dev_id);
static irqreturn_t button_before_gpio_interrupt_thread_fn(int irq, void *dev_id);
static irqreturn_t gpio_interrupt_thread_fn(enum hhgd_type type, int irq, void *dev_id);

bool hhgd_button_init(const struct hhgd_gpio_config* config, struct hhgd_error **error)
{
  gpio_button_next = config->button_next;
  gpio_button_before = config->button_before;

  // Get the IRQ number for our GPIO
  gpio_irq_number_next = gpio_to_irq(*gpio_button_next);

  if (request_threaded_irq(gpio_irq_number_next,          // IRQ number
                           gpio_irq_handler, // IRQ handler (Top half)
                           button_next_gpio_interrupt_thread_fn, // IRQ Thread handler (Bottom half)
                           IRQF_TRIGGER_FALLING,     // Handler will be called in raising edge
                           HHGD_DRIVER_NAME,         // used to identify the device name using this IRQ
                           NULL))                    // device id for shared IRQ
  {
    hhgd_error_new(error, HHGD_ERROR_GPIO_IRQ, "cannot register IRQ for button next");
    return false;
  }

  gpio_irq_number_before = gpio_to_irq(*gpio_button_next);

  if (request_threaded_irq(gpio_irq_number_before,   // IRQ number
                           gpio_irq_handler, // IRQ handler (Top half)
                           button_before_gpio_interrupt_thread_fn, // IRQ Thread handler (Bottom half)
                           IRQF_TRIGGER_FALLING,     // Handler will be called in raising edge
                           HHGD_DRIVER_NAME,         // used to identify the device name using this IRQ
                           NULL))                    // device id for shared IRQ
  {
    hhgd_error_new(error, HHGD_ERROR_GPIO_IRQ, "cannot register IRQ for button before");
    return false;
  }

  return true;
}

long hhgd_button_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	if(cmd == REGISTER_UAPP) 
  {
		task = get_current();
		pr_info("gpio_irq_signal: Userspace app with PID %d is registered\n", task->pid);
	}
  return 0;
}

bool hhgd_button_get_state(enum hhgd_type type)
{
  switch (type)
  {
  case HHGD_BUTTON_NEXT:
    return !gpio_get_value(gpio_irq_number_next);
  case HHGD_BUTTON_BEFORE:
    return !gpio_get_value(gpio_irq_number_before);
  default:
    return false;
  }
}

inline void hhgd_button_free(void)
{
  if(task != NULL)
  {
    	task = NULL;
  }
  free_irq(gpio_irq_number_next, NULL);
  free_irq(gpio_irq_number_before, NULL);
}


// Interrupt handler for GPIO 25. This will be called whenever there is a raising edge detected.
irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
  // avoid continuos click
  unsigned long diff = jiffies - old_jiffies;
  if (diff < DIFF_JIFFIES)
  {
    return IRQ_HANDLED;
  }

  old_jiffies = jiffies;

  if (atomic_read(&thread_busy) > 0)
  {
    pr_info("Thread busy\n");
    return IRQ_HANDLED;
  }

  atomic_inc(&thread_busy);

  /*
  ** If you don't want to call the thread fun, then you can just return
  ** IRQ_HANDLED. If you return IRQ_WAKE_THREAD, then thread fun will be called.
  */
  return IRQ_WAKE_THREAD;
}


inline irqreturn_t button_next_gpio_interrupt_thread_fn(int irq, void *dev_id)
{
  return gpio_interrupt_thread_fn(HHGD_BUTTON_NEXT, irq, dev_id);
}

inline irqreturn_t button_before_gpio_interrupt_thread_fn(int irq, void *dev_id)
{
  return gpio_interrupt_thread_fn(HHGD_BUTTON_BEFORE, irq, dev_id);
}

irqreturn_t gpio_interrupt_thread_fn(enum hhgd_type type, int irq, void *dev_id)
{

  for (u8 i = 0; i < MS_TO_MAINTAIN_CLICK; i++)
  {
    if (hhgd_button_get_state(type) == 0)
    {
      atomic_sub(1, &thread_busy);
      return IRQ_HANDLED;
    }

    mdelay(1);
  }

  struct kernel_siginfo info;

	if(task != NULL) {

    // Sending signal to app
    memset(&info, 0, sizeof(struct kernel_siginfo));
    info.si_signo = HHGD_SIGETX;

    // This is bit of a trickery: SI_QUEUE is normally used by sigqueue from user space,    and kernel space should use SI_KERNEL. 
    // But if SI_KERNEL is used the real_time data  is not delivered to the user space signal handler function. */
    info.si_code = SI_QUEUE;

    // real time signals may have 32 bits of data.
    info.si_int = 1;



		/* Send the signal */
		if(send_sig_info(HHGD_SIGETX, &info, task) < 0) 
			printk("gpio_irq_signal: Error sending signal\n");
	}

  atomic_sub(1, &thread_busy);

  return IRQ_HANDLED;
}