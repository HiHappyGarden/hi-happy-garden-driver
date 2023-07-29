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

#ifndef _HHGD_PARSER_
#define _HHGD_PARSER_
#include "constants.h"

#include <linux/init.h>

#define HHGD_PARSER_BUFF_MAX (48)

struct hhgd_parser
{
    enum hhgd_type type;
    char buff[HHGD_PARSER_BUFF_MAX];
    u8 status;
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
bool hhgd_parser_params(const char* buff, size_t len, struct hhgd_parser* parser);

#endif
