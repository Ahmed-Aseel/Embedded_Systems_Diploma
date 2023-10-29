/* 
 * File:   ecu_keypad.c
 * Author: Ahmed Aseel
 *
 * Created on April 22, 2023, 1:29 AM
 */

#include "ecu_keypad.h"

static const uint8 keypad_valus[KEYPAD_ROWS_NUMBER][KEYPAD_COLUMNS_NUMBER] = 
{
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'#', '0', '=', '+'},
};

Std_ReturnType ecu_keypad_initialize(const keypad_config_t* _keypad_config)
{
    Std_ReturnType ret = E_OK;
    uint8 rows_counter = 0, columns_counter = 0;
        
    if(NULL == _keypad_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        for(rows_counter = 0; rows_counter < KEYPAD_ROWS_NUMBER; rows_counter++)
        {
            ret = gpio_pin_initialize(&_keypad_config->keypad_rows_pins[rows_counter]);        
        }
        
        for(columns_counter = 0; columns_counter < KEYPAD_COLUMNS_NUMBER; columns_counter++)
        {
            ret = gpio_pin_direction_initialize(&_keypad_config->keypad_columns_pins[columns_counter]);        
        }
    }
        
    return ret;     
}

Std_ReturnType ecu_keypad_get_value(const keypad_config_t* _keypad_config, uint8* value)
{
    Std_ReturnType ret = E_OK;
    uint8 rows_counter = 0, columns_counter = 0, counter = 0;
    logic_t column_logic = GPIO_LOW;
    
    if(NULL == _keypad_config || NULL == value) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        for(rows_counter = 0; rows_counter < KEYPAD_ROWS_NUMBER; rows_counter++)
        {
            for(counter = 0; counter < KEYPAD_ROWS_NUMBER; counter++)
            {
                ret = gpio_pin_write_logic(&_keypad_config->keypad_rows_pins[counter], GPIO_LOW);        
            }
            
            ret = gpio_pin_write_logic(&_keypad_config->keypad_rows_pins[rows_counter], GPIO_HIGH); 
            
            for(columns_counter = 0; columns_counter < KEYPAD_COLUMNS_NUMBER; columns_counter++)
            {
                ret = gpio_pin_read_logic(&_keypad_config->keypad_columns_pins[columns_counter], &column_logic);
                if(GPIO_HIGH == column_logic)
                {
                   *value = keypad_valus[rows_counter][columns_counter]; 
                }
            }        
        }
    }
        
    return ret;      
}