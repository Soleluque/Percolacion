#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int poblar (int *red, float p, int dim);
int clasificar(int *red,int dim);
int etiqueta_falsa(int *red, int *hist, int S1, int S2, int i, int j, int dim);
int imprimir(int *red,int dim);
int percolacion(int *red, int dim);

int main(int argc,char** argv[])
{
  float p;
  int dim, i, j, S1, S2;
  int *red;
  int *hist;
  int *p1;
  int *p2;
  int *s;


  hist = malloc((dim*dim)/2);
  red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.
  p1 = malloc(dim);
  p2 = malloc(dim);
  s = malloc(dim);
  sscanf(argv[1],"%d",& dim);           //Busca el primero de los argumentos y lo usa como dim.
  sscanf(argv[2],"%f",& p);             //Busca el segundo de los argumentos y lo usa como p.

  poblar(red, p, dim);                  //Usa la funcion poblar.
  clasificar(red,dim);
  imprimir(red,dim);
  percolacion(red,dim);
  free(red);
  free(hist);
  free(p1);
  free(p2);
  free(s);


  return 0;
}




int poblar(int *red, float p, int dim)
{
  float random;
  int i,j;
  srand(time(NULL));


  for (i=0;i<dim*dim;i++)               //Toma como rango máximo la cantidad de celdas totales.

    { *(red+i)=0;                       //Asigna por defecto el valor <vacio>
      random=((float)rand())/((float) RAND_MAX);    //Valor aleatorio entre 0 y 1.
      if (random<p)
        *(red+i)=1;                       //Asigna como <lleno> si el valor aleatorio es menor.
    }

    for (i=0;i<dim;i++) // Imprimo la red para ver como quedo
      {for (j=0;j<dim;j++)
        {
          printf("%d ", *(red+dim*i+j));
        }
      printf("\n");
      }
  printf("\n");

  return 0;
}






int clasificar(int *red,int dim)
{
  int S1,S2,i,j,a;
  int frag = 2;
  int *hist;
  int minimo;
  int maximo;

  hist = malloc((dim*dim)/2);

  for(i=0;i<=(dim*dim)/2;i++) //genero historial
    {
      *(hist+i)=i;
    }




  if(*(red)) //primer lugar
    {
      *(red) = frag;
      frag++;

    }

  for(i=1;i<dim;i++) //primera fila sin primer lugar
  {
    S1= *(red+i-1);
    if(*(red+i) && S1)
    {
       *(red+i) = S1;

    }
    if(*(red+i) && !S1)
    {
      *(red+i)= frag;
      frag++;
    }


  }

  for(i=1;i<dim;i++)  //primera columna sin primer lugar
  {
    S2 = *(red+(i*dim-dim));
    if(*(red+(i*dim)) && S2)
    {
      *(red+(i*dim)) = S2;

    }
    if(*(red+(i*dim)) && !S2)
    {
      *(red+(i*dim))= frag;
      frag++;
    }

  }



  for(i=1;i<dim;i++) //todo el resto
  {
    for(j=1;j<dim;j++)
    {
      S1 = *(red+(i*dim+j-1));
      S2 = *(red+(i*dim+j-dim));

      if( *(red+(i*dim+j)) && (S1 || S2)) // Acá entran cuando los dos son 1 o cuando uno solo es 1. Los primeros tres if son si los dos son 1. Los otros dos si uno es 1.
        {
         if (S1 && S2 && (S2<S1)) //Los dos son 1 y S1 más grande
          {
            *(red+(i*dim+j)) = S2; //Cambio el lugar donde estoy parado por el minimo
            *(red+(i*dim+j-1))= S2; //Cambio al otro vecino por el minimo
            *(hist+S1)=-*(hist+S2); //Pongo el menos en el historial


          }
         if (S1 && S2 && (S1<S2)) //Los dos son 1 y S2 más grande
         {
           *(red+(i*dim+j)) = S1;
           *(red+(i*dim+j-dim))=S1;
           *(hist+S2)=-*(hist+S1);

         }

         if (S1 && S2 && S2==S1) //Los dos son 1 y son iguales, elijo arbitrariamente S2
         {
           *(red+(i*dim+j)) = S2;


         }
        if ((!S1) && S2) //S1 es cero y S2 es 1
        {
          *(red+(i*dim+j))= S2;

        }
        if (S1 && (!S2)) //S1 es 1 y S2 es cero
       {
         *(red+(i*dim+j))= S1;

       }
       }

      if ( *(red+(i*dim+j)) && (!S1 && !S2)) //los dos son cero
      {
        *(red+(i*dim+j))= frag;
        frag++;

      }


  }
}

/*for (i=0;i<=(dim*dim)/2;i++) // Imprimo historial para ver
  {
  printf("%d ",*(hist+i));
  }
  printf("\n");*/

etiqueta_falsa(red, hist, S1, S2, i, j,dim);

  for(i=0;i<dim*dim;i++) //cambio la red
  {
    a=*(red+i);
    *(red+i)=*(hist+a);

  }


return 0;

}

int etiqueta_falsa(int *red, int *hist, int S1, int S2, int i, int j, int dim)

  {

    for(i=1;i<dim;i++) //Cambio los valores del historial a positivos
    {
      for(j=1;j<dim;j++)
      {
        S1 = *(red+(i*dim+j-1));
        S2 = *(red+(i*dim+j-dim));
        while(*(hist+S1)<0 && S1)
        {
          *(hist+S1)=-*(hist+S1);

        }
        while(*(hist+S2)<0 && S2)
        {
          *(hist+S2)=-*(hist+S2);

        }


}
}

  /*for (i=0;i<=(dim*dim)/2;i++) // Imprimo historial para ver
    {
      printf("%d ",*(hist+i));
    }
    printf("\n");*/



    return 0;


}


int imprimir(int *red, int dim)
{ int i,j;

  printf("\n");
  for (i=0;i<dim;i++) // Imprimo la red para ver como quedo
    {for (j=0;j<dim;j++)
      {
        printf("%d ", *(red+dim*i+j));
      }
      printf("\n");
      }
      printf("\n");

return 0;
}

int percolacion(int *red, int dim) //Esta funcion me dice si percolo o no
{ int i, j,b;
  int *p1,*p2,*s;

  s = malloc(dim);
  p1 = malloc(dim);
  p2 = malloc(dim);

  for (i=0; i<dim;i++) //genero dos vectores con la primera y la ultima fila
  {
    *(p1+i)=*(red+i);
    *(p2+i)=*(red+(dim*(dim-1)+i));
  }

  for(i=0;i<dim;i++) //los imprimo para ver que onda
  {
    printf("%d ",*(p1+i));
  }

  printf("\n");

  for(i=0;i<dim;i++)
  {

    printf("%d ",*(p2+i)); //idem
  }

  for(i=0;i<dim;i++) //genero vector donde voy a guardar la informacion de si percolo o no
  {
    *(s+i)=0;
  }

  for (i=0;i<dim;i++) //si coincide algun numero pongo un 1 en otro vector
  {
    for (j=0; j<dim;j++)
    {
      if (*(p1+i) && *(p1+i)==*(p2+j))
      {
        b=*(p1+i);
        *(s+b)=*(s+b)+1;
      }

    }
  }

  printf("\n");
  for(i=0;i<dim;i++)
  {
    printf("%d ",*(s+i));
  }

  printf("\n");


    for(i=0;i<dim;i++) //si en el nuevo vector hay algun valor distinto de 0 aviso que percolo
    {
      if (*(s+i))
      {
        printf("He percolado =)");
        printf("\n");
        break;

      }
      }


  return 0;

}
