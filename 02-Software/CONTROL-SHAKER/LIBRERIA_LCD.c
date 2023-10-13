#include <xc.h>
#include "LIBRERIA_LCD.h"
#include "config.h"

void LCD_Init(void){
    //configuramos el puerto de salida
    TRIS_RS=0;
    TRIS_E=0;
    TRIS_D4=0;
    TRIS_D5=0;
    TRIS_D6=0;
    TRIS_D7=0;
    PIN_RS=0;  // para enviar datos en modo 0 
    __delay_ms(15);
    //iniciar secuencia se sincronia
    enviar_dato(0x30);
    __delay_ms(5);
    enviar_dato(0x30);
    __delay_us(100);
    enviar_dato(0x32);
    __delay_us(100);
    
    //configuramos la lcd
    __delay_us(100);
    enviar_dato(0x2B);
    __delay_us(100);
    enviar_dato(0x08);
    __delay_us(100);
    enviar_dato(0x0F);
    __delay_us(100);
    enviar_dato(0x01);
    __delay_us(100);
    enviar_dato(0x06);
    __delay_us(100);
    
    enviar_dato(0x0F);
    __delay_us(100);
    
    return;
}

void enviar_dato(unsigned char cmd){
    TRIS_D4=0;
    TRIS_D5=0;
    TRIS_D6=0;
    TRIS_D7=0;
    PIN_D4=0;
    PIN_D5=0;
    PIN_D6=0;
    PIN_D7=0;
    // modo escritura
    PIN_E=0;  // modo comando
    __delay_us(100); // se debe esperar 18TCY
    
    PIN_D4=(cmd>>4)&0x01;
    PIN_D5=(cmd>>5)&0x01;
    PIN_D6=(cmd>>6)&0x01;
    PIN_D7=(cmd>>7)&0x01;
    //cargamos el valor
    PIN_E=1;  // modo comando
    __delay_us(100); // se debe esperar 18TCY
    PIN_E=0;  // modo comando
    
    PIN_D4= cmd&0x01;
    PIN_D5=(cmd&0x0F)>>1;
    PIN_D6=(cmd&0x0F)>>2;
    PIN_D7=(cmd&0x0F)>>3;
    //cargamos el valor
    PIN_E=1;  // modo comando
    __delay_us(100); // se debe esperar 18TCY
    PIN_E=0;  // modo comando
    
    TRIS_D4=1;
    TRIS_D5=1;
    TRIS_D6=1;
    TRIS_D7=1;
    
    return ;
}

void LCD_Chr(unsigned char row, unsigned char column, char letra){
    LCD_XY(row,column);
    PIN_RS=1;
    enviar_dato(letra);
    PIN_RS=0;
    return;
}

void LCD_Chr_Cp(char letra){
    PIN_RS=1;
    enviar_dato(letra);
    PIN_RS=0;
    return;
}

void LCD_Cmd(unsigned char cmd){
    PIN_RS=0;
    enviar_dato(cmd);
    PIN_RS=0;
    return;
}

void LCD_XY(unsigned char row, unsigned char column){
    
    switch(row){
        case 1: LCD_Cmd(0x80 + (column-1)); break;
        case 2: LCD_Cmd(0xC0 + (column-1)); break;
        case 3: LCD_Cmd(0x94 + (column-1)); break;
        case 4: LCD_Cmd(0xD4 + (column-1)); break; 
    }
    __delay_ms(100);
    PIN_RS=0;
    return;
}

void LCD_Out_Cp(char *text){
    while(*text){
        LCD_Chr_Cp(*text);
        text++;
    }
    return;
}
void LCD_Out(unsigned char row, unsigned char column, char *text){
    LCD_XY(row,column);
    while(*text){
        LCD_Chr_Cp(*text);
        text++;
    }
    return;
}
void LCD_LIMPIA_FILA(unsigned char fila){
    LCD_Out(fila,1,"                    ");
}