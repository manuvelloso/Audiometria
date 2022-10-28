
void Menu()
{
 
   signed char x1= 16;
   signed char x0= 16;
   signed char x3=16;
   
   while(input(PIN_A3))
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
