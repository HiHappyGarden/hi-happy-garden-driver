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



#ifndef _HHGD_ERROR_
#define _HHGD_ERROR_

#include <linux/kernel.h>

#define MSG_LEN_ERROR_HGD (30)

/**
 * @brief Error code 
 */
enum hhgd_error_code
{
    HHGD_ERROR_NONE,
    HHGD_ERROR_GPIO_REQUEST = 1,
    HHGD_ERROR_GPIO_EXPORT,
    HHGD_ERROR_GPIO_DIRECTION,
    HHGD_ERROR_GPIO_DEBOUNCE,
    HHGD_ERROR_GPIO_IRQ,
    HHGD_ERROR_SYS_INFO_SYSFS_CREATE,
    HHGD_ERROR_SYS_INFO_SYSFS_NO_VHEAP,
};

/**
 * @brief Error structure
 */
struct hhgd_error
{
    enum hhgd_error_code code;          //!< Error code
    char msg[MSG_LEN_ERROR_HGD];    //!< Error message
};

/**
 * @brief Build new error
 * 
 * @param error 
 * @return 0 ok 
 * @return 0 error 
 */
bool hhgd_error_new(struct hhgd_error** error, enum hhgd_error_code code, const char* msg);

/**
 * @brief Prinnt and free error
 * 
 * @param error to print
 * @param msg addtidional message to add if NULL not print nothing
 * @param free 0 not free
 */
void hhgd_error_print(struct hhgd_error* error, const char* msg, bool free)  __attribute__((nonnull (1)));

/**
 * @brief Gel last error
 * 
 * @return const struct hhgd_error* NULL il no error occured
 */
const struct hhgd_error * hhgd_erro_get_last(void);

/**
 * @brief Free a rerro
 * 
 * @param error to free
 */
void hhgd_error_free(struct hhgd_error** error);

#endif
