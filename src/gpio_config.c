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

#include "gpio_config.h"

#include <linux/init.h>

static struct hhgd_gpio_config config;


static bool hhg_lcd_pin_out_setup(u8 gpio_number, u8 gpio_direction, struct hhgd_error** error);
static bool hhg_lcd_pin_in_setup(u8 gpio_number, struct hhgd_error** error);

bool hhgd_gpio_config_init(const struct hhgd_gpio_config* _config, struct hhgd_error** error)
{
    if(config == NULL)
    {
        return false;
    }
    memcpy(&config, _config);

    if(!hhg_lcd_pin_out_setup(config.led_green, 0, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_out_setup(config.led_red, 0, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_out_setup(config.relay_in1, 0, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_out_setup(config.relay_in2, 0, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_out_setup(config.relay_in3, 0, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_out_setup(config.relay_in4, 0, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_out_setup(config.lcd_power, 0, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_in_setup(config.button_next, error))
    {
        return false;
    }

    if(!hhg_lcd_pin_in_setup(config.button_before, error))
    {
        return false;
    }

#ifdef EN_DEBOUNCE
  //Debounce the button with a delay of 200ms
  if(gpio_set_debounce(HHGD_GPIO_BUTTON, 200) < 0)
  {
    char err[10];
    sprintf(err, "%u", HHGD_GPIO_BUTTON);
    hhgd_error_new(error, HHGD_ERROR_GPIO_DEBOUNCE, "Imppossible init debouce"); 
    return false;
  }
#endif

    return true;

r_gpio:
    hhgd_gpio_config_free();
    return false;
}


void hhgd_gpio_config_unexport(void)
{
    gpio_unexport(config.led_green);
    gpio_unexport(config.led_red);
    gpio_unexport(config.relay_in1);
    gpio_unexport(config.relay_in2);
    gpio_unexport(config.relay_in3);
    gpio_unexport(config.relay_in4);
    gpio_unexport(config.button_next);
    gpio_unexport(config.button_before);
    gpio_unexport(config.lcd_power);
}

void hhgd_gpio_config_free(void)
{
    gpio_free(config.led_green);
    gpio_free(config.led_red);
    gpio_free(config.relay_in1);
    gpio_free(config.relay_in2);
    gpio_free(config.relay_in3);
    gpio_free(config.relay_in4);
    gpio_free(config.button_next);
    gpio_free(config.button_before);
    gpio_free(config.lcd_power);
}



bool hhg_lcd_pin_out_setup(u8 gpio_number, u8 gpio_direction, struct hhgd_error** error)
{
	u8 ret;

	ret = gpio_request( gpio_number, "GPIO request");
	if( ret != 0 )	{
        char msg[MSG_LEN_ERROR_HGD] = { [0 ... MSG_LEN_ERROR_HGD - 1] = 0};
        printf(msg, sizeof(msg) - 1, "Failed to request GPIO %d \n", gpio_number);
        hhgd_error_new(error, HHGD_ERROR_GPIO_REQUEST, msg);
		return false;
	}	
	
	ret = gpio_export( gpio_number, 0);
	if( ret != 0 )	{
        char msg[MSG_LEN_ERROR_HGD] = { [0 ... MSG_LEN_ERROR_HGD - 1] = 0};
        printf(msg, sizeof(msg) - 1, "Failed to export GPIO %d \n", gpio_number);
        hhgd_error_new(error, HHGD_ERROR_GPIO_EXPORT, msg);
		return false;
	}

	ret = gpio_direction_output( gpio_number, gpio_direction);
	if( ret != 0 )	{
        char msg[MSG_LEN_ERROR_HGD] = { [0 ... MSG_LEN_ERROR_HGD - 1] = 0};
        printf(msg, sizeof(msg) - 1, "Failed to set GPIO direction %d \n", gpio_number );
        hhgd_error_new(error, HHGD_ERROR_GPIO_DIRECTION, msg);
		return false;
	}

	gpio_set_value(gpio_number, 0);

	return true; 
}

bool hhg_lcd_pin_in_setup(u8 gpio_number, struct hhgd_error** error)
{
    u8 ret;

	ret = gpio_request( gpio_number, "GPIO request");
	if( ret != 0 )	{
        char msg[MSG_LEN_ERROR_HGD] = { [0 ... MSG_LEN_ERROR_HGD - 1] = 0};
        printf(msg, sizeof(msg) - 1, "Failed to request GPIO %d \n", gpio_number);
        hhgd_error_new(error, HHGD_ERROR_GPIO_REQUEST, msg);
		return false;
	}	
	
	ret = gpio_export( gpio_number, 0);
	if( ret != 0 )	{
        char msg[MSG_LEN_ERROR_HGD] = { [0 ... MSG_LEN_ERROR_HGD - 1] = 0};
        printf(msg, sizeof(msg) - 1, "Failed to export GPIO %d \n", gpio_number);
        hhgd_error_new(error, HHGD_ERROR_GPIO_EXPORT, msg);
		return false;
	}

	ret = gpio_direction_input( gpio_number);
	if( ret != 0 )	{
        char msg[MSG_LEN_ERROR_HGD] = { [0 ... MSG_LEN_ERROR_HGD - 1] = 0};
        printf(msg, sizeof(msg) - 1, "Failed to set GPIO direction %d \n", gpio_number );
        hhgd_error_new(error, HHGD_ERROR_GPIO_DIRECTION, msg);
		return false;
	}

	gpio_set_value(gpio_number, 0);

	return true; 
}