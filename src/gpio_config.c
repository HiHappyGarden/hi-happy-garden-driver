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

#include "gpio_config.h"

#include <linux/init.h>

#define HHGD_IS_VALID(gpio, msg) \
    if (gpio_is_valid(gpio) == false) \
    { \
        hhgd_error_new(error, HHGD_ERROR_GPIO_NOT_VALID, msg); \
        return false; \
    } 
#define HHGD_REQUEST(gpio, msg) \
    if (gpio_request(gpio, #gpio) < 0) \
    { \
        hhgd_error_new(error, HHGD_ERROR_GPIO_REQUEST, msg); \
        return false; \
    }

static bool hhgd_gpio_consig_is_valid(struct hhgd_error** error);
static bool hhgd_gpio_config_request(struct hhgd_error** error);

bool hhgd_gpio_config_init(struct hhgd_error** error)
{
    if (hhgd_gpio_consig_is_valid(error) == false)
    {
        goto r_gpio;
    }
    

    if (hhgd_gpio_config_request(error) == false)
    {
        goto r_gpio;
    }

    gpio_direction_input(HHGD_GPIO_BUTTON);

#ifdef EN_DEBOUNCE
  //Debounce the button with a delay of 200ms
  if(gpio_set_debounce(HHGD_GPIO_BUTTON, 200) < 0)
  {
    char err[10];
    sprintf(err, "%u", HHGD_GPIO_BUTTON);
    hhgd_error_new(error, HHGD_ERROR_GPIO_DEBOUNCE, "Imppossible init debouce"); 
    return false;
  }
#endif

    gpio_direction_output(HHGD_GPIO_LED, 0);
    gpio_direction_output(HHGD_GPIO_RELEAY_IN1, 0);
    gpio_direction_output(HHGD_GPIO_RELEAY_IN2, 0);
    gpio_direction_output(HHGD_GPIO_RELEAY_IN3, 0);
    gpio_direction_output(HHGD_GPIO_RELEAY_IN4, 0);
    gpio_direction_output(HHGD_GPIO_LED_BL, 0);

    /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
     ** Now you can change the gpio values by using below commands also.
     ** echo 1 > /sys/class/gpio/gpio21/value  (turn ON the LED)
     ** echo 0 > /sys/class/gpio/gpio21/value  (turn OFF the LED)
     ** cat /sys/class/gpio/gpio21/value  (read the value LED)
     **
     ** the second argument prevents the direction from being changed.
     */
    gpio_export(HHGD_GPIO_BUTTON, false);
    gpio_export(HHGD_GPIO_LED, false);
    gpio_export(HHGD_GPIO_RELEAY_IN1, false);
    gpio_export(HHGD_GPIO_RELEAY_IN2, false);
    gpio_export(HHGD_GPIO_RELEAY_IN3, false);
    gpio_export(HHGD_GPIO_RELEAY_IN4, false);
    gpio_export(HHGD_GPIO_LED_BL, false);
    return true;

r_gpio:
    hhgd_gpio_config_free();
    return false;
}


void hhgd_gpio_config_unexport(void)
{
    gpio_unexport(HHGD_GPIO_BUTTON);
    gpio_unexport(HHGD_GPIO_LED);
    gpio_unexport(HHGD_GPIO_RELEAY_IN1);
    gpio_unexport(HHGD_GPIO_RELEAY_IN2);
    gpio_unexport(HHGD_GPIO_RELEAY_IN3);
    gpio_unexport(HHGD_GPIO_RELEAY_IN4);
    gpio_unexport(HHGD_GPIO_LED_BL);
}

void hhgd_gpio_config_free(void)
{
    gpio_free(HHGD_GPIO_BUTTON);
    gpio_free(HHGD_GPIO_LED);
    gpio_free(HHGD_GPIO_RELEAY_IN1);
    gpio_free(HHGD_GPIO_RELEAY_IN2);
    gpio_free(HHGD_GPIO_RELEAY_IN3);
    gpio_free(HHGD_GPIO_RELEAY_IN4);
    gpio_free(HHGD_GPIO_LED_BL);
}

bool hhgd_gpio_consig_is_valid(struct hhgd_error** error)
{
    // Checking the GPIO is valid or not
    HHGD_IS_VALID(HHGD_GPIO_BUTTON, "HHGD_GPIO_BUTTON")
    HHGD_IS_VALID(HHGD_GPIO_LED, "HHGD_GPIO_LED")
    HHGD_IS_VALID(HHGD_GPIO_RELEAY_IN1, "HHGD_GPIO_RELEAY_IN1")
    HHGD_IS_VALID(HHGD_GPIO_RELEAY_IN2, "HHGD_GPIO_RELEAY_IN2")
    HHGD_IS_VALID(HHGD_GPIO_RELEAY_IN3, "HHGD_GPIO_RELEAY_IN3")
    HHGD_IS_VALID(HHGD_GPIO_RELEAY_IN4, "HHGD_GPIO_RELEAY_IN4")
    HHGD_IS_VALID(HHGD_GPIO_LED_BL, "HHGD_GPIO_LED_BL")
    
    return true;
}

bool hhgd_gpio_config_request(struct hhgd_error** error)
{
    HHGD_REQUEST(HHGD_GPIO_BUTTON, "HHGD_GPIO_BUTTON")
    HHGD_REQUEST(HHGD_GPIO_LED, "HHGD_GPIO_LED")
    HHGD_REQUEST(HHGD_GPIO_RELEAY_IN1, "HHGD_GPIO_RELEAY_IN1")
    HHGD_REQUEST(HHGD_GPIO_RELEAY_IN2, "HHGD_GPIO_RELEAY_IN2")
    HHGD_REQUEST(HHGD_GPIO_RELEAY_IN3, "HHGD_GPIO_RELEAY_IN3")
    HHGD_REQUEST(HHGD_GPIO_RELEAY_IN4, "HHGD_GPIO_RELEAY_IN4")
    HHGD_REQUEST(HHGD_GPIO_LED_BL, "HHGD_GPIO_LED_BL")
    return true;
}