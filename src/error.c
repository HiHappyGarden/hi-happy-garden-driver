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


#include "error.h"
#include "constants.h"

#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/stddef.h>

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) HHGD_NAME ": " fmt
#endif


static struct hhgd_error *last_error = NULL;


bool hhgd_error_new(struct hhgd_error** error, enum hhgd_error_code code, const char* msg)
{
    if(error == NULL)
    {
        last_error = NULL;
        return false;
    }
    if(*error != NULL)
    {
        last_error = NULL;
        return false;
    }
    *error = vmalloc(sizeof(struct hhgd_error));
    if(!*error)
    {
        last_error = NULL;
        return false;
    }
    (*error)->code = code;

    strncpy((*error)->msg, msg, MSG_LEN_ERROR_HGD);
    last_error = *error;
    return true;
}

void hhgd_error_print(struct hhgd_error* error, const char* msg, _Bool free)
{
    if(error == NULL)
    {
        return;
    }

    
    if(msg)
    {
        static const char fmt[] = "%s: %s code=%u msg=%s";
        size_t len = strlen(msg) + strlen(fmt) + strlen(HHGD_NAME);
        char* to_print = vmalloc(len);
        if(!to_print)
        {
            return;
        }
        memzero_explicit(to_print, len);

        sprintf(to_print, fmt, HHGD_NAME, msg, error->code, error->msg);

        pr_info("%s", to_print);

        vfree(to_print);
    }
    else
    {
        pr_info("error code=%u msg=%s", error->code, error->msg);
    }

    if(free)
    {
        hhgd_error_free(&error);
    }
}

inline const struct hhgd_error * hhgd_erro_get_last(void)
{
    return last_error;
}

void hhgd_error_free(struct hhgd_error** error)
{
    if(*error == NULL)
    {
        return;
    }

    if(last_error == *error)
    {
        last_error = NULL;
    }

    *error = NULL;
    vfree(*error);
}

