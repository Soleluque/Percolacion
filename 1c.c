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
{ float p=0.5, p_i, p_f, prec = 0.0001, p_c, dev = 0;
  int dim, i, per, sem;
  int *red;
  float  *p_fs;
  FILE *desviacion;

  p_fs = malloc(10000*sizeof(float));
  desviacion = fopen("desviacion.txt", "w");

  dim=4;

  while(dim!=256)
  { red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.

    p_c = 0;

    for(sem=0;sem<10000;sem++)
    { p_f = 0;
      p_i = p;
      i = 2;

      while(fabs(p_f-p_i)>prec)
      { poblar(red, p_i, dim,sem);
        clasificar(red,dim);
        per = percolacion(red, dim);

        if(per == 1)
        { p_f = p_i;
          p_i = p_i - 1.0/pow(2,i);
          i++;
        }
        if(per == 0)
        { p_f = p_i;
          p_i = p_i + 1.0/pow(2,i);
          i++;
        }
      }
      *(p_fs+sem) = p_f;
      p_c += p_f;
    }

    p_c = p_c/10000;

    for(i=0;i<10000;i++)
    { dev += pow(*(p_fs+i)-p_c,2);
    }

    dev /= 10000;
    dev = pow(dev,0.5);

    fprintf(desviacion, "%f %f\n", dev, p_c);

    dim*=2;
  }

  fclose(desviacion);
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
