/*
* lcd.h
 *
 * Définitions des broches et prototypes pour le pilotage d'un écran LCD
 * compatible Hitachi HD44780 en mode 4 bits.
 *
 * Created on: Jan 5, 2026
 * Author: Lucas
 */


#ifndef LCD_H
#define LCD_H

#include "stm32g0xx_hal.h"
#include <stdio.h>

/* -------------------------------------------------------------------------- */
/*                      Mapping matériel GPIO			                      */
/* -------------------------------------------------------------------------- */

#define LCD_RS_Port GPIOB   // D2
#define LCD_RS_Pin  GPIO_PIN_6

#define LCD_EN_Port GPIOB   // D3
#define LCD_EN_Pin  GPIO_PIN_7

// Bus de données 4 bits (D4 à D7)
#define LCD_D4_Port GPIOB   // D4
#define LCD_D4_Pin  GPIO_PIN_1

#define LCD_D5_Port GPIOB   // D5
#define LCD_D5_Pin  GPIO_PIN_0

#define LCD_D6_Port GPIOB   // D6
#define LCD_D6_Pin  GPIO_PIN_2

#define LCD_D7_Port GPIOB   // D7
#define LCD_D7_Pin  GPIO_PIN_8

/* -------------------------------------------------------------------------- */
/*                         Interface publiques LCD                            */
/* -------------------------------------------------------------------------- */

void lcd_init(void); //Initialise l'écran LCD (Mode 4 bits, curseur off, auto-incrément).
void lcd_send_commande(uint8_t cmd); //Envoi les commandes propres à l'écran LCD
void lcd_show_carac(char c); //Affiche un caractère ASCII sur le LCD.
void lcd_show_txt(const char *str); //Affiche une chaîne de caractères à la position actuelle.
void LCD_PrintFloat(float value, uint8_t col, uint8_t row, const char* label);
void lcd_position_cursor(uint8_t col, uint8_t row); // Positionne le curseur à une coordonnée précise. COL (0-15) et row (0-1)
void lcd_clear(void); // Efface l'écran complet et repositionne le curseur en (0,0).
void lcd_print(uint8_t col, uint8_t row, const char *text);
void lcd_print_int(int value); //Affiche un entier sur le LCD.
void lcd_print_float_1(float value); //Affiche un flottant sur le LCD.

#endif /* LCD_H */
