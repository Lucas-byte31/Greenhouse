/*
 * lcd.c
 *
 * Implémentation du protocole de communication LCD.
 * Le mode 4 bits est utilisé pour économiser des broches GPIO sur le STM32.
 *
 *  Created on: Nov 27, 2025
 *      Author: lucas
 */
#include "lcd.h"

/* -------------------------------------------------------------------------- */
/*                          Fonctions internes (static)                       */
/* -------------------------------------------------------------------------- */
// Attente temporelle non-bloquante.
static void lcd_wait_ms(uint32_t delay)
{
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < delay)
    {
        // attente non bloquante (CPU libre)
    }
}

/* Envoie une impulsion sur la ligne EN */
static void lcd_send_pulse(void)
{
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_SET);
    lcd_wait_ms(1);
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    lcd_wait_ms(1);
}

/* Envoie un nibble (4 bits) sur les broches D4-D7 */
// on place les 4 bits sur les lignes D4-D7
// puis on génère une impulsion sur la ligne Enable (EN).
static void lcd_send_nibble(uint8_t nibble)
{
    HAL_GPIO_WritePin(LCD_D4_Port, LCD_D4_Pin, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_Port, LCD_D5_Pin, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_Port, LCD_D6_Pin, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_Port, LCD_D7_Pin, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    lcd_send_pulse();
}

/* Envoie un octet (commande ou donnée) */
// Envoie un octet complet (8 bits) en deux étapes.
// rs : 0 pour une commande, 1 pour une donnée (caractère).
static void lcd_send_octet(uint8_t value, uint8_t rs)
{
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, rs);

    // On envoie d'abord les 4 bits de poids fort (MSB)
    lcd_send_nibble(value >> 4);     // nibble haut
    // Puis les 4 bits de poids faible (LSB)
    lcd_send_nibble(value & 0x0F);   // nibble bas
}

/* -------------------------------------------------------------------------- */
/*                          Fonctions publiques LCD                           */
/* -------------------------------------------------------------------------- */

/* Envoie une commande */
void lcd_send_commande(uint8_t cmd)
{
    lcd_send_octet(cmd, 0);
    lcd_wait_ms(2);
}

/* Efface l'écran */
void lcd_clear(void)
{
    lcd_send_commande(0x01);
    lcd_wait_ms(2);
}

/* Initialisation du LCD en mode 4 bits */
// Respecte la séquence de démarrage imposée par la datasheet
void lcd_init(void)
{
	lcd_wait_ms(40);   // Attente après alimentation

    // Séquence spéciale pour entrer en mode 4 bits
    lcd_send_nibble(0x03); lcd_wait_ms(5); // Attente stabilisation tension
    lcd_send_nibble(0x03); lcd_wait_ms(5);
    lcd_send_nibble(0x03); lcd_wait_ms(1);
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
    snprintf(buffer, sizeof(buffer), "%s %.1f C", label, value);
    lcd_position_cursor(col, row);
    lcd_show_txt(buffer);
}


/* Positionne le curseur (col 0-15, row 0-1) */
void lcd_position_cursor(uint8_t col, uint8_t row)
{
    uint8_t offsets[] = {0x00, 0x40};
    lcd_send_commande(0x80 | (col + offsets[row]));
}

// fonction inline pour afficher une ligne du LCD
void lcd_print(uint8_t col, uint8_t row, const char *text)
{
    lcd_position_cursor(col, row);
    lcd_show_txt(text);
}

// print sur le LCD sans passer par une fonction printf ou sprintf ou snprintf
//Pour les entiers
void lcd_print_int(int value)
{
	// Buffer local pour stocker les chiffres (max 5 chiffres + signe + \0 pour un int16)
    char buf[7]; // -32768\0
    int i = 0;

    // --- Gestion du signe ---
    if (value < 0) {
        lcd_show_carac('-'); // Affiche le signe moins immédiatement
        value = -value; // Travaille ensuite avec la valeur absolue
    }

    // Algorithme de décomposition (Modulo/Division)
    // On extrait les chiffres un par un en partant des unités
    do {
    	// '0' + reste de la division par 10 transforme le chiffre en caractère ASCII
        buf[i++] = '0' + (value % 10);
        value /= 10;
    } while (value && i < sizeof(buf)); // Continue tant qu'il y a des chiffres


    //  Inversion et affichage
     // Les chiffres ayant été extraits à l'envers (unités d'abord),
    // on parcourt le buffer en sens inverse pour les envoyer au LCD.
    while (i--) {
        lcd_show_carac(buf[i]);
    }
}
//Pour les FLoats sans passer par le printf
void lcd_print_float_1(float value)
{
    int iv = (int)value;
    int dec = (int)((value - iv) * 10);
    if (dec < 0) dec = -dec;

    lcd_print_int(iv);
    lcd_show_carac('.');
    lcd_show_carac('0' + dec);
}
