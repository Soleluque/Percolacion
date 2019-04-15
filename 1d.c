#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int poblar (int *red, float p, int dim, int sem);
int etiqueta_verdadera(int *historial, int s);
int etiqueta_falsa(int *red,int *historial, int S1, int S2, int i, int j, int dim);
int clasificar(int *red,int dim);
int percolacion(int *red, int dim);
int imprimir(int *red,int dim);


//Función principal:
int main(int argc,char*argv[])
{ float p;
  int i, j, k, dim, sem = 0;
  int *red, *s, *ns, *dims;
  float  *p_c;
  FILE *n_s;

  p_c = malloc(6*sizeof(float));
  dims = malloc(6*sizeof(int));
  n_s = fopen("n_s.txt", "w");

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

  for(i=0;i<6;i++)
  { p = *(p_c+i) - 0.1;
    dim = *(dims+i);

    red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.
    s = malloc((dim*dim)*sizeof(int));
    ns = malloc((dim*dim)*sizeof(int));

    fprintf(n_s, "Dimensión = %i\n", dim);

    while(p<=*(p_c+i)+0.1)
    { poblar(red, p, dim,sem);
      clasificar(red,dim);

      for(j=0;j<dim*dim;j++)
      { *(s+j) = 0;
      }

      for(j=0;j<dim*dim;j++)
      { *(ns+j) = 0;
      }

      for(j=0; j<dim*dim; j++)
        { if(*(red+j))
          { *(s+*(red+j)) += 1;
          }
        }

      for(j=0; j<dim*dim; j++)
      { if(*(s+j))
        { *(ns+*(s+j)) += 1;
        }
      }

      for(j=0; j<dim*dim; j++)
      { for(k=0; k<dim*dim; k++)
        { if(*(s+j) == *(s+k) && *(s+j))
          { *(ns+*(s+j)) += 1;
          }
          break;
        }
      }

      fprintf(n_s, "Probabilidad = %f\n", p);
      fprintf(n_s, "s n_s\n");
      for(j=0; j<dim*dim; j++)
      { fprintf(n_s, "%i %i\n", *(s+j), *(ns+j));
      }
      fprintf(n_s, "\n");

      p += 0.0005;

    }

  }

  fclose(n_s);
  free(ns);
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
