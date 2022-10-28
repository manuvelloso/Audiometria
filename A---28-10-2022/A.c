#include <A.h>






void main()
{
   TRISB=0x04;
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);

   
   lcd_init();

   
   Menu();
   printf(lcd_putc, "LISTO");
}
