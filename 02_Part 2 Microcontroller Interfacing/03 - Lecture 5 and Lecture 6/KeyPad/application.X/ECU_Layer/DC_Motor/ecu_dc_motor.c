/* 
 * File:   ecu_dc_motor.h
 * Author: Ahmed Aseel
 *
 * Created on April 20, 2023, 3:17 AM
 */

#include "ecu_dc_motor.h"

Std_ReturnType ecu_dc_motor_initialize(const dc_motor_config_t* _dc_motor_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _dc_motor_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_initialize(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN1_INDEX]);
        ret = gpio_pin_initialize(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN2_INDEX]);
    }
        
    return ret;     
}

Std_ReturnType ecu_dc_motor_move_right(const dc_motor_config_t* _dc_motor_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _dc_motor_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_write_logic(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN1_INDEX], GPIO_HIGH);
        ret = gpio_pin_write_logic(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN2_INDEX], GPIO_LOW);
    }
        
    return ret;      
}

Std_ReturnType ecu_dc_motor_move_left(const dc_motor_config_t* _dc_motor_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _dc_motor_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_write_logic(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN1_INDEX], GPIO_LOW);
        ret = gpio_pin_write_logic(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN2_INDEX], GPIO_HIGH);
    }
        
    return ret;      
}

Std_ReturnType ecu_dc_motor_stop(const dc_motor_config_t* _dc_motor_config)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _dc_motor_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {   
        ret = gpio_pin_initialize(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN1_INDEX]);
        ret = gpio_pin_initialize(&_dc_motor_config->dc_motor_pins[DC_MOTOR_PIN2_INDEX]);
    }
        
    return ret;      
}
