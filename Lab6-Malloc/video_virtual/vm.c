#include "csapp.h"

int main()
{
  int *x = malloc(sizeof(int));
  *x = 10;
  
  if(Fork() == 0)
    printf("%p\n",x);
  else
    {
      *x = 20;
      printf("%p\n",x);
    }

  return 1;
}
