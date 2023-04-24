/* 
 * File:   ecu_lcd.c
 * Author: Ahmed Aseel
 *
 * Created on April 23, 2023, 4:08 PM
 */

#include "ecu_lcd.h"

static Std_ReturnType lcd_send_4bits(const lcd_4bits_config_t* _lcd_4bits_config, uint8 data_command);
static Std_ReturnType lcd_4bits_send_enable_signal(const lcd_4bits_config_t* _lcd_4bits_config);
static Std_ReturnType lcd_4bits_set_cursor(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col);

static Std_ReturnType lcd_8bits_send_enable_signal(const lcd_8bits_config_t* _lcd_8bits_config);
static Std_ReturnType lcd_8bits_set_cursor(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col);


Std_ReturnType ecu_lcd_4bits_initialize(const lcd_4bits_config_t* _lcd_4bits_config)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    
    if(NULL == _lcd_4bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = gpio_pin_initialize(&_lcd_4bits_config->lcd_rs);
        ret = gpio_pin_initialize(&_lcd_4bits_config->lcd_en);
        
        for(counter = 0; counter < 4; counter++)
        {
            ret = gpio_pin_initialize(&_lcd_4bits_config->lcd_data_pins[counter]);
        }
        
        __delay_ms(20);
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_8BITS_MODE_2LINES);
        __delay_ms(5);
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_8BITS_MODE_2LINES);
        __delay_us(150);
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_8BITS_MODE_2LINES);        

        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_CLEAR);        
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_RETURN_HOME);        
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_ENTRY_MODE);        
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_CURSOR_OFF_DISPLAY_ON);        
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_4BITS_MODE_2LINES); 
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, LCD_DDRAM_START); 
    }
        
    return ret;    
}

Std_ReturnType ecu_lcd_4bits_send_command(const lcd_4bits_config_t* _lcd_4bits_config, uint8 command)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_4bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_rs, GPIO_LOW);
        
        ret = lcd_send_4bits(_lcd_4bits_config, (command >> 4) & 0x0F);
        ret = lcd_4bits_send_enable_signal(_lcd_4bits_config);
        
        ret = lcd_send_4bits(_lcd_4bits_config, command & 0x0F);
        ret = lcd_4bits_send_enable_signal(_lcd_4bits_config);
    }
        
    return ret;      
}

Std_ReturnType ecu_lcd_4bits_send_char_data(const lcd_4bits_config_t* _lcd_4bits_config, uint8 data)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_4bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_rs, GPIO_HIGH);
        
        ret = lcd_send_4bits(_lcd_4bits_config, (data >> 4) & 0x0F);
        ret = lcd_4bits_send_enable_signal(_lcd_4bits_config);
        
        ret = lcd_send_4bits(_lcd_4bits_config, data & 0x0F);
        ret = lcd_4bits_send_enable_signal(_lcd_4bits_config);
    }
        
    return ret;      
}

Std_ReturnType ecu_lcd_4bits_send_char_data_pos(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col, uint8 data)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_4bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = lcd_4bits_set_cursor(_lcd_4bits_config, row, col);
        ret = ecu_lcd_4bits_send_char_data(_lcd_4bits_config, data);
    }
        
    return ret;      
}

Std_ReturnType ecu_lcd_4bits_send_string(const lcd_4bits_config_t* _lcd_4bits_config, uint8* str)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_4bits_config ||  NULL == str) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        while(*str)
        {
            ret = ecu_lcd_4bits_send_char_data(_lcd_4bits_config, *str);
            str++;
        }
    }
        
    return ret;      
}

Std_ReturnType ecu_lcd_4bits_send_string_pos(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col, uint8* str)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_4bits_config ||  NULL == str) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = lcd_4bits_set_cursor(_lcd_4bits_config, row, col);
        ret = ecu_lcd_4bits_send_string(_lcd_4bits_config, str);
    }
        
    return ret;     
}

Std_ReturnType ecu_lcd_4bits_send_custom_char(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col,
                                              const uint8 _char[], uint8 mem_pos)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    
    if(NULL == _lcd_4bits_config ||  NULL == _char) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, (LCD_CGRAM_START + (mem_pos * 8)));
        
        for(counter = 0; counter < 8; counter++)
        {
            ret = ecu_lcd_4bits_send_char_data(_lcd_4bits_config, _char[counter]);
        }
        
        ret = ecu_lcd_4bits_send_char_data_pos(_lcd_4bits_config, row, col, mem_pos);
    }
        
    return ret;     
}


Std_ReturnType ecu_lcd_8bits_initialize(const lcd_8bits_config_t* _lcd_8bits_config)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    
    if(NULL == _lcd_8bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = gpio_pin_initialize(&_lcd_8bits_config->lcd_rs);
        ret = gpio_pin_initialize(&_lcd_8bits_config->lcd_en);
        
        for(counter = 0; counter < 8; counter++)
        {
            ret = gpio_pin_initialize(&_lcd_8bits_config->lcd_data_pins[counter]);
        }      
        
        __delay_ms(20);
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_8BITS_MODE_2LINES);
        __delay_ms(5);
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_8BITS_MODE_2LINES);
        __delay_us(150);
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_8BITS_MODE_2LINES);        

        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_CLEAR);        
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_RETURN_HOME);        
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_ENTRY_MODE);        
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_CURSOR_OFF_DISPLAY_ON);        
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_8BITS_MODE_2LINES);        
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, LCD_DDRAM_START); 
    }
        
    return ret;     
}

Std_ReturnType ecu_lcd_8bits_send_command(const lcd_8bits_config_t* _lcd_8bits_config, uint8 command)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    
    if(NULL == _lcd_8bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = gpio_pin_write_logic(&_lcd_8bits_config->lcd_rs, GPIO_LOW);
        
        for(counter = 0; counter < 8; counter++)
        {
            ret = gpio_pin_write_logic(&_lcd_8bits_config->lcd_data_pins[counter], (command >> counter) & 0x01);
        }
        
        ret = lcd_8bits_send_enable_signal(_lcd_8bits_config);
    }
        
    return ret;     
}

Std_ReturnType ecu_lcd_8bits_send_char_data(const lcd_8bits_config_t* _lcd_8bits_config, uint8 data)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    
    if(NULL == _lcd_8bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = gpio_pin_write_logic(&_lcd_8bits_config->lcd_rs, GPIO_HIGH);
        
        for(counter = 0; counter < 8; counter++)
        {
            ret = gpio_pin_write_logic(&_lcd_8bits_config->lcd_data_pins[counter], (data >> counter) & 0x01);
        }   
        
        ret = lcd_8bits_send_enable_signal(_lcd_8bits_config);
    }
        
    return ret;     
}

Std_ReturnType ecu_lcd_8bits_send_char_data_pos(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col, uint8 data)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_8bits_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = lcd_8bits_set_cursor(_lcd_8bits_config, row, col);
        ret = ecu_lcd_8bits_send_char_data(_lcd_8bits_config, data);
    }
        
    return ret;     
}

Std_ReturnType ecu_lcd_8bits_send_string(const lcd_8bits_config_t* _lcd_8bits_config, uint8* str)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_8bits_config || NULL == str) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        while(*str)
        {
            ret = ecu_lcd_8bits_send_char_data(_lcd_8bits_config, *str);
            str++;
        }
    }
        
    return ret;     
}

Std_ReturnType ecu_lcd_8bits_send_string_pos(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col, uint8* str)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _lcd_8bits_config || NULL == str) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = lcd_8bits_set_cursor(_lcd_8bits_config, row, col);
        ret = ecu_lcd_8bits_send_string(_lcd_8bits_config, str);        
    }
        
    return ret;     
}

Std_ReturnType ecu_lcd_8bits_send_custom_char(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col,
                                              const uint8 _char[], uint8 mem_pos)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    
    if(NULL == _lcd_8bits_config  ||  NULL == _char) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, (LCD_CGRAM_START + (mem_pos * 8)));
        
        for(counter = 0; counter < 8; counter++)
        {
            ret = ecu_lcd_8bits_send_char_data(_lcd_8bits_config, _char[counter]);
        }
        
        ret = ecu_lcd_8bits_send_char_data_pos(_lcd_8bits_config, row, col, mem_pos);
    }
        
    return ret;     
}

Std_ReturnType convert_byte_to_string(uint8 value, uint8* str)
{
    Std_ReturnType ret = E_OK;

    if(NULL == str) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        memset(str, '\0', 4);
        sprintf(str, "%i", value);
    }    
            
    return ret;      
}

Std_ReturnType convert_short_to_string(uint16 value, uint8* str)
{
    Std_ReturnType ret = E_OK;

    if(NULL == str) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        memset(str, '\0', 6);
        sprintf(str, "%i", value);
    }     
        
    return ret;      
}

Std_ReturnType convert_int_to_string(uint32 value, uint8* str)
{
    Std_ReturnType ret = E_OK;

    if(NULL == str) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        memset(str, '\0', 11);
        sprintf(str, "%i", value);
    }     
        
    return ret;      
}

static Std_ReturnType lcd_send_4bits(const lcd_4bits_config_t* _lcd_4bits_config, uint8 data_command)
{
    Std_ReturnType ret = E_OK;
    
    ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_data_pins[0], (data_command >> 0) & 0x01);
    ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_data_pins[1], (data_command >> 1) & 0x01);
    ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_data_pins[2], (data_command >> 2) & 0x01);
    ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_data_pins[3], (data_command >> 3) & 0x01);
 
    return ret;      
}

static Std_ReturnType lcd_4bits_send_enable_signal(const lcd_4bits_config_t* _lcd_4bits_config)
{
    Std_ReturnType ret = E_OK;

    ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_en, GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&_lcd_4bits_config->lcd_en, GPIO_LOW);
        
    return ret;      
}

static Std_ReturnType lcd_4bits_set_cursor(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col)
{
    Std_ReturnType ret = E_OK;
    col--;
    
    switch(row)
    {
        case ROW1:
            ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, (0x80 + col));   
            break;
            
        case ROW2:
            ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, (0xC0 + col));   
            break;
            
        case ROW3:
            ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, (0x94 + col));   
            break;
            
        case ROW4:
            ret = ecu_lcd_4bits_send_command(_lcd_4bits_config, (0xD4 + col));   
            break;

        default:
            ret = E_NOT_OK;
    }
    
    return ret;    
}

static Std_ReturnType lcd_8bits_send_enable_signal(const lcd_8bits_config_t* _lcd_8bits_config)
{
    Std_ReturnType ret = E_OK;

    ret = gpio_pin_write_logic(&_lcd_8bits_config->lcd_en, GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&_lcd_8bits_config->lcd_en, GPIO_LOW);
        
    return ret;       
}

static Std_ReturnType lcd_8bits_set_cursor(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col)
{
    Std_ReturnType ret = E_OK;
    col--;
    
    switch(row)
    {
        case ROW1:
            ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, (0x80 + col));   
            break;
            
        case ROW2:
            ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, (0xC0 + col));   
            break;
            
        case ROW3:
            ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, (0x94 + col));   
            break;
            
        case ROW4:
            ret = ecu_lcd_8bits_send_command(_lcd_8bits_config, (0xD4 + col));   
            break;

        default:
            ret = E_NOT_OK;
    }
    
    return ret;
}