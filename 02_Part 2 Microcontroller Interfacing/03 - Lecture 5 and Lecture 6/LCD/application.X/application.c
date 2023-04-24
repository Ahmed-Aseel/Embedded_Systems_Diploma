/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

Std_ReturnType ret = E_NOT_OK;

uint8 counter = 0;
//uint8 counter_str[4];

const uint8 customchar1[] = {0x0E, 0x0A, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x00};
const uint8 customchar2[] = {0x0E, 0x0A, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x00};
const uint8 customchar3[] = {0x0E, 0x0A, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x00};
const uint8 customchar4[] = {0x0E, 0x0A, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x00};
const uint8 customchar5[] = {0x0E, 0x0A, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00};
const uint8 customchar6[] = {0x0E, 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00};

int main()
{
    application_initialize();
    
//    ret = ecu_lcd_4bits_send_char_data_pos(&lcd_1, ROW1, 1,  'A');
//    ret = ecu_lcd_4bits_send_char_data_pos(&lcd_1, ROW2, 2,  'B');
//    ret = ecu_lcd_4bits_send_char_data_pos(&lcd_1, ROW3, 3,  'C');
//    ret = ecu_lcd_4bits_send_char_data_pos(&lcd_1, ROW4, 4,  'D');
//    
//    ret = ecu_lcd_8bits_send_char_data_pos(&lcd_2, ROW1, 1,  'A');
//    ret = ecu_lcd_8bits_send_char_data_pos(&lcd_2, ROW2, 2,  'B');
//    ret = ecu_lcd_8bits_send_char_data_pos(&lcd_2, ROW3, 3,  'C');
//    ret = ecu_lcd_8bits_send_char_data_pos(&lcd_2, ROW4, 4,  'D');
    
//    ret = ecu_lcd_4bits_send_string(&lcd_1, "AHMED");
//    ret = ecu_lcd_8bits_send_string(&lcd_2, "AHMED");
//
//    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW2, 2, "ASEEL");
//    ret = ecu_lcd_8bits_send_string_pos(&lcd_2, ROW2, 2, "ASEEL");
    
//    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW2, 1, "Embedded Diploma");
//    ret = ecu_lcd_8bits_send_string_pos(&lcd_2, ROW1, 1, "Loading");
    
    while(1)
    {
//        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, 5, "HELLO ALL");        
//        ret = ecu_lcd_4bits_send_command(&lcd_1, LCD_DISPLAY_SHIFT_RIGHT);
//        __delay_ms(250);
//        ret = ecu_lcd_4bits_send_command(&lcd_1, LCD_DISPLAY_SHIFT_RIGHT);
//        __delay_ms(250);
//        ret = ecu_lcd_4bits_send_command(&lcd_1, LCD_DISPLAY_SHIFT_LEFT);
//        __delay_ms(250);
//        ret = ecu_lcd_4bits_send_command(&lcd_1, LCD_DISPLAY_SHIFT_LEFT);
//        __delay_ms(250);
        
//        for(counter = 5; counter <= 7; counter++)
//        {  
//            ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, counter, "HELLO ALL");    
//            __delay_ms(250);
//            ret = ecu_lcd_4bits_send_char_data_pos(&lcd_1, ROW1, counter,  ' ');
//        }      
//        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, 5,  "            ");
//        
//        for(counter = 7; counter >= 5; counter--)
//        {  
//            ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, counter, "HELLO ALL");    
//            __delay_ms(250);
//            ret = ecu_lcd_4bits_send_char_data_pos(&lcd_1, ROW1, (counter + 8),  ' ');
//        }   
//        
//        for(counter = 8; counter <= 15; counter++)
//        {  
//            ret = ecu_lcd_8bits_send_char_data_pos(&lcd_2, ROW1, counter, '.');    
//            __delay_ms(50);
//        }  
//        ret = ecu_lcd_8bits_send_string_pos(&lcd_2, ROW1, 8,  "        ");
                
//        ret = ecu_lcd_4bits_send_command(&lcd_1, LCD_CLEAR);
//        ret = ecu_lcd_8bits_send_command(&lcd_2, LCD_CLEAR);
//    
//        ret = ecu_lcd_4bits_send_string(&lcd_1, "counter: ");
//        ret = ecu_lcd_8bits_send_string(&lcd_2, "counter: ");
//        
//        for(counter = 98; counter < 105; counter++)
//        {
//            ret = convert_byte_to_string(counter, counter_str);
//            ret = ecu_lcd_4bits_send_string_pos(&lcd_1, 1, 10, counter_str);
//            ret = ecu_lcd_8bits_send_string_pos(&lcd_2, 1, 10, counter_str);     
//            __delay_ms(500);
//        }
        
//            ret = ecu_lcd_4bits_send_custom_char(&lcd_1, ROW1, 1, customchar1, 0);
        ret = ecu_lcd_8bits_send_custom_char(&lcd_2, ROW1, 20, customchar1, 0); 
        __delay_ms(250);
        ret = ecu_lcd_8bits_send_custom_char(&lcd_2, ROW1, 20, customchar2, 0); 
        __delay_ms(250);
        ret = ecu_lcd_8bits_send_custom_char(&lcd_2, ROW1, 20, customchar3, 0);    
        __delay_ms(250);
        ret = ecu_lcd_8bits_send_custom_char(&lcd_2, ROW1, 20, customchar4, 0);    
        __delay_ms(250);
        ret = ecu_lcd_8bits_send_custom_char(&lcd_2, ROW1, 20, customchar5, 0);    
        __delay_ms(250);
        ret = ecu_lcd_8bits_send_custom_char(&lcd_2, ROW1, 20, customchar6, 0);    
        __delay_ms(250);
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
}