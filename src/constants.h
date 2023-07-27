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


#ifndef _HDG_COSTANTS_
#define _HDG_COSTANTS_

#define HGD_NAME "happy_gardenpi_driver" 
#define HGD_TO_STR(_str) #_str

typedef unsigned char __u8;
typedef void* hgd_ptr;

enum hgd_type
{
    HGD_LED,
    HGD_RELAY_1,
    HGD_RELAY_2,
    HGD_RELAY_3,
    HGD_RELAY_4,
    HGD_BUTTON,
    HGD_LCD,
};


#endif
