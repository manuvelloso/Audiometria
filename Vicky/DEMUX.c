#include <DEMUX.h>
#use delay(clock = 4M)

#byte trisa = 0x85
#byte porta = 0x05

#byte trisb = 0x86
#byte portb = 0x06

int i = 0x00;
int T1 = 10;

int nueva_frec = 0;

// Interrupción de timer0 la utilizamos para registrar el NO PULSADO/ NO ESCUCHO
#int_TIMER1
void TIMER1_isr() {

   contador_tmr1--;          //Se decrementa hasta llegar a cero
   set_timer1(28036);//Carga de nuevo el timer1
   if (contador_tmr1 <= 0) // Si llega a cero, se cumplió el periodo de muestreo
   {
      // Antes de que llegue al ultimo volumen:
      int masc = input(PIN_A3);
      masc << 3;
      
      porta = vol + masc;
      
      
      
      contador_tmr1 = 10;  // Inicializa el contador para el próximo periodo
   }
      
      if(vol == 0x08) { // Si llegó al último volumen
         lista[pos_frec].listening = 0; // No escucho?
         vol = 0x00;
        // nueva_frec = 1;
      } else {
         //Aumento el volumen
         vol++;  
      }
      
}


#int_TIMER0
void TIMER0_isr() {
   output_toggle(PIN_A3);
   set_timer0(0);
}


#int_ext
void ext_isr() {
   
   if(input(PIN_B0)) {
   /*lista[pos_frec].listening = 1; // Se escuchó?
   lista[pos_frec].dB = vol; // Hay que hacer la cuenta*/
   disable_interrupts(int_TIMER1);
   nueva_frec = 1;
   i = 0x00;
   porta = 0x00;
   delay_ms(500);
   }
   
}



// Tiempo = 4/fosc * Pr * (256 - TMR0)

void main()
{
      
   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_8 );
   T1 = 10;        //Carga Contrador para Calcular el Periodo de Muestreo
   set_timer1(28036); 
   
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
   set_timer0(0);
   
   enable_interrupts(int_TIMER0);
   enable_interrupts(int_TIMER1);
   enable_interrupts(int_EXT);
   enable_interrupts(GLOBAL);

   
   trisa = 0x00;
   porta = 0x08;
   trisb = 0xff;

   while(TRUE) {
      if(nueva_frec) {
      enable_interrupts(int_TIMER1);
      }
      
      
   }

}
