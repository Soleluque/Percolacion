#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

int poblar (int *red, float p, int dim, int sem);
int etiqueta_verdadera(int *historial, int s);
int etiqueta_falsa(int *red,int *historial, int S1, int S2, int i, int j, int dim);
int clasificar(int *red,int dim);
int percolacion(int *red, int dim);
int imprimir(int *red,int dim);


//Función principal:
int main(int argc,char*argv[])
{ float p, sp=0;
  int i, j, k, dim, sem, semm=27000;
  int *red, *s, *dims, *per;
  float *p_c;
  FILE *perc;

  p_c = malloc(6*sizeof(float));
  dims = malloc(6*sizeof(int));

  *(p_c) = 0.5630;
  *(p_c+1) = 0.5813;
  *(p_c+2) = 0.5878;
  *(p_c+3) = 0.5932;
  *(p_c+4) = 0.5922;
  *(p_c+5) = 0.5927;

  *(dims) = 4;
  *(dims+1) = 8;
  *(dims+2) = 16;
  *(dims+3) = 32;
  *(dims+4) = 64;
  *(dims+5) = 128;

  perc = fopen("masa", "w");

  for(i=0;i<6;i++)
  {dim = *(dims+i);

   red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.
   s = malloc((dim*dim)*sizeof(int));
   per = malloc(dim*sizeof(int));

   p = *(p_c+i);


     for(j=0;j<dim;j++)
      { *(per+j) = 0;
      }

      for(sem=0; sem<semm; sem++)
      { poblar(red, p, dim,sem);
        clasificar(red,dim);

        for(j=0;j<dim*dim;j++)
        { *(s+j) = 0;
        }

        for(j=0;j<dim;j++)
        { for(k=0;k<dim;k++)
          { if (*(red+j) == *(red+(dim-1)*dim+k) && *(red+j))
            { *(per+*(red+j)) += 1;
            }
          }
        }

        for(j=0; j<dim*dim; j++)
        { if(*(red+j))
          { *(s+*(red+j)) += 1;
          }
        }

        for(j=0; j<dim; j++)
        { if(*(per+j))
          { sp += *(s+j);
          }
        }
      }

      sp /= semm;    //Promedio de tamaño percolante.

      fprintf(perc, "%i ", dim);
      fprintf(perc, "%f", sp);
      fprintf(perc, "\n");
  }

  fclose(perc);

  free(per);
  free(s);
  free(dims);
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
{ int S1,S2,i,j,frag;
  int *historial;

  //Creo la tira historial:
  historial = malloc((dim*dim)/2*sizeof(int));
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
    { *(red+i) = etiqueta_verdadera(historial, S1);
    }
    if(*(red+i) && !S1)  //S1 es 0.
    { *(red+i)=frag;
      frag++;
    }
  }

  for(i=1;i<dim;i++)  //Primera columna sin primer lugar.
  { S2 = *(red+(i*dim-dim));
    if(*(red+(i*dim)) && S2)  //S2 es 1.
    { *(red+(i*dim)) = etiqueta_verdadera(historial, S2);
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
      { if (S1 && S2 && (etiqueta_verdadera(historial, S2)<etiqueta_verdadera(historial, S1))) //Los dos son 1 y S1 más grande.
        { *(red+(i*dim+j)) = etiqueta_verdadera(historial, S2);
          *(historial+S1) = -abs(*(historial+S2));
        }
        if (S1 && S2 && (etiqueta_verdadera(historial, S1)<etiqueta_verdadera(historial, S2))) //Los dos son 1 y S2 más grande.
        { *(red+(i*dim+j)) = etiqueta_verdadera(historial, S1);
          *(historial+S2) = -abs(*(historial+S1));
        }
        if (S1 && S2 && (etiqueta_verdadera(historial, S2)==etiqueta_verdadera(historial, S1))) //Los dos son 1 y son iguales, elijo arbitrariamente S2.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial, S2);
        }
        if ((!S1) && S2) //S1 es cero y S2 es 1.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial, S2);
        }
        if (S1 && (!S2)) //S1 es 1 y S2 es cero.
        { *(red+(i*dim+j))=etiqueta_verdadera(historial, S1);
        }
       }

      if ( *(red+(i*dim+j)) && ((!S1) && (!S2))) //los dos son cero.
      { *(red+(i*dim+j))=frag;
        frag++;
      }
    }
  }

  for(i=0; i<dim*dim/2; i++)
  { *(historial+i)=etiqueta_verdadera(historial, i);
  }


  for(i=0;i<dim*dim;i++)
  { *(red+i) = *(historial+*(red+i));
  }

  free(historial);

  return 0;

  }

int etiqueta_verdadera(int *historial, int s)
{ while(*(historial+s)<0)
  { s = - *(historial+s);
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
