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



#ifndef _HHGD_GPIO_CONFIG_
#define _HHGD_GPIO_CONFIG_

#include "error.h"
#include "constants.h"

#include <linux/gpio.h>

/// IN | Button pin
#define HHGD_GPIO_BUTTON      (3) 

/// OUT | Led pin
#define HHGD_GPIO_LED        (24)

/// OUT | Releay port 1
#define HHGD_GPIO_RELEAY_IN1 (25) 

/// OUT | Releay port 2
#define HHGD_GPIO_RELEAY_IN2 (8)

/// OUT | Releay port 3
#define HHGD_GPIO_RELEAY_IN3 (7) 

/// OUT | Releay port 4
#define HHGD_GPIO_RELEAY_IN4 (1) 

/// OUT | Balcklighting anode
#define HHGD_GPIO_LED_BL     (14) 

struct hhgd_gpio_config
{
    short* led_green;
    short* led_red  ;
    short* relay_in1;
    short* relay_in2;
    short* relay_in3;
    short* relay_in4;
    short* button_next;
    short* button_before;
    short* lcd_power;
};

/**
 * @brief Init pin configuration
 * 
 * @param[out] error if pass NULL not return erro 
 * @return 0 init correctly
 * @return false some error
 */
bool hhgd_gpio_config_init(const struct hhgd_gpio_config* config, struct hhgd_error** error) __attribute__((nonnull (1)));

/**
 * @brief Free pin config
 * 
 */
void hhgd_gpio_config_unexport(void);

/**
 * @brief Free pin config
 * 
 */
void hhgd_gpio_config_free(void);

#endif
