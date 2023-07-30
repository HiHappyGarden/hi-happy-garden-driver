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



#ifndef _HHGD_RELAY_
#define _HHGD_RELAY_

#include "constants.h"

#include <linux/init.h>

struct hhgd_gpio_config;

/**
 * @brief Set all port to off
 */
void hhgd_relay_init(const struct hhgd_gpio_config* config) __attribute__((nonnull (1)));

/**
 * @brief Turn on/off relay
 * @param relay id
 * @param set true = on
 * @param set false = off
 */
void hhgd_relay_set_state(enum hhgd_type relay, bool set);

/**
 * @brief Togle relay
 * 
 */
void hhgd_relay_toggle_state(enum hhgd_type relay);

/**
 * @brief Get actual relay status
 * 
 * @return true Turn on
 * @return false Turn off
 */
bool hhgd_relay_get_state(enum hhgd_type relay);


#endif
