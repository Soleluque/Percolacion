#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int poblar (int *red, float p, int dim);
int etiqueta_falsa(int *red,int *historial, int S1, int S2, int i, int j, int dim);
int clasificar(int *red,int dim);
int percolacion(int *red, int dim);
int imprimir(int *red,int dim);


//Función principal:
int main(int argc,char*argv[])
{ float p;
  int dim;
  int *red;
  sscanf(argv[1],"%d",& dim);           //Busca el primero de los argumentos y lo usa como dim.
  sscanf(argv[2],"%f",& p);             //Busca el segundo de los argumentos y lo usa como p.
  red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.

  poblar(red, p, dim);
  imprimir(red,dim);
  printf("\n");
  clasificar(red,dim);
  imprimir(red,dim);
  percolacion(red, dim);
  free(red);
  return 0;
}

//Funciones secundarias:
int poblar(int *red, float p, int dim)
{ float random;
  int i;
  srand(time(NULL));


  for (i=0;i<dim*dim;i++)               //Toma como rango máximo la cantidad de celdas totales.
    { *(red+i)=0;                       //Asigna por defecto el valor <vacio>
      random=((float)rand())/((float) RAND_MAX);    //Valor aleatorio entre 0 y 1.
      if (random<p)
      {*(red+i)=1;
      }                       //Asigna como <lleno> si el valor aleatorio es menor.
    }

  return 0;
}


int clasificar(int *red,int dim)
{ int S1,S2,i,j,frag;
  int *historial;

  //Creo la tira historial:
  historial = malloc((dim*dim)/2);
  for(i=0;i<=(dim*dim)/2;i++)
  { *(historial+i) = i;
  }

  frag = 2;

  if(*(red)) //Primer lugar.
  { *(red) =frag;
    frag++;
  }

  for(i=1;i<dim;i++) //Primera fila sin primer lugar.
  { S1= *(red+i-1);
    if(*(red+i) && S1)  //S1 es 1.
    { *(red+i) = S1;
    }
    if(*(red+i) && !S1)  //S1 es 0.
    { *(red+i)=frag;
      frag++;
    }
  }

  for(i=1;i<dim;i++)  //Primera columna sin primer lugar.
  { S2 = *(red+(i*dim-dim));
    if(*(red+(i*dim)) && S2)  //S2 es 1.
    { *(red+(i*dim)) = S2;
    }
    if(*(red+(i*dim)) && !S2)  //S2 es 0.
    { *(red+(i*dim))=frag;
      frag++;
    }
  }

  for(i=1;i<dim;i++) //Todo el resto.
  { for(j=1;j<dim;j++)
    { S1 = *(red+(i*dim+j-1)); //El casillero de la izquierda.
      S2 = *(red+(i*dim+j-dim)); //El casillero de arriba.

      if( *(red+(i*dim+j)) && (S1 || S2)) // Acá entran cuando los dos o uno de ellos son 1.
      { if (S1 && S2 && (S2<S1)) //Los dos son 1 y S1 más grande.
        { *(red+(i*dim+j))=S2;
          *(red+(i*dim+j-1))=S2;
          *(historial+S1) = -*(historial+S2);
        }
        if (S1 && S2 && (S1<S2)) //Los dos son 1 y S2 más grande.
        { *(red+(i*dim+j))=S1;
          *(red+(i*dim+j-dim))=S1;
          *(historial+S2) = -*(historial+S1);
        }
        if (S1 && S2 && (S2==S1)) //Los dos son 1 y son iguales, elijo arbitrariamente S2.
        { *(red+(i*dim+j))=S2;
        }
        if ((!S1) && S2) //S1 es cero y S2 es 1.
        { *(red+(i*dim+j))=S2;
        }
        if (S1 && (!S2)) //S1 es 1 y S2 es cero.
        { *(red+(i*dim+j))=S1;
        }
       }

      if ( *(red+(i*dim+j)) && ((!S1) && (!S2))) //los dos son cero.
      { *(red+(i*dim+j))=frag;
        frag++;
      }
    }
  }

  /*for(i=0;i<=(dim*dim)/2;i++)
  { printf("%d ", *(historial+i));
  }
  printf("\n");*/

  for(i=1;i<dim;i++) //Todo el resto.
  { for(j=1;j<dim;j++)
    { S1 = *(red+(i*dim+j-1)); //El casillero de la izquierda.
      S2 = *(red+(i*dim+j-dim)); //El casillero de arriba.

      etiqueta_falsa(red, historial, S1, S2, i, j, dim);
      }
  }

  /*for(i=0;i<dim*dim;i++)
  { *(red+i) = *(historial+*(red+i));
  }*/

  /*for(i=0;i<=(dim*dim)/2;i++)
  { printf("%d ", *(historial+i));
  }
  printf("\n");*/

  free(historial);
  return 0;

  }

int etiqueta_falsa(int *red,int *historial, int S1, int S2, int i, int j, int dim)
{ while(*(historial+S1)<0 && S1)  //Cambia etiquete en historial.
  { *(historial+S1) = -*(historial+S1);
  }
  while (*(historial+S2)<0 && S2)
  { *(historial+S2) = -*(historial+S2);  //Cambia etiquete en historial.
  }

  return 0;

}


int imprimir(int *red, int dim)         //Imprime una fila debajo de la otra.
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

int percolacion(int *red, int dim)
{ int i,j;
  int *percolante_s,*percolante_i;

  percolante_s = malloc(dim);
  percolante_i = malloc(dim);

  for(i=0;i<dim;i++)
  { *(percolante_s+i) = *(red+i);
    *(percolante_i+i) = *(red+dim*dim+i);
  }

  for(i=0;i<dim;i++)
  { for(j=0;j<dim;j++)
    { if (*(percolante_s+i) == *(percolante_i+j) && *(percolante_s+i))
      { printf("He percolado!");
      }
    }
  }

  return 0;

}
