//#include <pruebaCom.h>
#include <16f1827.h>
#use delay(internal=4M)
#use RS232(BAUD=4800, XMIT=PIN_B5, parity=N)

typedef struct fecha{
   int dia;
   int mes;
   int ano;
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


 char* cadena="edi2";
   sData paquete[11];
   
   
void transmitirDatos(sData* datos, Paciente pac);

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
   
   while(TRUE)
   {
      
       transmitirDatos(&paquete, pac);
       delay_ms(20);    
   
      //TODO: User Code
   }

}

void transmitirDatos(sData* datos, Paciente pac)
{
   printf("Nombre: %s Sexo: %c Fecha de nacimiento: %d/%d/%d  Edad: %d", *pac.nombre, pac.sexo, pac.fNac.dia, pac.fNac.mes, pac.fNac.ano, pac.edad);

   for(int i=0;i<11;i++)
   {
   
      if(datos[i].ear)
         {printf("Listening: %d, Frecuencia: %d, dB: %d, oreja: derecha",datos[i].listening, datos[i].freq, datos[i].dB);}
      else
         {printf("Listening: %d, Frecuencia: %d, dB: %d, oreja: izquierda", datos[i].listening, datos[i].freq, datos[i].dB);}
   }

}
