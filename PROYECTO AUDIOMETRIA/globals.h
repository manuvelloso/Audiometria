#include <16F1827.h>
#device ADC=10
#use delay(clock=4000000,crystal=4MHz)

#define use port_b_lcd TRUE
#include <lcd.c>

#BYTE TRISA = 0x85
#BYTE TRISB = 0x86
#BYTE PORTA = 0x05

#define N_tmr0 60 //Se calculó el N_tmr0 para 3 segundos
#define N 12

// Vector de frecuancias
int FrecuenciasARegistrar[N] = { 125, 250, 500, 750, 1000, 1500, 2000, 3000, 4000, 6000, 8000 };

int Frecuencia = FrecuenciasARegistrar[0]; // Frecuencia inicializada en 125

// Variables globales
int N_tmr1 = 0;         // Valor que toma el tmr1
int contador_tmr0 = 0;  // Para realizar ciclos de repetici?n en el tmr1
int pos_frec = 0;       // Indica la posicion "actual" en la lista sDatos y FrecuenciasARegistrar
int nueva_frec = 0;     // Booleano para ver si termino la frecuencia de registro
int terminarMenu = 0;

// EStructura
struct sDatos{      
   int listening; //1 si está escuchando    
   int freq;      
   int dB;      
   int ear; //1 derecho
};

struct sDatos lista[N]; // Lista de sDatos
