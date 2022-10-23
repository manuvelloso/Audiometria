#include <PF.h>

#BYTE TRISA = 0x85
#BYTE TRISB = 0x86
#BYTE PORTA = 0x05
#BYTE PORTB = 0x06

int Frecuencia = FrecuenciasARegistrar[0]; // Frecuencia inicializada en 125

// Variables globales
int N_tmr1 = 0;         // Valor que toma el tmr1
int contador_tmr0 = 0;  // Para realizar ciclos de repetición en el tmr1
int pos_frec = 0;       // Indica la posicion "actual" en la lista sDatos y FrecuenciasARegistrar
int nueva_frec = 0;     // Booleano para ver si termino la frecuencia de registro


struct sDatos lista[N]; // Lista de sDatos

//Esta interrupcion la utilizamos para generar la senal cuadrada en el pin A7
#int_TIMER1
void TIMER1_isr() {
   output_toggle(PIN_A7);
   set_timer1(N_tmr1);
}

//Esta interrupcion la utilizamos para registrar el NO PULSADO
#int_TIMER0
void TIMER0_isr() {
   contador_tmr0++;
   if (contador_tmr0 == 6) {
      lista[pos_frec].listening = 0; // No escuchó
      contador_tmr0 = 0;
      nueva_frec = 1;
   }

}

//Esta interrupcion la utilizamos para registrar el PULSADO al presionar el boton de escucha en RB0
#int_ext
void ext_isr() {
   lista[pos_frec].listening = 1; // Sí escuchó
   disable_interrupts(int_TIMER0);
   nueva_frec = 1;
}


void main() {
   // Seteo de los timers con Prescaler = 256
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
   setup_timer_1(RTCC_INTERNAL | RTCC_DIV_256);

   // Seteos de entrada/ salida
   TRISB = 0x04;
   TRISA = 0x40;

   // Usamos LCD
   lcd_init();
   
   // Mostramos el menu
   Menu();
   
   printf(lcd_putc, "/f");
   printf(lcd_putc, "Toma de datos en curso");

   do {
      Frecuencia = FrecuenciasARegistrar[pos_frec];
      lista[pos_frec].freq = Frecuencia;

      // Habilitamos interrupciones
      enable_interrupts(int_ext);      // Interrupción externa RB0
      Ext_int_edge(H_to_L);
      enable_interrupts(int_TIMER0);   // Interrupción de timer0
      enable_interrupts(int_TIMER1);   // Interrupcion de timer1
      enable_interrupts(GLOBAL);       // Llave maestra

      // Seteo de timer1 según la frecuencia de análisis
      Seteo_Senal();

      delay_ms(1000);

      while (nueva_frec == 0) //!nueva_frec
         Enviar_Senal();  // se envia durante 3 segundos la misma frecuencia hasta realizar la escritura en la eeprom

      pos_frec++;

   } while (Frecuencia != 8000);

   Finalizar();

   Comunicar();

}


void Menu() {

   // Variables
   signed char x1 = 16;
   signed char x0 = 16;
   signed char x3 = 16;

   while (input(PIN_B3)) {
      
      if (x0 >= -24) {
         Lcd_gotoxy(x0, 1);
         printf(lcd_putc, "Frecuencias a registrar: ");
         
         delay_ms(100);
         
         x0 = x0 - 1; //x0--;
      }
      
      if (x0 < -24 && x1 >= -47) {
         printf(lcd_putc, "\f");
         
         Lcd_gotoxy(x1, 0);
         printf(lcd_putc, "125, 250, 500, 750, 1K, 1K5, 2K, 3K, 4K, 6K, 8K \n");
         
         delay_ms(100);
         
         x1 = x1 - 1; //x1--;
      }

      if (x1 < -47) {
         printf(lcd_putc, "\f");
         
         Lcd_gotoxy(x3, 1);
         printf(lcd_putc, "Pulse el boton para comenzar");
         
         delay_ms(100);
         
         x3 = x3 - 1; //x3--;
      }
      
      if (x3 < -28) {
         x1 = 16;
         x0 = 16;
         x3 = 16;
      }

   }


}

void Seteo_Senal() {
   // Se calcula N_tmr1 mediante la ecuación T = 4/fosc * 256 * (256 - N_tmr1), prescaler = 256
   float T0 = 1 / Frecuencia; // Periodo
   float T = T0 / 2; // Subperiodo
   N_tmr1 = 256 - (T * 4000000 / (4 * 256));

}

void Enviar_Senal() {
   // Inicialización de timers
   set_timer0(N_tmr0);
   set_timer1(N_tmr1);
   
   output_high(PIN_B7); //pin de salida de la patita
}

void Finalizar() {
   printf(lcd_putc, "Se registraron todas las mediciones /f");
   // DATOS A LA EEPROM
}

void Comunicar() {}

// Esta vuelta es para un oido
// Antes del Comunicar() repetir para 2do oido
// Apretar el boton de Menu
