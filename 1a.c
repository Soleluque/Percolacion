#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int poblar (int *red, float p, int dim, int sem);
int clasificar(int *red,int dim);
int etiqueta_verdadera(int *historial, int s);
int percolacion(int *red, int dim);
int imprimir(int *red,int dim);


//Función principal:
int main(int argc,char*argv[])
{ float p, p_i, p_f, prec = 0.0001, p_c;
  int dim, i, per, sem;
  int *red;

  sscanf(argv[1],"%d",& dim);           //Busca el primero de los argumentos y lo usa como dim.
  sscanf(argv[2],"%f",& p);             //Busca el segundo de los argumentos y lo usa como p.

  red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.

  p_c = 0;

  for(sem=0;sem<10000;sem++)
  { p_f = 0;
    p_i = p;
    i = 2;

    while(fabs(p_f-p_i)>prec)
    { poblar(red, p_i, dim,sem);
      //imprimir(red, dim);
      //printf("\n");
      clasificar(red,dim);
      //imprimir(red, dim);
      per = percolacion(red, dim);
      //printf("%i\n", per);

      if(per)
      { p_f = p_i;
        p_i -= 1.0/pow(2,i);
        i++;
      }
      if(!per)
      { p_f = p_i;
        p_i += 1.0/pow(2,i);
        i++;
      }
    }

    p_c += p_f;

  }

  p_c /= 10000;

  printf("El P_c para L = %i es: %f.\n", dim, p_c);

  free(red);

  return 0;
}

//Funciones secundarias:
int poblar(int *red, float p, int dim, int sem)
{ float random;
  int i;
  srand(sem);


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
{ int s,S1,S2,i,j,frag;
  int *historial;

  //Creo la tira historial:
  historial = malloc((((dim*dim)/2)+1)*sizeof(int));

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
    { *(red+i) = etiqueta_verdadera(historial,S1);

    }
    if(*(red+i) && !S1)  //S1 es 0.
    { *(red+i)=frag;
      frag++;
    }
  }

  for(i=1;i<dim;i++)  //Primera columna sin primer lugar.
  { S2 = *(red+(i*dim-dim));
    if(*(red+(i*dim)) && S2)  //S2 es 1.
    { *(red+(i*dim)) = etiqueta_verdadera(historial,S2);
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
      { if (S1 && S2 && (etiqueta_verdadera(historial,S2)<etiqueta_verdadera(historial,S1))) //Los dos son 1 y S1 más grande.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial,S2);

          *(historial+etiqueta_verdadera(historial,S1))=-etiqueta_verdadera(historial,S2);
        }
        if (S1 && S2 && (etiqueta_verdadera(historial,S1)<etiqueta_verdadera(historial,S2))) //Los dos son 1 y S2 más grande.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial,S1);

          *(historial+etiqueta_verdadera(historial,S2))=-etiqueta_verdadera(historial,S1);
        }
        if (S1 && S2 && (etiqueta_verdadera(historial,S2)==etiqueta_verdadera(historial,S1))) //Los dos son 1 y son iguales, elijo arbitrariamente S2.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial,S2);

       }
        if ((!S1) && S2) //S1 es cero y S2 es 1.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial,S2);

        }
        if (S1 && (!S2)) //S1 es 1 y S2 es cero.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial,S1);

        }
       }

      if ( *(red+(i*dim+j)) && ((!S1) && (!S2))) //los dos son cero.
      { *(red+(i*dim+j))=frag;
        frag++;
      }
    }
  }

  /*for(i=0;i<((dim*dim)/2);i++)
   { printf("%i ", *(historial+i));
   }
   printf("\n");*/



  for(s=0;s<dim*dim/2;s++)
  {
    *(historial+s)=etiqueta_verdadera(historial, s);

  }


  for(i=0;i<dim*dim;i++)
  { *(red+i) = *(historial+*(red+i));
  }

  /*for(i=0;i<((dim*dim)/2);i++)
   { printf("%i ", *(historial+i));
   }
   printf("\n");*/

  free(historial);

  return 0;

  }


int etiqueta_verdadera(int *historial, int s)
{

  while(*(historial+s)<0)
  {
    s=-*(historial+s);
  }

  return *(historial+s);


}





int imprimir(int *red, int dim)         //Imprime una fila debajo de la otra.
{ int i,j;

  for (i=0;i<dim;i++)
    {for (j=0;j<dim;j++)
      {
      printf("%02d ", *(red+dim*i+j));
      }
    printf("\n");
    }

  return 0;
}

int percolacion(int *red, int dim)
{ int i,j,per = 0;

  for(i=0;i<dim;i++)
  { for(j=0;j<dim;j++)
    { if (*(red+i) == *(red+(dim-1)*dim+j) && *(red+i))
      { per = 1;
      }
    }
  }

  return per;
}
