/* 
 * File:   ecu_button.c
 * Author: Ahmed Aseel
 *
 * Created on April 18, 2023, 2:20 PM
 */

#include "ecu_button.h"

/**
 * 
 * @param _btn_config
 * @return 
 */
Std_ReturnType ecu_button_initialize(const button_config_t* _btn_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _btn_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = gpio_pin_direction_initialize(&_btn_config->button_pin);
    }
        
    return ret;
}

/**
 * 
 * @param _btn_config
 * @param btn_state
 * @return 
 */
Std_ReturnType ecu_button_read_state(const button_config_t* _btn_config, button_state_t* btn_state)
{
    Std_ReturnType ret = E_OK;
    logic_t pin_logic_state = GPIO_LOW;
    
    if(NULL == _btn_config || NULL == btn_state) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(BUTTON_ACTIVE_LOW == _btn_config->button_connection)
        {
            ret = gpio_pin_read_logic(&_btn_config->button_pin, &pin_logic_state);
            
            if(GPIO_HIGH == pin_logic_state)
            {
                *btn_state = BUTTON_RELEASED;
            }
            else
            {
                *btn_state = BUTTON_PRESSED;                
            }   
        }
        
        else if(BUTTON_ACTIVE_HIGH == _btn_config->button_connection)
        {
            ret = gpio_pin_read_logic(&_btn_config->button_pin, &pin_logic_state);
            
            if(GPIO_LOW == pin_logic_state)
            {
                *btn_state = BUTTON_RELEASED;
            }
            else
            {
                *btn_state = BUTTON_PRESSED;                
            }             
        }
        
        else
        {
            ret = E_NOT_OK;        
        }
    }
    
    return ret;
}