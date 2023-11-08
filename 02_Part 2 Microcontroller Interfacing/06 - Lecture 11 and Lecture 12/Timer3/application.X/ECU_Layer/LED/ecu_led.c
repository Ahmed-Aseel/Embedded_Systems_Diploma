/* 
 * File:   ecu_led.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 7:16 PM
 */

#include "ecu_led.h"

/**
 * 
 * @param _led_config
 * @return 
 */
Std_ReturnType ecu_led_initialize(const led_config_t* _led_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _led_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {      
        ret = gpio_pin_initialize(&_led_config->led_pin);
    }
        
    return ret;    
}

/**
 * 
 * @param _led_config
 * @return 
 */
Std_ReturnType ecu_led_turn_off(const led_config_t* _led_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _led_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_write_logic(&_led_config->led_pin, GPIO_LOW);
    }
        
    return ret;     
}

/**
 * 
 * @param _led_config
 * @return 
 */
Std_ReturnType ecu_led_turn_on(const led_config_t* _led_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _led_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_write_logic(&_led_config->led_pin, GPIO_HIGH);
    }
        
    return ret;     
}

/**
 * 
 * @param _led_config
 * @return 
 */
Std_ReturnType ecu_led_toggle(const led_config_t* _led_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _led_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_toggle_logic(&_led_config->led_pin);
    }
        
    return ret;     
}