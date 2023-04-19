/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

led_config_t led_1 = {.led_pin.port = PORTC_INDEX, .led_pin.pin = GPIO_PIN0,
                      .led_pin.direction = GPIO_DIRECTION_OUTPUT, .led_pin.logic = GPIO_LOW};

led_config_t led_2 = {.led_pin.port = PORTC_INDEX, .led_pin.pin = GPIO_PIN1,
                      .led_pin.direction = GPIO_DIRECTION_OUTPUT, .led_pin.logic = GPIO_LOW};

led_config_t led_3 = {.led_pin.port = PORTC_INDEX, .led_pin.pin = GPIO_PIN2,
                      .led_pin.direction = GPIO_DIRECTION_OUTPUT, .led_pin.logic = GPIO_LOW};

led_config_t led_4 = {.led_pin.port = PORTC_INDEX, .led_pin.pin = GPIO_PIN3,
                      .led_pin.direction = GPIO_DIRECTION_OUTPUT, .led_pin.logic = GPIO_LOW};


button_config_t btn_high = {.button_pin.port = PORTD_INDEX, .button_pin.pin = GPIO_PIN0, 
                            .button_pin.direction = GPIO_DIRECTION_INPUT, .button_pin.logic = GPIO_LOW,
                            .button_connection = BUTTON_ACTIVE_HIGH, .button_state = BUTTON_RELEASED};

button_config_t btn_low = {.button_pin.port = PORTD_INDEX, .button_pin.pin = GPIO_PIN1, 
                            .button_pin.direction = GPIO_DIRECTION_INPUT, .button_pin.logic = GPIO_HIGH,
                            .button_connection = BUTTON_ACTIVE_LOW, .button_state = BUTTON_RELEASED};

Std_ReturnType ret = E_NOT_OK;

button_state_t btn_high_status = BUTTON_RELEASED;
button_state_t btn_high_valid_status = BUTTON_RELEASED;
button_state_t btn_high_last_valid_status = BUTTON_RELEASED;

button_state_t btn_low_status = BUTTON_RELEASED;

led_state_t led1_status = LED_OFF;

uint32 btn_high_valid = 0;
uint32 program_selected = 0;

void program_1(void)
{
     ret = ecu_led_turn_on(&led_1);
    __delay_ms(500);
    ret = ecu_led_turn_off(&led_1); 
}

void program_2(void)
{
    uint8 counter = 0;
    for(counter = 0; counter < 2; counter++)
    {
        ret = ecu_led_turn_on(&led_1);
        __delay_ms(500);
        ret = ecu_led_turn_off(&led_1);    
        __delay_ms(500);
    }
}
void program_3(void)
{
    uint8 counter = 0;
    for(counter = 0; counter < 3; counter++)
    {
        ret = ecu_led_turn_on(&led_1);
        __delay_ms(500);
        ret = ecu_led_turn_off(&led_1);    
        __delay_ms(500);
    }   
}

int main()
{
    application_initialize();

    while(1)
    {
/*        ret = ecu_led_turn_on(&led_1);
        ret = ecu_led_turn_on(&led_2);
        ret = ecu_led_turn_on(&led_3);
        ret = ecu_led_turn_on(&led_4);
        __delay_ms(500);
        ret = ecu_led_turn_off(&led_1);
        ret = ecu_led_turn_off(&led_2);
        ret = ecu_led_turn_off(&led_3);
        ret = ecu_led_turn_off(&led_4);
        __delay_ms(500);
*/

/*        ret = ecu_led_toggle(&led_1);
        ret = ecu_led_toggle(&led_2);
        ret = ecu_led_toggle(&led_3);
        ret = ecu_led_toggle(&led_4);
        __delay_ms(500);
*/
        
/*        ret = ecu_button_read_state(&btn_high, &btn_high_status);
       
        if(BUTTON_PRESSED == btn_high_status)
        {
          ret = gpio_pin_write_logic(&led_1, GPIO_HIGH);      
        }
        else
        {
          ret = gpio_pin_write_logic(&led_1, GPIO_LOW);                  
        }

        ret = ecu_button_read_state(&btn_low, &btn_low_status);
        
        if(BUTTON_PRESSED == btn_low_status)
        {
          ret = gpio_pin_write_logic(&led_2, GPIO_HIGH);      
        }
        else
        {
          ret = gpio_pin_write_logic(&led_2, GPIO_LOW);                  
        }
 */
    
        ret = ecu_button_read_state(&btn_high, &btn_high_status);
       
        if(BUTTON_PRESSED == btn_high_status)
        {
            btn_high_valid++;
            if(btn_high_valid > 500)
            {
                btn_high_valid_status = BUTTON_PRESSED;
            }
        }
        else
        {
            btn_high_valid = 0; 
            btn_high_valid_status = BUTTON_RELEASED;
        }
       
        if(btn_high_last_valid_status != btn_high_valid_status)
        {
            btn_high_last_valid_status = btn_high_valid_status;
            if(BUTTON_PRESSED == btn_high_valid_status)
            {
/*
                if(LED_OFF == led1_status)
                {
                    ecu_led_turn_on(&led_1);
                    led1_status = LED_ON;
                }
                else
                {
                    ecu_led_turn_off(&led_1);                    
                    led1_status = LED_OFF;
                }
*/
                if(3 == program_selected)
                {
                    program_selected = 0;
                }
                
                program_selected++;
        
                switch(program_selected)
                {
                    case 1: program_1(); break;
                    case 2: program_2(); break;
                    case 3: program_3(); break;
                    default: break; /* nothing */
                }
            }   
        }
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ret = ecu_led_initialize(&led_1);
    ret = ecu_led_initialize(&led_2);
    ret = ecu_led_initialize(&led_3);
    ret = ecu_led_initialize(&led_4);   
    
    ret = ecu_button_initialize(&btn_high);
    ret = ecu_button_initialize(&btn_low);   
}