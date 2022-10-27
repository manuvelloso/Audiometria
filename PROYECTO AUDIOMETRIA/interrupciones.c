#include <globals.h>

// Interrupción de timer1 la utilizamos para generar la señal cuadrada en el pin A7
#int_TIMER0
void TIMER0_isr() {
   output_toggle(PIN_A7);
   set_timer0(N_tmr0);
}


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






// Interrupción externa RB0 la utilizamos para registrar el PULSADO/ ESCUCHO al presionar el boton en RB0
#int_ext
void ext_isr() {
   
   if(input(PIN_B0)) {
      lista[pos_frec].listening = 1; // Se escuchó?
      lista[pos_frec].dB = vol; // Hay que hacer la cuenta
      disable_interrupts(int_TIMER1);
      nueva_frec = 1; // Cambiamos de frecuencia
      
      i = 0x00; // Volumen inicial
      porta = 0x00; // Para que deje de sonar
      delay_ms(500); // Por un cachito de tiempo
   }
   
   
   
}

// aca hay que poner una variable para que se interrumpa con el boton de rb4 a rb7
#int_RB
void RB_isr() {

   terminarMenu = 1;

}

