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


#include "led.h"
#include "gpio_config.h"


void hhgd_led_init(void)
{
    hhgd_led_set_state(false);
}

void hhgd_led_set_state(bool state)
{
    if(hhgd_led_get_state() != state)
    {
        gpio_set_value(HHGD_GPIO_LED, state);
    }
}

inline void hhgd_led_toggle_state(void)
{
    hhgd_led_set_state(!gpio_get_value(HHGD_GPIO_LED));
}

inline bool hhgd_led_get_state(void)
{
    return gpio_get_value(HHGD_GPIO_LED);
}