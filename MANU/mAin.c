#include <A.h>

void main()
{
   set_tris_a(0x10);
   set_tris_b(0xff);
   output_a(0x00);
   
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_8 );
   
   lcd_init();

   int i = 0;
   
   Menu();
   printf(lcd_putc, "\fLISTO");
   
   set_tris_b(0xff);//parausarelutto

do {
      set_tris_b(0xff);//parausarelutto
      Frecuencia = FrecuenciasARegistrar[i];
      
      // Seteo de timer1 según la frecuencia de análisis
      Seteo_Senal();
      
      //inicializacion de timers 
      set_timer0(N_tmr0); 
      set_timer1(N_tmr1);
      
       // Habilitamos interrupciones
      enable_interrupts(int_TIMER1);   // Interrupción de timer0
      enable_interrupts(int_TIMER0);   // Interrupcion de timer1
      enable_interrupts(int_ext);      // Interrupción externa RB0
      //Ext_int_edge(H_to_L); 
      enable_interrupts(GLOBAL);       // Llave maestra

      while (!nueva_frec){
      };
      
      i++;
 
   } while (Frecuencia != 8000);
   
   delay_ms(100);
   //transmitirDatos();
}
