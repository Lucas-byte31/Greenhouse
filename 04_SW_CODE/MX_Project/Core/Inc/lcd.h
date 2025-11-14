/*
 * lcd.h
 *
 *  Created on: Nov 14, 2025
 *      Author: basti
 */



/* Definir les broches de contr�le de l'�cran LCD*/
#define RS PB6
#define EN PB7
#define D4 PB1
#define D5 PB0
#define D6 PB2
#define D7 PB8
/* Fonctions de delai*/
void Lcd_Delay(unsigned int time) {
    while(time--);
}

void Lcd_Cmd(char cmd) {
    RS = 0; // Mode commande
    D7 = (cmd >> 3) & 1;
    D6 = (cmd >> 2) & 1;
    D5 = (cmd >> 1) & 1;
    D4 = cmd & 1;
    EN = 1;
    Lcd_Delay(500);
    EN = 0;
}

void Lcd_Write_Char(char data) {
    RS = 1; // Mode donn�es
    D7 = (data >> 3) & 1;
    D6 = (data >> 2) & 1;
    D5 = (data >> 1) & 1;
    D4 = data & 1;
    EN = 1;
    Lcd_Delay(500);
    EN = 0;
}

void Lcd_Write_String(char *str) {
    while(*str) {
        Lcd_Write_Char(*str++);
    }
}

void Lcd_Set_Cursor(char row, char column) {
    char temp, low4, high4;
    if(row == 1) {
        temp = 0x80 + column - 1;
        high4 = temp >> 4;
        low4 = temp & 0x0F;
        Lcd_Cmd(high4);
        Lcd_Cmd(low4);
    } else if(row == 2) {
        temp = 0xC0 + column - 1;
        high4 = temp >> 4;
        low4 = temp & 0x0F;
        Lcd_Cmd(high4);
        Lcd_Cmd(low4);
    }
}

void Lcd_Clear(void) {
    Lcd_Cmd(0x01);
}

void Lcd_Init(void) {
    D4 = D5 = D6 = D7 = 0;
    __delay_ms(20);
    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_ms(11);
    Lcd_Cmd(0x03);
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x08);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x06);
}

#ifndef INC_LCD_H_
#define INC_LCD_H_



#endif /* INC_LCD_H_ */
