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


#ifndef _HHGD_BUTTON_
#define _HHGD_BUTTON_

#include "error.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioctl.h>

#define HHGD_SIGETX 44

/**
 * @brief Init button 
 * 
 * @param[out] error if pass NULL not return erro 
 * @return true if ok
 * @return false if error
 */
bool hhgd_button_init(struct hhgd_error **error);

/**
 * @brief To send signal retrived from irq when click
 */
long hhgd_button_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// void hhgd_button_release(void);

/**
 * @brief Get actual button status
 * 
 * @return true Turn on
 * @return false Turn off
 */
bool hhgd_button_get_state(void);

/**
 * @brief Free button
 */
void hhgd_button_free(void);

#endif
