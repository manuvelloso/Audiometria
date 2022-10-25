
#include <globals.h>
#fuses XT,NOWDT,NOPROTECT,PUT //ordenes para el programador
#use delay (clock=4M)   // Fosc=8Mhz
#use RS232(BAUD=4800, XMIT=PIN_A0, parity=N)
//rx=b1
//en PC-> pin 2 es el pin receptor

typedef struct fecha{
   int dia;
   int mes;
   long int ano;
   }Fecha;

typedef struct paciente{
   char* nombre;
   char sexo;//f: femenino m:masculino
   int edad;
   Fecha fNac;  
}Paciente;

typedef struct data{
   
   int listening;//1 si está escuchando  
   int freq;   
   int dB;   
   int ear; //1 derecho
}sData;   

sData paquete[11];

void transmitirDatos(sData* datos, Paciente pac)
{
     printf("Nombre: %s \r\nSexo: %c \r\nFecha de nacimiento: %d/%d/%ld  \r\nEdad: %d\r\n ", *pac.nombre, pac.sexo, pac.fNac.dia, pac.fNac.mes, pac.fNac.ano, pac.edad);
   printf("\r\n--------------------------------\r\n");
   delay_ms(1000);
   
   for(int i=0;i<11;i++)
   {
   
      if(datos[i].ear)
         {printf("-Listening: %d \r\n-Frecuencia: %d \r\n-dB: %d\r\n-Oreja: derecha \r\n",datos[i].listening, datos[i].freq, datos[i].dB);
         delay_ms(1000);
         }
      else
         {printf("-Listening: %d \r\n-Frecuencia: %d \r\n-dB: %d \r\n-oreja: izquierda", datos[i].listening, datos[i].freq, datos[i].dB);
           delay_ms(1000);
         }
}

void main()
{
   Paciente pac;
   pac.nombre="Melanie";
   pac.sexo='f';
   pac.edad=20;
   pac.fNac.dia=06;
   pac.fNac.mes=01;
   pac.fNac.ano=2002;
   
   for(int i=0;i<11;i++)
   {
      paquete[i].listening=i+1;
      paquete[i].freq=i+1;
      paquete[i].dB=i+1;
      paquete[i].ear=1;   
   }
   
   //printf("\f");
   transmitirDatos(&paquete, pac);
}
