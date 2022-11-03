void Menu()
{
 
   signed char x1= 16;
   signed char x0= 16;
   signed char x3=16;
   
   while(input(PIN_A6))
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

void Seteo_Senal() {
   // Se calcula N_tmr1 mediante la ecuación T = 4/fosc * 256 * (256 - N_tmr1), prescaler = 256
   float T0 = 1 / Frecuencia; // Periodo
   float T = T0 / 2; // Subperiodo
   N_tmr0 = 256 - (T * 4000000 / (4 * 256));
   vol = 0x00;
   nueva_frec = 0;
}

void transmitirDatos()
{
   
  // printf("\f");
   //printf("\r\n--------------------------------\r\n");
   printf("\r\n -Listening: %d \r\n -Frecuencia: %ld \r\n -dB: %d", lista[pos_frec].listening,lista[pos_frec].freq, lista[pos_frec].dB);
   /*for(int i=0;i<11;i++)
   {
   
      //if(datos[i].ear)
         printf("\r\n--------------------------------\r\n");
         printf("-Listening: %d \r\n-Frecuencia: %ld \r\n-dB: %d",lista[i].listening, lista[i].freq, lista[i].dB);
         
         delay_ms(1000);
   }   
      else
         {printf("-Listening: %d \r\n-Frecuencia: %ld \r\n-dB: %d \r\n-oreja: izquierda", lista[i].listening, lista[i].freq, lista[i].dB);
           delay_ms(1000);
         }*/
}
