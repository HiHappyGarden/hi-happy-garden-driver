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


#ifndef _HHGD_LED_
#define _HHGD_LED_

#include "constants.h"

#include <linux/init.h>

struct hhgd_gpio_config;

/**
 * @brief Init led to off
 * 
 */
void hhgd_led_init(const struct hhgd_gpio_config* config) __attribute__((nonnull (1)));

/**
 * @brief Turn on/off led
 * 
 * @param set true = on
 * @param set false = off
 */
void hhgd_led_set_state(enum hhgd_type type, bool set);

/**
 * @brief Toggle led
 * 
 */
void hhgd_led_toggle_state(enum hhgd_type type);

/**
 * @brief Get actual led status
 * 
 * @return true Turn on
 * @return false Turn off
 */
bool hhgd_led_get_state(enum hhgd_type type);

#endif
