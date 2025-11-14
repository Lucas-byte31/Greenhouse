#include "stm32g0xx_hal.h"

// Définition des broches LCD
#define RS_PIN GPIO_PIN_6
#define EN_PIN GPIO_PIN_7
#define D4_PIN GPIO_PIN_1
#define D5_PIN GPIO_PIN_0
#define D6_PIN GPIO_PIN_2
#define D7_PIN GPIO_PIN_8
#define LCD_PORT GPIOB // Toutes les broches sont sur GPIOB

// Fonction de délai (remplace le while(time--) par HAL_Delay)
void Lcd_Delay(uint32_t time_us) {
    HAL_Delay(time_us / 1000); // Convertit en millisecondes
}

// Écriture des 4 bits sur les broches D4 à D7
void Lcd_Set_Data(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 0) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 3) & 0x01);
}

// Envoi d'une commande à l'écran
void Lcd_Cmd(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET); // RS = 0 (commande)
    Lcd_Set_Data(cmd);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    Lcd_Delay(500);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
}

// Envoi d'un caractère à afficher
void Lcd_Write_Char(char data) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET); // RS = 1 (donnée)
    Lcd_Set_Data(data);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    Lcd_Delay(500);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
}

// Affichage d'une chaîne de caractères
void Lcd_Write_String(char *str) {
    while (*str) {
        Lcd_Write_Char(*str++);
    }
}

// Positionnement du curseur
void Lcd_Set_Cursor(uint8_t row, uint8_t column) {
    uint8_t address;

    if (row == 1) {
        address = 0x80 + (column - 1); // Ligne 1
    } else if (row == 2) {
        address = 0xC0 + (column - 1); // Ligne 2
    } else {
        return; // Ligne invalide
    }

    // Envoie l'adresse en deux parties (mode 4 bits)
    Lcd_Cmd(address >> 4);     // Partie haute
    Lcd_Cmd(address & 0x0F);   // Partie basse
}


// Effacement de l'écran
void Lcd_Clear(void) {
    Lcd_Cmd(0x01);
}

// Initialisation de l'écran LCD
void Lcd_Init(void) {
	HAL_GPIO_WritePin(GPIOB, D4_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, D5_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, D6_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, D7_PIN, GPIO_PIN_RESET);

    HAL_Delay(20);
    Lcd_Cmd(0x03);
    HAL_Delay(5);
    Lcd_Cmd(0x03);
    HAL_Delay(11);
    Lcd_Cmd(0x03);
    Lcd_Cmd(0x02); // Mode 4 bits

    // Configuration de l'affichage
    Lcd_Cmd(0x02); // 4 bits
    Lcd_Cmd(0x08); // 2 lignes, 5x8 points
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0C); // Affichage ON, curseur OFF
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x06); // Incrément automatique
}
