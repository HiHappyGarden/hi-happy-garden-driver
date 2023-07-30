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

static short gpio_led_green;
static short gpio_led_red;

void hhgd_led_init(const struct hhgd_gpio_config* config)
{
    if(config == NULL)
    {
        return false;
    }

    gpio_led_green = config->led_green;
    gpio_led_red = config->led_red;

    hhgd_led_set_state(false);
}

void hhgd_led_set_state(enum hhgd_type type, bool state)
{
    if(type == HHGD_LED_GREEN)
    {
        if(hhgd_led_get_state(type) != state)
        {
            gpio_set_value(gpio_led_green, state);
        }
    }
    else if(type == HHGD_LED_RED)
    {
        if(hhgd_led_get_state(type) != state)
        {
            gpio_set_value(gpio_led_red, state);
        }
    }
}

void hhgd_led_toggle_state(enum hhgd_type type)
{
    if(type == HHGD_LED_GREEN )
    {
        hhgd_led_set_state(!gpio_get_value(gpio_led_green));
    }
    else if(type == HHGD_LED_GREEN )
    {
        hhgd_led_set_state(!gpio_get_value(gpio_led_red));
    }
}

bool hhgd_led_get_state(enum hhgd_type type)
{
    if(type == HHGD_LED_GREEN )
    {
       return gpio_get_value(gpio_led_green):
    }
    else if(type == HHGD_LED_GREEN )
    {
        return gpio_get_value(gpio_led_red);
    }
    return false;
}