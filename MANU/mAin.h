// NEW GLOBALS

#include <16F1827.h>
#device ADC=10
#use delay(clock=4000000,crystal=4MHz)
#use RS232(BAUD=1200, XMIT=PIN_A3, parity=N)

#define use port_b_lcd TRUE
#define LCD_ENABLE_PIN  PIN_B3
#include <lcd.c>

#define N_tmr1 28036 //Se calculó el N_tmr0 para 3 segundos
#define N 11

// Vector de frecuancias
long int FrecuenciasARegistrar[N] = { 125,250, 500, 750, 1000, 1500, 2000, 3000, 4000, 6000, 8000 };

long int Frecuencia = FrecuenciasARegistrar[0]; // Frecuencia inicializada en 125

// Variables globales
int N_tmr0 = 0;         // Valor que toma el tmr0
int contador_tmr1 = 10;  // Para realizar ciclos de repetici?n en el tmr1
int pos_frec = 0;       // Indica la posicion "actual" en la lista sDatos y FrecuenciasARegistrar
int nueva_frec = 0;     // Booleano para ver si termino la frecuencia de registro
int vol = 0; // Primer volumen para inicializar

// Estructura
struct sDatos{      
   int listening; //1 si está escuchando    
   long int freq;      
   int dB;      
   //int ear; //1 derecho
};

struct sDatos lista[N]; // Lista de sDatos
#include <AF.c>

// Interrupción de timer0 la utilizamos para registrar el NO PULSADO/ NO ESCUCHO
#int_TIMER1
void TIMER1_isr() {
   contador_tmr1--;          //Se decrementa hasta llegar a cero
   set_timer1(N_tmr1);//Carga de nuevo el timer1
   if (contador_tmr1 <= 0) // Si llega a cero, se cumplió el periodo de muestreo
   {
      // Antes de que llegue al ultimo volumen:
      int masc = input(PIN_A4);
      masc << 4;
      output_a(vol + masc);
      contador_tmr1 = 10;  // Inicializa el contador para el próximo periodo
      
      if(vol == 0x08) { // Si llegó al último volumen
      lista[pos_frec].listening = 0; // No escucho
      lista[pos_frec].freq = Frecuencia;
      lista[pos_frec].dB = -1; // ñoescuchoñiñguño
      
      vol = 0x00;
      
      nueva_frec = 1;//camviofrecporqueñoescucho
      
      //printf(lcd_putc, "\ffrec=%ld vol=%d",lista[pos_frec].freq,lista[pos_frec].dB);
       printf(lcd_putc, "\fTMR0=%d",N_tmr0);
      transmitirDatos();
   } else {
      vol++;  //Aumento el volumen
   }
   }
}

#int_TIMER0
void TIMER0_isr() {
   output_toggle(PIN_A4);
   set_timer0(N_tmr0);
   
}

#int_ext
void ext_isr() {
   
   if(input(PIN_B0)) {
   lista[pos_frec].listening = 1; // Se escuchó?
   lista[pos_frec].dB = vol; // Hay que hacer la cuenta
   lista[pos_frec].freq = Frecuencia;
   
   disable_interrupts(int_TIMER1);
   disable_interrupts(int_TIMER0);
   
   nueva_frec = 1;
   printf(lcd_putc, "\ffrec=%ld vol=%d",lista[pos_frec].freq,lista[pos_frec].dB);
   //printf(lcd_putc, "\fTMR0=%d",N_tmr0);
   
   transmitirDatos();
   vol=0;
   }
}



// Tiempo = 4/fosc * Pr * (256 - TMR0)
