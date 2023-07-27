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

#ifndef _HDG_PARSER_
#define _HDG_PARSER_
#include "constants.h"

#include <linux/init.h>

#define HGD_PARSER_BUFF_MAX (48)

struct hgd_parser
{
    enum hgd_type type;
    char buff[HGD_PARSER_BUFF_MAX];
    __u8 status;
};

/**
 * @brief Parse write section
 *
 * @param buff data buffer from user
 * @param len buffer
 * @param[out] parser data parsed (already alloced)
 * @return true successful
 * @return false fail
 */
bool hgd_parser_params(const char* buff, size_t len, struct hgd_parser* parser);

#endif
