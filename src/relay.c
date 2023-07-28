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


#include "relay.h"
#include "gpio_config.h"

static u8 get_relay_gpio(enum hhgd_type relay)
{
    switch (relay)
    {
    case HHGD_RELAY_1:
        return HHGD_GPIO_RELEAY_IN1;
    case HHGD_RELAY_2:
        return HHGD_GPIO_RELEAY_IN2;
    case HHGD_RELAY_3:
        return HHGD_GPIO_RELEAY_IN3;
    case HHGD_RELAY_4:
        return HHGD_GPIO_RELEAY_IN4;
    default:
        return 0;
    }
}
void hhgd_relay_init(void)
{
    hhgd_relay_set_state(HHGD_RELAY_1, false);
    hhgd_relay_set_state(HHGD_RELAY_2, false);
    hhgd_relay_set_state(HHGD_RELAY_3, false);
    hhgd_relay_set_state(HHGD_RELAY_4, false);
}

void hhgd_relay_set_state(enum hhgd_type relay, bool state)
{
    if(hhgd_relay_get_state(relay) != state)
    {
        u8 gpio = 0;
        if((gpio = get_relay_gpio(relay)) > 0)
        {
            gpio_set_value(gpio, !state);
        }
    }
}

inline void hhgd_relay_toggle_state(enum hhgd_type relay)
{
    u8 gpio = 0;
    if((gpio = get_relay_gpio(relay)) > 0)
    {
        hhgd_relay_set_state(gpio, !hhgd_relay_get_state(gpio));
    }
}

inline bool hhgd_relay_get_state(enum hhgd_type relay)
{
    u8 gpio = 0;
    if((gpio = get_relay_gpio(relay)) > 0)
    {
        return !gpio_get_value(gpio);
    }
    else
    {
        return false;
    }
}