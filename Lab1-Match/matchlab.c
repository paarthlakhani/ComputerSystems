/*
    Author: Paarth Lakhani
    uid: u0936913
*/
#include<stdio.h>
#include<string.h>

void matchSequence(char **argv,int argc,int a, int b, int c, int t);
void matchA(char **argv, int argCount, int t);
void matchB(char **argv, int argCount, int t);
void matchC(char **argv, int argCount, int t);
char*  alphabetCount(char *argument,short lowerBound,short upperBound, char testChar);
char* oddUppercase(char *argument);

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
	  default:
	    printf("Invalid flag.Run the program again.!\n");
	    return 1;
	  } // end of switch statement
	  argc--;
	} // end of while loop

      if(!(a && b && c))
	{
	  // not passd any flags. Default is -a
	  a = 1;
	}

      // Do we assume that atleast one argument is given.!
      matchSequence(argv,argc,a,b,c,t);

    }

  

  return 0;
}


void matchSequence(char **argv,int argCount,int a, int b, int c, int t)
{
  if(a==1)
    {
      matchA(argv,argCount,t);
    }
  else if(b==1)
    {
      matchB(argv,argCount,t);
    }
  else if(c==1)
    {
      matchC(argv,argCount,t);
    }

}

void matchA(char **argv, int argCount, int t)
{
  int i;
  int countK = 0;
  int countX = 0;

  // Creating a pointer array to store the answers.
  char *answers[argCount];

  for(i=0; i<argCount; i++)
    {
      char *argument = argv[i];

      int index = 0;
      char compareAlphabet;
      
      // Checking the first condition
      while(*argument=='k')
	{
	  countK++;
	  argument++;
	}

      // Checking the second condition
      argument = alphabetCount(argument,4,5,'x');
      
      if(argument!=NULL)
	{
	  // Check the third condition
	  argument = oddUppercase(argument);
	  
	  if(argument!=NULL && strlen(argument) == 0)
	    {
	      //	      answers[i] = "yes";
	      // Do the conversion here.
	      if(t==1)
		{
		  printf("conversion needs to be done here.");
		}
	      else
		printf("yes\n");
	    }
	  else
	    {
	      if(t==0)
		printf("no\n");
	      //	      answers[i] = "no";
	    }
	}
      else
	{
	  if(t==0)
	    printf("no\n");
	  //	  answers[i] = "no";
	}
    }
}

// Returns a string, pointing to the latest index.
char*  alphabetCount(char *argument,short lowerBound,short upperBound, char testChar)
{

  int countTestChar = 0;

  while(*argument == testChar)
    {
      countTestChar++;
      argument++;
    }
  
  if(countTestChar>=lowerBound && countTestChar<=upperBound)
    return argument;
  return NULL;
}

char* oddUppercase(char *argument)
{
  int uppercaseCount = 0;

  while(*argument>=65 && *argument<=90)
    {
      uppercaseCount++;
      argument++;
    }
  
  // Find out upperCount to be an odd number.
  uppercaseCount = uppercaseCount &  1;
  
  if(uppercaseCount == 1)
    return argument;
  return NULL;
}


void matchB(char **argv, int argCount, int t)
{
  int i;


  for(i=0; i<argCount; i++)
    {
      
    }
}

void matchC(char **argv, int argCount, int t)
{
  int i;

  for( i=0; i<argCount; i++)
    {

    }
}
