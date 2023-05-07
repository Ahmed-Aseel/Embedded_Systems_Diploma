/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

Std_ReturnType ret = E_NOT_OK;
//
//void INT0_App_ISR(void)
//{
//    ret = ecu_led_toggle(&led_1);
//}
//
//void INT1_App_ISR(void)
//{
//    ret = ecu_led_toggle(&led_2);
//}
//
//void INT2_App_ISR(void)
//{
//    ret = ecu_led_toggle(&led_3);
//}

//INTx_config_t int0_obj = 
//{
//    .EXT_InterruptHandler = INT0_App_ISR,
//    .source = INTERRUPT_EXTERNAL_INT0,
//    .edge = INTERRUPT_RISING_EDGE,
//    .priority = INTERRUPT_PRIORITY_HIGH,
//    .INTx_pin.port = PORTB_INDEX, 
//    .INTx_pin.pin = GPIO_PIN0,
//    .INTx_pin.direction = GPIO_DIRECTION_INPUT,
//    .INTx_pin.logic = GPIO_LOW
//};
//
//INTx_config_t int1_obj = 
//{
//    .EXT_InterruptHandler = INT1_App_ISR,
//    .source = INTERRUPT_EXTERNAL_INT1,
//    .edge = INTERRUPT_RISING_EDGE,
//    .priority = INTERRUPT_PRIORITY_LOW,
//    .INTx_pin.port = PORTB_INDEX, 
//    .INTx_pin.pin = GPIO_PIN1,
//    .INTx_pin.direction = GPIO_DIRECTION_INPUT,
//    .INTx_pin.logic = GPIO_LOW
//};
//
//INTx_config_t int2_obj = 
//{
//    .EXT_InterruptHandler = INT2_App_ISR,
//    .source = INTERRUPT_EXTERNAL_INT2,
//    .edge = INTERRUPT_RISING_EDGE,
//    .priority = INTERRUPT_PRIORITY_HIGH,
//    .INTx_pin.port = PORTB_INDEX, 
//    .INTx_pin.pin = GPIO_PIN2,
//    .INTx_pin.direction = GPIO_DIRECTION_INPUT,
//    .INTx_pin.logic = GPIO_LOW    
//};

void rb4_intr_App_ISR_Low(void)
{
    ret = ecu_led_turn_off(&led_1);
}

void rb4_intr_App_ISR_High(void)
{
    ret = ecu_led_turn_on(&led_1);
}

void rb5_intr_App_ISR_Low(void)
{
    ret = ecu_led_turn_off(&led_2);
}

void rb5_intr_App_ISR_High(void)
{
    ret = ecu_led_turn_on(&led_2);
}

void rb6_intr_App_ISR_Low(void)
{
    ret = ecu_led_turn_off(&led_3);
}

void rb6_intr_App_ISR_High(void)
{
    ret = ecu_led_turn_on(&led_3);
}

void rb7_intr_App_ISR_Low(void)
{
    ret = ecu_led_turn_off(&led_4);
}

void rb7_intr_App_ISR_High(void)
{
    ret = ecu_led_turn_on(&led_4);
}

RBx_intr_config_t rb4_obj = 
{
    .EXT_InterruptHandler_Low = rb4_intr_App_ISR_Low,
    .EXT_InterruptHandler_High = rb4_intr_App_ISR_High,
    .priority = INTERRUPT_PRIORITY_HIGH,
    .RBx_pin.port = PORTB_INDEX, 
    .RBx_pin.pin = GPIO_PIN4,
    .RBx_pin.direction = GPIO_DIRECTION_INPUT,
    .RBx_pin.logic = GPIO_LOW
};

RBx_intr_config_t rb5_obj = 
{
    .EXT_InterruptHandler_Low = rb5_intr_App_ISR_Low,
    .EXT_InterruptHandler_High = rb5_intr_App_ISR_High,
    .priority = INTERRUPT_PRIORITY_LOW,
    .RBx_pin.port = PORTB_INDEX, 
    .RBx_pin.pin = GPIO_PIN5,
    .RBx_pin.direction = GPIO_DIRECTION_INPUT,
    .RBx_pin.logic = GPIO_LOW
};

RBx_intr_config_t rb6_obj = 
{
    .EXT_InterruptHandler_Low = rb6_intr_App_ISR_Low,
    .EXT_InterruptHandler_High = rb6_intr_App_ISR_High,
    .priority = INTERRUPT_PRIORITY_HIGH,
    .RBx_pin.port = PORTB_INDEX, 
    .RBx_pin.pin = GPIO_PIN6,
    .RBx_pin.direction = GPIO_DIRECTION_INPUT,
    .RBx_pin.logic = GPIO_LOW    
};

RBx_intr_config_t rb7_obj = 
{
    .EXT_InterruptHandler_Low = rb7_intr_App_ISR_Low,
    .EXT_InterruptHandler_High = rb7_intr_App_ISR_High,
    .priority = INTERRUPT_PRIORITY_HIGH,
    .RBx_pin.port = PORTB_INDEX, 
    .RBx_pin.pin = GPIO_PIN7,
    .RBx_pin.direction = GPIO_DIRECTION_INPUT,
    .RBx_pin.logic = GPIO_LOW    
};

int main()
{
    application_initialize();
//    ret = mcal_interrupt_INTx_Init(&int0_obj);
//    ret = mcal_interrupt_INTx_Init(&int1_obj);
//    ret = mcal_interrupt_INTx_Init(&int2_obj);
 
    ret = mcal_interrupt_RBx_Init(&rb4_obj);
    ret = mcal_interrupt_RBx_Init(&rb5_obj);
    ret = mcal_interrupt_RBx_Init(&rb6_obj);    
    ret = mcal_interrupt_RBx_Init(&rb7_obj);    
    
    while(1)
    {

    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
}