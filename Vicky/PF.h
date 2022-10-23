#include <16F1827.h>
#device ADC = 10
#use delay(clock = 4000000, crystal = 4MHz)
#define use port_b_lcd TRUE
#include <lcd.c>

#define N 12
#define N_tmr0 60 //Se calcul� el N_tmr0 para 3 segundos

typedef struct sDatos{ 
int listening;
int freq;      
int dB; // Stand by      
int ear;
} sData;

int FrecuenciasARegistrar[N] = {125, 250, 500, 750, 1000, 1500, 2000, 3000, 4000, 6000, 8000};

void Menu();
void Seteo_Senal();  // Seteo de la senal segun la frecuencia
void Enviar_Senal(); // Inicializacion de timers
void Finalizar();    // Datos a la EEPROM
void Comunicar();

