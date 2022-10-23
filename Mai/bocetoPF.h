#include <16F1827.h>
#device ADC=10
#use delay(crystal=4)

typedef struct data
{ 
int listening;   int freq;   
   int dB;   
   int ear;
}sData;

int FrecuenciasARegistrar[12]={125,250,500,750,1000,1500,2000,3000,4000,6000,8000};

void Menu();
void Seteo_Senal();  // seteo de la senal segun la frecuencia
void Enviar_Senal(); //inicializacion de timers
void Finalizar(); //Datos a la eeprom
void Comunicar();
