/*
    Author: Paarth Lakhani
    uid: u0936913
*/
#include<stdio.h>

int main( int argc, char **argv)
{
  
  int a = 0; // mode a
  int b = 0; // mode b
  int c = 0; // mode c
  int t = 0; // tansformation t
  char flag;
  
  if(argc == 1)
    {
      printf("There are no arguments provided. ");
      return 1;
    }
  else
    {
      // We now need to use the flags and take the arguments passing that.
      
      //while( argc > 1 && argv[0][0]=='-')
      argc--;

      while( (++argv)[0][0] == '-')
	{
	  flag = argv[0][1];
	  switch (flag)
	  {
	    case 'a':
	      a=1;
	      break;
	    case 'b':
	      b=1;
	      break;
	    case 'c':
	      c=1;
	      break;
	    case 't':
	      t=1;
	      break;
	  } // end of switch statement
	  argc--;
	} // end of while loop

      // Do we assume that atleast one argument is given.!
      matchSequence(argv,argc,int a,int b, int c);

    }

  

  return 0;
}


void matchSequence(char **argv,int argCount,int a, int b, int c, int t)
{
  if(a==1)
    {
      matchA(argv,t);

    }
  else if(b==1)
    {
      matchB(argv,t);
    }
  else if(c==1)
    {
      matchC(argv,t);
    }

}

void matchA(char **argv, int t)
{
  
}

void matchB(char **argv, int t)
{
  
}

void matchC(char **argv, int t)
{
  
}
