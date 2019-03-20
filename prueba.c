#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int poblar (int *red, float p, int dim);
int imprimir(int *red,int dim);

int main(int argc,char** argv[])
{
  float p;
  int dim;
  int *red;
  red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.
  sscanf(argv[1],"%d",& dim);           //Busca el primero de los argumentos y lo usa como dim.
  sscanf(argv[2],"%f",& p);             //Busca el segundo de los argumentos y lo usa como p.

  poblar(red, p, dim);                  //Usa la funcion poblar.
  imprimir(red,dim);
  return 0;
}




int poblar(int *red, float p, int dim)
{ float random;
  int i;
  srand(time(NULL));


  for (i=0;i<dim*dim;i++)               //Toma como rango máximo la cantidad de celdas totales.

    { *(red+i)=0;                       //Asigna por defecto el valor <vacio>
      random=((float)rand())/((float) RAND_MAX);    //Valor aleatorio entre 0 y 1.
      if (random<p)
      *(red+i)=1;                       //Asigna como <lleno> si el valor aleatorio es menor.
    }

  return 0;
}

int imprimir(int *red, int dim)         //Imprime una fila debajo de otra.
{ int i,j;

  for (i=0;i<dim;i++)
    {for (j=0;j<dim;j++)
      {
      printf("%d ", *(red+dim*i+j));
      }
    printf("\n");
    }

  return 0;
}
