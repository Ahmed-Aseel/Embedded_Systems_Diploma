/* 
 * File:   ecu_lcd.h
 * Author: Ahmed Aseel
 *
 * Created on April 23, 2023, 4:08 PM
 */

#ifndef ECU_LCD_H
#define	ECU_LCD_H

/* Section: Includes */
#include "ecu_lcd_cfg.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"

/* Section: Macro Declarations */
#define LCD_CLEAR                    0x01
#define LCD_RETURN_HOME              0x02
#define LCD_ENTRY_MODE               0X06
#define LCD_CURSOR_OFF_DISPLAY_ON    0x0C
#define LCD_CURSOR_OFF_DISPLAY_OFF   0x08
#define LCD_CURSOR_ON_BLINK_ON       0x0F
#define LCD_CURSOR_ON_BLINK_OFF      0x0E
#define LCD_DISPLAY_SHIFT_RIGHT      0x1C
#define LCD_DISPLAY_SHIFT_LEFT       0x18
#define LCD_8BITS_MODE_2LINES        0x38
#define LCD_4BITS_MODE_2LINES        0x28
#define LCD_CGRAM_START              0x40
#define LCD_DDRAM_START              0x80

#define ROW1                         1
#define ROW2                         2
#define ROW3                         3
#define ROW4                         4

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */
typedef struct {
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data_pins[4];
}lcd_4bits_config_t;

typedef struct {
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data_pins[8];
}lcd_8bits_config_t;

/* Section: Function Declarations */
Std_ReturnType ecu_lcd_4bits_initialize(const lcd_4bits_config_t* _lcd_4bits_config);
Std_ReturnType ecu_lcd_4bits_send_command(const lcd_4bits_config_t* _lcd_4bits_config, uint8 command);
Std_ReturnType ecu_lcd_4bits_send_char_data(const lcd_4bits_config_t* _lcd_4bits_config, uint8 data);
Std_ReturnType ecu_lcd_4bits_send_char_data_pos(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col, uint8 data);
Std_ReturnType ecu_lcd_4bits_send_string(const lcd_4bits_config_t* _lcd_4bits_config, uint8* str);
Std_ReturnType ecu_lcd_4bits_send_string_pos(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col, uint8* str);
Std_ReturnType ecu_lcd_4bits_send_custom_char(const lcd_4bits_config_t* _lcd_4bits_config, uint8 row, uint8 col,
                                              const uint8 _char[], uint8 mem_pos);

Std_ReturnType ecu_lcd_8bits_initialize(const lcd_8bits_config_t* _lcd_8bits_config);
Std_ReturnType ecu_lcd_8bits_send_command(const lcd_8bits_config_t* _lcd_8bits_config, uint8 command);
Std_ReturnType ecu_lcd_8bits_send_char_data(const lcd_8bits_config_t* _lcd_8bits_config, uint8 data);
Std_ReturnType ecu_lcd_8bits_send_char_data_pos(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col, uint8 data);
Std_ReturnType ecu_lcd_8bits_send_string(const lcd_8bits_config_t* _lcd_8bits_config, uint8* str);
Std_ReturnType ecu_lcd_8bits_send_string_pos(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col, uint8* str);
Std_ReturnType ecu_lcd_8bits_send_custom_char(const lcd_8bits_config_t* _lcd_8bits_config, uint8 row, uint8 col,
                                              const uint8 _char[], uint8 mem_pos);

Std_ReturnType convert_uint8_to_string(uint8 value, uint8* str);
Std_ReturnType convert_uint16_to_string(uint16 value, uint8* str);
Std_ReturnType convert_uint32_to_string(uint32 value, uint8* str);

#endif	/* ECU_LCD_H */

