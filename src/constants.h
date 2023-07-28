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

#define HHGD_NAME "hhgd" 
#define HHGD_TO_STR(_str) #_str

typedef unsigned char u8;
typedef void* hhgd_ptr;

enum hhgd_type
{
    HHGD_LED,
    HHGD_RELAY_1,
    HHGD_RELAY_2,
    HHGD_RELAY_3,
    HHGD_RELAY_4,
    HHGD_BUTTON,
    HHGD_LCD,
};


#endif
