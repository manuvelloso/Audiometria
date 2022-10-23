#include <funciones.c>

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
