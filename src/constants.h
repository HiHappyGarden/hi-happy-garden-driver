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


#ifndef _HHGD_COSTANTS_
#define _HHGD_COSTANTS_

#define HHGD_TO_STR(_str) #_str

#define HHGD_DRIVER_NAME "hhgd"
#define HHGD_CLASS_NAME "hhgd"
#define HHGD_MAJOR_NUM_START (0)
#define HHGD_MINOR_NUM_COUNT (1)
#define HHGD_SIGETX 10

typedef unsigned char u8;
typedef void* hhgd_ptr;

enum hhgd_type
{
    HHGD_LED_GREEN,
    HHGD_LED_RED,
    HHGD_RELAY_IN1,
    HHGD_RELAY_IN2,
    HHGD_RELAY_IN3,
    HHGD_RELAY_IN4,
    HHGD_BUTTON_NEXT,
    HHGD_BUTTON_BEFORE,
    HHGD_LCD,
    HHGD_NONE,
};

#endif
