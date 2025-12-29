#ifndef LCD_H
#define LCD_H

#include "stm32g0xx_hal.h"

/* -------------------------------------------------------------------------- */
/*                      Définition des broches du LCD                         */
/* -------------------------------------------------------------------------- */

#define LCD_RS_Port GPIOB   // D2
#define LCD_RS_Pin  GPIO_PIN_6

#define LCD_EN_Port GPIOB   // D3
#define LCD_EN_Pin  GPIO_PIN_7

#define LCD_D4_Port GPIOB   // D4
#define LCD_D4_Pin  GPIO_PIN_1

#define LCD_D5_Port GPIOB   // D5
#define LCD_D5_Pin  GPIO_PIN_0

#define LCD_D6_Port GPIOB   // D6
#define LCD_D6_Pin  GPIO_PIN_2

#define LCD_D7_Port GPIOB   // D7
#define LCD_D7_Pin  GPIO_PIN_8

/* -------------------------------------------------------------------------- */
/*                         Fonctions publiques LCD                             */
/* -------------------------------------------------------------------------- */

void lcd_init(void);
void lcd_send_commande(uint8_t cmd);
void lcd_show_carac(char c);
void lcd_show_txt(const char *str);
void LCD_PrintFloat(float value, uint8_t col, uint8_t row, const char* label);
void lcd_position_cursor(uint8_t col, uint8_t row);
void lcd_clear(void);

#endif /* LCD_H */
