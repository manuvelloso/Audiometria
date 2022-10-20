#include <PF.h>
#define use port_b_lcd TRUE
#include <lcd.c>

#BYTE TRISA = 0x85
#BYTE TRISB = 0x86
#BYTE PORTA = 0x05

#define N_tmr0 60 //Se calculó el N_tmr0 para 3 segundos
#define N 12

// Vector de frecuancias
int FrecuenciasARegistrar[N] = { 125, 250, 500, 750, 1000, 1500, 2000, 3000, 4000, 6000, 8000 };

// Variables
int Frecuencia = FrecuenciasARegistrar[0]; // Frecuencia inicializada en 125
int N_tmr1 = 0;
int contador_tmr0 = 0;
int nueva_frec = 0;
int terminarMenu = 0;

//Funciones
void Menu();
void Seteo_Senal();  // Seteo de la senal según la frecuencia
void Enviar_Senal(); // Inicialización de timer0 y timer1
void Finalizar();    // Datos a la eeprom
void Comunicar();

// EStructura
struct sDatos{      
   int listening; //1 si está escuchando    
   int freq;      
   int dB;      
   int ear; //1 derecho
};


struct sDatos lista[N];


// Interrupción de timer1 la utilizamos para generar la señal cuadrada en el pin B7
#int_TIMER1
void TIMER1_isr() {
   output_toggle(PIN_B7);
   set_timer1(N_tmr1);
}

// Interrupción de timer0 la utilizamos para registrar el NO PULSADO/ NO ESCUCHO
#int_TIMER0
void TIMER0_isr() {
   contador_tmr0 += 1;
   if (contador_tmr0 == 6) {
      //registrar no escucho para ESCRIBIR EN EEPROM 
      contador_tmr0 = 0;
      nueva_frec = 1;
   }
}

// Interrupción externa RB0 la utilizamos para registrar el PULSADO/ ESCUCHO al presionar el boton en RB0
#int_ext
void ext_isr() {
   //registrar escucho para ESCRIBIR EN EEPROM
   disable_interrupts(int_TIMER0);
   nueva_frec = 1;
}

// aca hay que poner una variable para que se interrumpa con el boton de rb4 a rb7
#int_RB
void RB_isr() {

   terminarMenu = 1;

}

void main() {
   
   // Seteo de los timers con Prescaler = 256
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
   setup_timer_1(RTCC_INTERNAL | RTCC_DIV_256);

   // Seteo de entradas analógicas
   setup_adc_ports(ALL_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);

   // Usamos LCD
   lcd_init();
 
   // Mostramos el menu
   Menu();
   
   int i = 0; // Iterador
   
   do {
      Frecuencia = FrecuenciasARegistrar[i];
      
      // Habilitamos interrupciones
      enable_interrupts(int_ext);      // Interrupción externa RB0
      Ext_int_edge(H_to_L);            
      enable_interrupts(int_TIMER0);   // Interrupción de timer0
      enable_interrupts(int_TIMER1);   // Interrupcion de timer1
      enable_interrupts(GLOBAL);       // Llave maestra

      // Seteo de timer1 según la frecuencia de análisis
      Seteo_Senal();
      
      delay_ms(1000);

      while (!nueva_frec)
         Enviar_Senal();
         
      i++;

   } while (Frecuencia != 8000);


   Finalizar();

   Comunicar();

}

// Pulsador de Menu RB3
void Menu() {
   Lcd_init(); // ¡CUIDADO!
   signed char x1 = 16;
   signed char x0 = 16;
   signed char x3 = 16;

   while (!terminarMenu) {
      if (x0 >= -24) {
         Lcd_gotoxy(x0, 1);
         printf(lcd_putc, "Frecuencias a registrar: ");
         delay_ms(100);
         x0 = x0 - 1;
      }


      if (x0 < -24 && x1 >= -47) {
         printf(lcd_putc, "\f");
         Lcd_gotoxy(x1, 0);
         printf(lcd_putc, "125, 250, 500, 750, 1K, 1K5, 2K, 3K, 4K, 6K, 8K \n");
         delay_ms(100);
         x1 = x1 - 1;
      }

      if (x1 < -47) {
         printf(lcd_putc, "\f");
         Lcd_gotoxy(x3, 1);
         printf(lcd_putc, "Pulse el boton para comenzar");
         delay_ms(100);
         x3 = x3 - 1;
      }
      
      if (x3 < -28) {
         x1 = 16;
         x0 = 16;
         x3 = 16;
      }

   }
   
   return;
}

void Seteo_Senal() {
   // Se calcula N_tmr1 mediante la ecuación T = 4/fosc * 256 * (256 - N_tmr1), prescaler = 256
   float T0 = 1 / Frecuencia; // Periodo
   float T = T0 / 2; // Subperiodo
   N_tmr1 = 256 - (T * 4000000 / (4 * 256));
}

// Se envia durante 3 segs la frecuencia hasta realizar la escritura en la EEPROM
void Enviar_Senal() {
   //inicializacion de timers
   set_timer0(N_tmr0); 
   set_timer1(N_tmr1);
   
   output_high(PIN_A7); //pin de salida de la patita
}

void Finalizar() {
   printf(lcd_putc, "Se registraron todas las mediciones");
   delay_ms(1500);
   printf(lcd_putc, "\f");
   // Datos a la EEPROM
}

void Comunicar() {}

