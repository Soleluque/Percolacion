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
  red = malloc(dim*dim*sizeof(int));
  sscanf(argv[1],"%d",& dim);
  sscanf(argv[2],"%f",& p);

  poblar(red, p, dim);
  imprimir(red,dim);
  return 0;
}




int poblar(int *red, float p, int dim)
{ float random;
  int i;
  srand(time(NULL));


  for (i=0;i<dim*dim;i=i+1)

    { *(red+i)=0;
      random=((float)rand())/((float) RAND_MAX);
      if (random<p)
      *(red+i)=1;
    }

  return 0;
}

int imprimir(int *red, int dim)
{ int i,j;

  for (i=0;i<dim;i=i+1)
    {for (j=0;j<dim;j=j+1)
      {
      printf("%d ", *(red+dim*i+j));
      }
    printf("\n");
    }

  return 0;
}
