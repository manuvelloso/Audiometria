#include <funciones.h>

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
