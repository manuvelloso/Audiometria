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
  contador_tmr1++;
   if (contador_tmr1 == 6) {
      
      contador_tmr1 = 0; 
      
      if(vol == 0b1111) { // Si llegó al último volumen
         lista[pos_frec].listening = 0; // No escucho?
         nueva_frec = 1;
      } else {
         //Aumento el volumen
         vol = vol + 0b0001;  
      }
      
   }
}

// Interrupción externa RB0 la utilizamos para registrar el PULSADO/ ESCUCHO al presionar el boton en RB0
#int_ext
void ext_isr() {
   lista[pos_frec].listening = 1; // Se escuchó?
   lista[pos_frec].dB = vol; // Hay que hacer la cuenta
   disable_interrupts(int_TIMER1);
   nueva_frec = 1;
}

// aca hay que poner una variable para que se interrumpa con el boton de rb4 a rb7
#int_RB
void RB_isr() {

   terminarMenu = 1;

}

