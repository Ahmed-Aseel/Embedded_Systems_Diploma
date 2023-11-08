/* 
 * File:   ecu_7_segment.c
 * Author: Ahmed Aseel
 *
 * Created on April 20, 2023, 8:03 PM
 */

#include "ecu_7_segment.h"

Std_ReturnType ecu_7_segment_initialize(const segment_config_t* _segment_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _segment_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_initialize(&_segment_config->segment_pins[SEGMENT_PIN1_INDEX]);
        ret = gpio_pin_initialize(&_segment_config->segment_pins[SEGMENT_PIN2_INDEX]);
        ret = gpio_pin_initialize(&_segment_config->segment_pins[SEGMENT_PIN3_INDEX]);
        ret = gpio_pin_initialize(&_segment_config->segment_pins[SEGMENT_PIN4_INDEX]);
    }
        
    return ret;       
}

Std_ReturnType ecu_7_segment_write_number(const segment_config_t* _segment_config, uint8 number)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _segment_config && number > 9) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_write_logic(&_segment_config->segment_pins[SEGMENT_PIN1_INDEX], number & 0x01);
        ret = gpio_pin_write_logic(&_segment_config->segment_pins[SEGMENT_PIN2_INDEX], (number >> 1) & 0x01);
        ret = gpio_pin_write_logic(&_segment_config->segment_pins[SEGMENT_PIN3_INDEX], (number >> 2) & 0x01);
        ret = gpio_pin_write_logic(&_segment_config->segment_pins[SEGMENT_PIN4_INDEX], (number >> 3) & 0x01);
    }
        
    return ret;       
}