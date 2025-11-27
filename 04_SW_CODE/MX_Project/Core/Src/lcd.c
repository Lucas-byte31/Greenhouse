/*
 * lcd.c
 *
 *  Created on: Nov 27, 2025
 *      Author: lucas
 */
#include "lcd.h"

/* -------------------------------------------------------------------------- */
/*                          Fonctions internes (static)                       */
/* -------------------------------------------------------------------------- */

/* Envoie une impulsion sur la ligne EN */
static void lcd_send_pulse(void)
{
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

/* Écrit un nibble (4 bits) sur les broches D4-D7 */
static void lcd_send_nibble(uint8_t nibble)
{
    HAL_GPIO_WritePin(LCD_D4_Port, LCD_D4_Pin, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_Port, LCD_D5_Pin, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_Port, LCD_D6_Pin, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_Port, LCD_D7_Pin, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    lcd_send_pulse();
}

/* Envoie un octet (commande ou donnée) */
static void lcd_send_octet(uint8_t value, uint8_t rs)
{
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, rs);

    lcd_send_nibble(value >> 4);     // nibble haut
    lcd_send_nibble(value & 0x0F);   // nibble bas
}

/* -------------------------------------------------------------------------- */
/*                          Fonctions publiques LCD                           */
/* -------------------------------------------------------------------------- */

/* Envoie une commande */
void lcd_send_commande(uint8_t cmd)
{
    lcd_send_octet(cmd, 0);
    HAL_Delay(2);
}

/* Efface l'écran */
void lcd_clear(void)
{
    lcd_send_commande(0x01);
    HAL_Delay(2);
}

/* Initialisation du LCD en mode 4 bits */
void lcd_init(void)
{
    HAL_Delay(40);   // Attente après alimentation

    // Séquence spéciale pour entrer en mode 4 bits
    lcd_send_nibble(0x03); HAL_Delay(5);
    lcd_send_nibble(0x03); HAL_Delay(5);
    lcd_send_nibble(0x03); HAL_Delay(1);
    lcd_send_nibble(0x02); // passage mode 4 bits

    lcd_send_commande(0x28); // 4 bits, 2 lignes, matrice 5x8
    lcd_send_commande(0x0C); // écran ON, curseur OFF
    lcd_clear();
    lcd_send_commande(0x06); // auto-incrément du curseur
}

/* Affiche 1 caractère */
void lcd_show_carac(char c)
{
    lcd_send_octet(c, 1);
}

/* Affiche une chaîne */
void lcd_show_txt(const char *str)
{
    while (*str)
    {
        lcd_show_carac(*str++);
    }
}

/* LCD print float*/
void LCD_PrintFloat(float value, uint8_t col, uint8_t row, const char* label)
{
    char buffer[20];
    sprintf(buffer, "%s %.1f C", label, value);
    lcd_position_cursor(col, row);
    lcd_show_txt(buffer);
}


/* Positionne le curseur (col 0-15, row 0-1) */
void lcd_position_cursor(uint8_t col, uint8_t row)
{
    uint8_t offsets[] = {0x00, 0x40};
    lcd_send_commande(0x80 | (col + offsets[row]));
}
