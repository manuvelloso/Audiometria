// NEW GLOBALS

#include <16F1827.h>
#device ADC=10
#use delay(crystal=4MHz)
#include <lcd.c>


#BYTE TRISA=0x85
#BYTE TRISB=0x86
#BYTE PORTA=0x05
#BYTE PORTB=0x06
#BYTE OPTION_REG=0X81

void Menu();
#include <Afunc.c>

