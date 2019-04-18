#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>


int poblar (int *red, float p, int dim, int sem);
int clasificar(int *red,int dim);
int etiqueta_verdadera(int *historial, int s);
int percolacion(int *red, int dim);
int imprimir(int *red,int dim);


//Función principal:
int main(int argc,char*argv[])
{ float p;
  int dim, i,j,k,sem,semm=10;
  int *red,*dims;
  float *p_c;
  char *ns_n;
  double *ns_p;
  //int *per;


  //sscanf(argv[1],"%d",& dim);           //Busca el primero de los argumentos y lo usa como dim.
  //sscanf(argv[2],"%f",& p);             //Busca el segundo de los argumentos y lo usa como p.


  FILE *n_s;



  p_c= malloc(6*sizeof(float));
  dims= malloc(6*sizeof(int));
  ns_n= malloc(sizeof("ns128.txt"));



  *(p_c)=0.5630; //p_c por dimension
  *(p_c+1)=0.5813;
  *(p_c+2)=0.5878;
  *(p_c+3)=0.5932;
  *(p_c+4)=0.5922;
  *(p_c+5)=0.5927;



  *(dims)=4;
  *(dims+1)=8;
  *(dims+2)=16;
  *(dims+3)=32;
  *(dims+4)=64;
  *(dims+5)=128;





for(i=0;i<6;i++)
  {
    sprintf(ns_n,"ns%d",*(dims+i));
    n_s= fopen(ns_n,"w");

    int *s,*ns;

    dim=*(dims+i);

    p=*(p_c+i)-0.1;


    red = malloc(dim*dim*sizeof(int));
    s=malloc(dim*dim*sizeof(int));
    ns=malloc(dim*dim*sizeof(int));
    ns_p= malloc(dim*dim*sizeof(double));
    //per=malloc(dim*dim*sizeof(int));
    sem=0;

    fprintf(n_s, "%d\n",dim);



    while(p<=(*(p_c+i)+0.1))
    {
      for(j=0;j<dim*dim;j++)
      {
        *(ns_p+j)=0;
      }

      for(sem=0;sem<semm;sem++)
      {

        poblar(red, p, dim,sem);

        clasificar(red,dim);


        for(j=0;j<dim*dim;j++)
        {
          *(s+j)=0;
        }

        for(j=0;j<dim*dim;j++)
        {
          *(ns+j)=0;
        }

        /*for(j=0;j<dim*dim;j++)
        {
          *(per+j)=0;
        }*/

        /*for(k=0;k<dim;k++) //me genero un vector que me va a decir que clusters percolaron
        { for(j=0;j<dim;j++)
          { if (*(red+k) == *(red+(dim-1)*dim+j) && *(red+k))
            { *(per+*(red+k))+=1;
            }
          }
        }*/


        for(j=0;j<dim*dim;j++) //en la posicion i de s va a estar el tamaño del cluster i, pero no va a considerar los que percolaron
          {
            if(*(red+j) /*&& !*(per+j)*/)
            {
              *(s+*(red+j))+= 1;
            }
          }



          for(j=0;j<dim*dim;j++) //en la posicion i me va a decir cuantos cluster de tamaño i tengo. Aca los seteo a 1 los ocupado y abajo les sumo 1 si se repitan
          {
            if(*(s+j))
            {
              *(ns+*(s+j)) +=1;
            }
          }

          for(j=0;j<dim*dim;j++)
          {
            for(k=0;k<dim*dim;k++)
            {
              if(*(s+j) & (*(s+j)==*(s+k)))
              {
                *(ns+*(s+j)) += 1;
              }
              break;
            }
          }

          for(j=0;j<dim*dim;j++)
          {
            *(ns_p+j)+= *(ns+j);
          }



          /*printf("Para probabilidad %f para dimension L= %d , el vector s es = ",p,dim);
          for(j=0;j<dim*dim;j++)
          {
            printf("%d ",*(s+j));

          }
          printf("Y el vector ns es = ");

          for(j=0;j<dim*dim;j++)
          {
            printf("%d ",*(ns+j));
          }

          printf("\n");

          imprimir(red,dim);
          printf("%f %d\n",p,dim );*/
        }




        for(j=0;j<dim*dim;j++)
        {
          *(ns_p+j)= *(ns_p+j)/semm;
        }

        fprintf(n_s,"%f\n",p);
        //fprintf(text,"s ns\n");

        for(j=0;j<dim*dim;j++)
        {
          if(*(ns_p+j))
          {
            fprintf(n_s, "%d %f\n",j,*(ns_p+j));
          }
        }

        fprintf(n_s,"\n");
        p=p+0.05;
        }

        fclose(n_s);

    }





  free(red);
  free(dims);

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
