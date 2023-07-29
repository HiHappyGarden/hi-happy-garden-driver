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

#include "parser.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>


#define HHGD_STD_FILL_PARSER(_const) \
        parser->type = _const; \
        long status; \
        long ret = kstrtol (args[1], 10, &status); \
        if(ret == -ERANGE || ret == -EINVAL ) \
        { \
            return false; \
        } \
        parser->status = status; \
        return true; 

bool hhgd_parser_params(const char* buff, size_t len, struct hhgd_parser* parser)
{
    if (!parser || !buff)
    {
        return false;
    }

    memset(parser, 0, sizeof(struct hhgd_parser));

    char args[2][HHGD_PARSER_BUFF_MAX] = {
        [0] = { [ 0 ... HHGD_PARSER_BUFF_MAX -1] = 0 },
        [1] = { [ 0 ... HHGD_PARSER_BUFF_MAX -1] = 0 },
    };

    u8 no_buffer_overflow = 0;
    bool check = true;
    char* args0_ptr = args[0];
    char* args1_ptr = args[1];
    const char* ptr_after_arg0 = buff; 
    for(u8 i = 0; i < len; i++)
    {
        if(buff[i] == ' ')
        {
            if(check == false && strlen(args[1]))
            {
                break;
            }
            else
            {
                no_buffer_overflow = 0;
                check = false;
                ptr_after_arg0++;
                continue;
            }
            
        }
        else
        {
            if(check)
            {
                if(no_buffer_overflow >= HHGD_PARSER_BUFF_MAX)
                {
                    return false;
                }
                *args0_ptr = buff[i];
                args0_ptr++;
                ptr_after_arg0++;
                no_buffer_overflow++;
            } 
            else
            {
                if(no_buffer_overflow >= HHGD_PARSER_BUFF_MAX)
                {
                    return false;
                }
                *args1_ptr = buff[i];
                args1_ptr++;
                no_buffer_overflow++;
            }
        }

        
    }

    if(strncmp(args[0], HHGD_TO_STR(HHGD_LED), HHGD_PARSER_BUFF_MAX) == 0)
    {
        HHGD_STD_FILL_PARSER(HHGD_LED)
    }
    else if(strncmp(args[0], HHGD_TO_STR(HHGD_BUTTON), HHGD_PARSER_BUFF_MAX) == 0)
    {
        HHGD_STD_FILL_PARSER(HHGD_BUTTON)
    }
    if(strncmp(args[0], HHGD_TO_STR(HHGD_LCD), HHGD_PARSER_BUFF_MAX) == 0)
    {
        parser->type = HHGD_LCD;
        strncpy(parser->buff, ptr_after_arg0,  strlen(ptr_after_arg0));
    }
    else if(strncmp(args[0], HHGD_TO_STR(HHGD_RELAY_1), HHGD_PARSER_BUFF_MAX) == 0)
    {
        HHGD_STD_FILL_PARSER(HHGD_RELAY_1)   
    }
    else if(strncmp(args[0], HHGD_TO_STR(HHGD_RELAY_2), HHGD_PARSER_BUFF_MAX) == 0)
    {
        HHGD_STD_FILL_PARSER(HHGD_RELAY_2)
    }
    else if(strncmp(args[0], HHGD_TO_STR(HHGD_RELAY_3), HHGD_PARSER_BUFF_MAX) == 0)
    {
        HHGD_STD_FILL_PARSER(HHGD_RELAY_3)
    }
    else if(strncmp(args[0], HHGD_TO_STR(HHGD_RELAY_4), HHGD_PARSER_BUFF_MAX) == 0)
    {
        HHGD_STD_FILL_PARSER(HHGD_RELAY_4)
    }
    return true;
}
