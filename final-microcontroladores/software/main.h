/* 
// Universidade Federal de Santa Catarina - Microcontroladores
// Desenvolvedor: Ale Chaito
// Data: 18/11/2018
// Objetivo: Mesa de bilhar microcontrolada
*/ 
#include <16F877A.h>
#device ADC = 8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOPROTECT

#use delay(crystal=4000000)

#define LCD_ENABLE_PIN  PIN_E0    
#define LCD_RS_PIN      PIN_E1   
#define LCD_RW_PIN      PIN_E2 
#define LCD_DATA4       PIN_D4 
#define LCD_DATA5       PIN_D5  
#define LCD_DATA6       PIN_D6   
#define LCD_DATA7       PIN_D7 


#define START     PIN_B1
#define COIN    PIN_B2
#define GEL_START PIN_B3
#define THERMO    PIN_B4

#define MOTOR     PIN_D0
#define NIGHT_LED PIN_D1
#define BUZZER    PIN_D2
#define GEL_LIGHT PIN_D3

#define M0        PIN_C0
#define M1        PIN_C1
#define M2        PIN_C2
#define M3        PIN_C3

#define GEL_MOTOR PIN_C4

#include <lcd.c>

void setup_game();
void check_light();
void player_point();
void open_drawer();

