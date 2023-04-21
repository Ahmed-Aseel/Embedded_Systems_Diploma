/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

//uint8 seg_common_anode_valus[]   = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
//uint8 seg_common_cathode_valus[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
//uint8 ones = 0, tens = 0;

//uint8 number = 0;

segment_config_t seg_1 = {
  .segment_pins[SEGMENT_PIN1_INDEX].port = PORTC_INDEX,
  .segment_pins[SEGMENT_PIN1_INDEX].pin  = GPIO_PIN0,
  .segment_pins[SEGMENT_PIN1_INDEX].direction = GPIO_DIRECTION_OUTPUT,
  .segment_pins[SEGMENT_PIN1_INDEX].logic = GPIO_LOW,

  .segment_pins[SEGMENT_PIN2_INDEX].port = PORTC_INDEX,
  .segment_pins[SEGMENT_PIN2_INDEX].pin  = GPIO_PIN1,
  .segment_pins[SEGMENT_PIN2_INDEX].direction = GPIO_DIRECTION_OUTPUT,
  .segment_pins[SEGMENT_PIN2_INDEX].logic = GPIO_LOW,

  .segment_pins[SEGMENT_PIN3_INDEX].port = PORTC_INDEX,
  .segment_pins[SEGMENT_PIN3_INDEX].pin  = GPIO_PIN2,
  .segment_pins[SEGMENT_PIN3_INDEX].direction = GPIO_DIRECTION_OUTPUT,
  .segment_pins[SEGMENT_PIN3_INDEX].logic = GPIO_LOW,

  .segment_pins[SEGMENT_PIN4_INDEX].port = PORTC_INDEX,
  .segment_pins[SEGMENT_PIN4_INDEX].pin  = GPIO_PIN3,
  .segment_pins[SEGMENT_PIN4_INDEX].direction = GPIO_DIRECTION_OUTPUT,
  .segment_pins[SEGMENT_PIN4_INDEX].logic = GPIO_LOW,
  
  .segment_type = SEGMENT_COMMON_ANODE
};

uint8 hours = 23, minutes = 59, seconds = 50, counter = 0;

Std_ReturnType ret = E_NOT_OK;

int main()
{
    application_initialize();

    while(1)
    {
//        for(tens = 0; tens < 10; tens++)
//        {
//            ret = gpio_port_write_logic(PORTD_INDEX, seg_common_anode_valus[tens]);
//        
//            for(ones = 0; ones < 10; ones++)
//            {
//                ret = gpio_port_write_logic(PORTC_INDEX, seg_common_anode_valus[ones]);
//                __delay_ms(250);
//            }        
//        }

//        for(number = 0; number < 10; number++)
//        {
//            ret = ecu_7_segment_write_number(&seg_1, number);  
//            __delay_ms(250);
//        }
        
        for(counter = 0; counter < 50; counter++)
        {
            ret = gpio_port_write_logic(PORTD_INDEX, 0x3E);    
            ret = gpio_port_write_logic(PORTC_INDEX, (uint8)(hours / 10));  
            __delay_us(3333);
        
            ret = gpio_port_write_logic(PORTD_INDEX, 0x3D);    
            ret = gpio_port_write_logic(PORTC_INDEX, (uint8)(hours % 10));  
            __delay_us(3333);
        
            ret = gpio_port_write_logic(PORTD_INDEX, 0x3B);    
            ret = gpio_port_write_logic(PORTC_INDEX, (uint8)(minutes / 10));  
            __delay_us(3333);
        
            ret = gpio_port_write_logic(PORTD_INDEX, 0x37);    
            ret = gpio_port_write_logic(PORTC_INDEX, (uint8)(minutes % 10));  
            __delay_us(3333);
        
            ret = gpio_port_write_logic(PORTD_INDEX, 0x2F);    
            ret = gpio_port_write_logic(PORTC_INDEX, (uint8)(seconds / 10));  
            __delay_us(3333);
        
            ret = gpio_port_write_logic(PORTD_INDEX, 0x1F);    
            ret = gpio_port_write_logic(PORTC_INDEX, (uint8)(seconds % 10));  
            __delay_us(3333);        
        }
        
        seconds++;
        if(seconds == 60)
        {
            seconds = 0;
            minutes++;
        }
        if(minutes == 60)
        {
            minutes = 0;
            hours++;
        }
        if(hours == 24)
        {
            hours = 0;
        }
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
//    ret = gpio_port_direction_initialize(PORTC_INDEX, 0x80);
//    ret = gpio_port_direction_initialize(PORTD_INDEX, 0x80);
 
//    ret = gpio_port_write_logic(PORTC_INDEX, seg_common_anode_valus[0]);
//    ret = gpio_port_write_logic(PORTD_INDEX, seg_common_anode_valus[0]);    

    ret = ecu_7_segment_initialize(&seg_1);
    ret = gpio_port_direction_initialize(PORTD_INDEX, 0xC0);
}