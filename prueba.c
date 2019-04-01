#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int poblar (int *red, float p, int dim, float seed);
int clasificar(int *red,int dim);
int etiqueta_falsa(int *red, int *hist, int S1, int S2, int i, int j, int dim);
int imprimir(int *red,int dim);
int percolacion(int *red, int dim, float p);



int main(int argc,char** argv[])
{
  float p, seed;
  int dim, i, j, S1, S2;
  int *red;
  int *hist;
  int *s;
  float *pc;


  hist = malloc((dim*dim)/2);
  red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.
  s = malloc(5*dim);                    //El 5 es arbitrario, pero si pongo solo dim hace lio y se pisa la memoria
  pc = malloc(5*dim);                      //CON ESTE HAY QUE TENER CUIDADO, PORQUE NO SE REALMENTE QUE LARGO VA A TENER Y PUEDE LLEGAR A PISARSE CON ALGO. HABRIA QUE ESTIMARLO
  sscanf(argv[1],"%d",& dim);           //Busca el primero de los argumentos y lo usa como dim.
  sscanf(argv[2],"%f",& p);             //Busca el segundo de los argumentos y lo usa como p.
  sscanf(argv[3],"%f",& seed);

  poblar(red, p, dim, seed);                  //Usa la funcion poblar.
  clasificar(red,dim);
  imprimir(red,dim);
  percolacion(red,dim,p);
  free(red);
  free(hist);
  free(s);
  free(pc);


  return 0;
}




int poblar(int *red, float p, int dim, float seed)
{
  float random;
  int i,j;
  //srand(time(NULL));
  srand(seed); //Ahora le voy a dar una seed yo

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

int percolacion(int *red, int dim, float p) //Esta funcion me dice si percolo o no
{ int i, j,b,suma;
  int *s;
  float *pc;
  //float p;



  s = malloc(5*dim);
  suma=0;


  for(i=0;i<dim;i++) //aca voy a guardar la informacion sobre si percolo o no
  {
    *(s+i)=0;
  }

  for (i=0;i<dim;i++) //si coincide algun numero pongo un 1
  {
    for (j=0; j<dim;j++)
    {
      if (*(red+i) && *(red+i)==*(red+(dim*(dim-1)+j))) //comparo primera y ultima fila
      {
        b=*(red+i);
        *(s+b)=*(s+b)+1;
      }

    }
  }

  printf("\n");
  for(i=0;i<dim;i++) //Imprimo s para ver que onda
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
        break; //para que me avise la 1era vez que ve algo distinto de 0 y nada mas

      }
    }

    for(i=0;i<dim;i++) // Sumo los valores del pointer s. Si esto da distinto de 0, se que percolo
    {
      suma=suma+*(s+i);

    }
    printf("\n");
    printf("%d La suma es ",suma);
    printf("\n");

    printf("%f ",p );
    printf("\n");

    *(pc)=p;  //aca y con el for me creo mi pointer pc que va a contener a las probabilidades


    for(i=0;i<5*dim;i++) //ESTO ES UN PROBLEMA Y EL LUGAR 0 TAMBIEN, ESTIMO SERA PROBLEMA DE MEMORIA
    {
      *(pc+i)=0;
    }

    for (i=0;i<5*dim;i++) //lo imprimo a ver si esta bien
    {
      printf("%f ",*(pc+i));
    }

//El primer p lo hago por separado tambin porque necesito si o si dos para comparar

    if (suma) //si da distinto de 0 quiere decir que percolo, asi que le resto p/2
    {
      *(pc+1)=*(pc)-*(pc)/2;

    }
    else //al reves
    {
      *(pc+1)=*(pc)+*(pc)/2;
    }

    for(i=2;i<5*dim;i++) //el resto
    {
       while ( (*(pc+i)-*(pc+i-1)>0.01) || (*(pc+i)-*(pc+i-1)<-0.01) ) //tengo que tener en cuenta que en el paso posterior puede ser menor p
       {
         if(suma)
         {
           *(pc+i)=*(pc+i-1) - *(pc+i-1)/2;
         }
         else
         {
        *(pc+i)=*(pc+i-1) + *(pc+i-1)/2;
        }
      }

    }

    printf("\n");

    for(i=0;i<5*dim;i++)
    {
      printf("%f ",*(pc+i));

    }

    printf("\n");

    return 0;

}

//int pc(int dim, float p, int *s) //esto va a calcular pc

/*{ int i,suma;

  //printf("%d\n",suma );
//Me sigue tirando suma 0, lo que psaba antes con s. que poronga


   //while (p) //no sé, acá tengo que ponerle alguna condiciòn para que esto lo haga hasta alcanzar tal precision en p.
  //{ Algo de que la diferencia entre dos p sucesivos sea menor a 0.01. Quizas tenga que hacer un pointer donde voy almacenando los p que calculo y ahi si puedo sacar la diferencia entre dos sucesivos tranca.

  //printf("\n");

  /*for(i=0;i<dim;i++) //Imprimo s para ver que onda
  {
    printf("%d ",*(s+i));
  }*/

  //printf("\n");
  //printf("\n");

  /*for(i=0;i<dim;i++) // Sumo los valores del pointer s
  {
    suma=suma+*(s+i);

  }
  printf("%d La suma es ",suma);

  if (suma) //si da distinto de 0 quiere decir que percolo, asi que le resto p/2
  {
    p=p-p/2;
  }
  else //al reves
  {
    p=p+p/2;
  }
  printf("f\n",p);

  //}
  return 0;
}*/
