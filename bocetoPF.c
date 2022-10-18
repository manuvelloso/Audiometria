#include <bocetoPF.h>
#define use port_b_lcd TRUE
#include <lcd.c>

#BYTE TRISA = 0x85
#BYTE TRISB = 0x86
#BYTE PORTA = 0x05
int N_tmr1=0;
int contador_tmr0=0;
int nueva_frec=0;
int terminarMenu=0;
/*typedef enum {F125=125, F250=250, F500=500, F750=750, F1K=1000,F1K5=1500,F2K=2000,F3K=3000, F4K=4000,F6K=6000,F8K=8000} F;*/
//F Frecuencia=125;
int FrecuenciasARegistrar[12]={125,250,500,750,1000,1500,2000,3000,4000,6000,8000};
int Frecuencia=FrecuenciasARegistrar[0];


void Menu();
void Seteo_Senal();  // seteo de la senal segun la frecuencia
void Enviar_Senal(); //inicializacion de timers
void Finalizar(); //Datos a la eeprom
void Comunicar();


//Esta interrupcion la utilizamos para generar la senal cuadrada en el pin B7
#int_TIMER1
void TIMER1_isr()
{
   output_toggle(PIN_B7);
   set_timer1(N_tmr1);
}

// aca hay que poner una variable para que se interrumpa con el boton de rb4 a rb7
#int_RB
void RB_isr()
{
  
  terminarMenu=1;
  
}

//Esta interrupcion la utilizamos para registrar el PULSADO al presionar el boton en RB0
#int_ext
void ext_isr() 
{
   //registrar escucho para ESCRIBIR EN EEPROM
    disable_interrupts(int_TIMER0);
    nueva_frec=1;
}

//Esta interrupcion la utilizamos para registrar el NO PULSADO
#int_TIMER0
void TIMER0_isr()
{
   contador_tmr0=contador_tmr0+1;
   if(contador_tmr0==6)
   {
     //registrar no escucho para ESCRIBIR EN EEPROM 
     contador_tmr0=0;
     nueva_frec=1;
   }
     
}


void main()
{
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
   setup_timer_1(RTCC_INTERNAL|RTCC_DIV_256);
   
   setup_adc_ports(ALL_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   
   lcd_init();
   int i=0;
   
   Menu();
   do
   {
   Frecuencia=FrecuenciasARegistrar[i];
   enable_interrupts(int_ext);
   Ext_int_edge(H_to_L);
   enable_interrupts(int_TIMER0);
   enable_interrupts(int_TIMER1);
   enable_interrupts(GLOBAL);
   
   Seteo_Senal(); // para crear la senal seteo el timer1 segun la frecuencia de analisis
   delay_ms(1000);
   
    while(nueva_frec==0)
    {  Enviar_Senal(); } // se envia durante 3 segundos la misma frecuencia hasta realizar la escritura en la eeprom
   
   i=i+1;
   
   }while(Frecuencia!=8000);
   
   Finalizar();
   
   Comunicar();
   
}


void Menu()
{
   Lcd_init();
   signed char x1= 16;
   signed char x0= 16;
   signed char x3=16;
   
   while(!terminarMenu)
   {
     if(x0>=-24)
     {
     Lcd_gotoxy(x0,1);
     printf(lcd_putc, "Frecuencias a registrar: ");
     delay_ms(100);
     x0=x0-1;
     }
     
     
     if(x0<-24 && x1>=-47) {
     printf(lcd_putc,"\f");
     Lcd_gotoxy(x1,0);
     printf(lcd_putc, "125, 250, 500, 750, 1K, 1K5, 2K, 3K, 4K, 6K, 8K \n");
     delay_ms(100);
     x1=x1-1;}
     
     if(x1<-47) 
     {
      printf(lcd_putc,"\f");
      Lcd_gotoxy(x3,1);
      printf(lcd_putc, "Pulse el boton para comenzar");
      delay_ms(100);
      x3=x3-1;
     }
     if(x3<-28)
     {   
      x1= 16;
      x0=16;
      x3=16;
     }
     
     }  


}

void Seteo_Senal()
{
   float T0=1/Frecuencia;
   float T=T0/2;
   N_tmr1=256-(T*4000000/(4*256));
   
   /*switch(frecuencia)
   {
   case F125: 
   *N_tmr1=0;
   *contador_tmr1=0;
   break;
   case F250:
   break;
   case F500:
   break;
   case F750: 
   break;
   case F1K:
   break;
   case F1K5:
   break;
   case F2K:
   break;
   case F3K:
   break;
   case F4K:
   break;
   case F6K:
   break;
   case F8K:
   break;
   } */
}

//inicializacion de timers
void Enviar_Senal()
{   
   set_timer0(60); //hay que calcularle el N para 3seg 
   set_timer1(N_tmr1);
   output_high(PIN_B7); //pin de salida de la patita
}

void Finalizar()
{
   printf(lcd_putc,"Se registraron todas las mediciones /f");
   // DATOS A LA EEPROM
}

void Comunicar(){}
