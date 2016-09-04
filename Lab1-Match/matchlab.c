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
char* alphabetCount(char *argument,short lowerBound,short upperBound, char testChar);
char* oddUppercase(char *argument);
char* decimalCount(char*argument, short lowerBound,short upperBound,char *X);
char* oddRepetitions(char*argument,char testChar);
char* checkXForB(char*argument,char *X);
int checkXForC(char*argument,char *X);

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

      //if(!(a && b && c))
      if(a!=1 && b!=1 && c!=1)
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

  // Creating a pointer array to store the answers.
  char *answers[argCount];

  for(i=0; i<argCount; i++)
    {
      char *argument = argv[i];
      
      // Storing this for later transformation.
      char firstAl = *argument;
      char *transformedData[strlen(argument)];
      memset(transformedData,'\0',sizeof(transformedData));
      strcpy(transformedData, argument);

      int index = 0;
      char compareAlphabet;
      
      // Checking the first condition
      while(*argument=='k')
	{
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
	      //answers[i] = "yes";
	      // Do the conversion here.
	      if(t==1)
		{
		  memset(transformedData,firstAl,strlen(transformedData));
		  printf("%s\n",transformedData);
		}
	      else
		printf("yes\n");
	    }
	  else
	    {
	      if(t==0)
		printf("no\n");
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




void matchB(char **argv, int argCount, int t)
{
  int i;

  for(i=0; i<argCount; i++)
    {
      char *argument = argv[i];
      
      // Checking the first condition
      argument = alphabetCount(argument,1,2,'g');
      
      if(argument!=NULL)
	{
	  // Checking the second condition
	  char *X[3];
	  memset(X,'\0',sizeof(X));
	  argument = decimalCount(argument,1,3,X);

	  if(argument!=NULL)
	    {
	      // Checking the third condition
	      argument = oddRepetitions(argument,'v');
	      //printf("The argument is:%s",argument);
	      if(argument!=NULL)
		{
		  // Checking the forth condition
		  argument = oddUppercase(argument);

		  if(argument!=NULL)
		    {
		      //printf("The argument is: %s",argument);
		      // Checking the fifth condition
		      int j = 0;
		      
		      /*printf("X is:%s\n",X);
		      
		      while(j<strlen(X))
			{
			  printf("Xs is:%c\n",X[j]);
			  j++;
			  }*/

		      argument = checkXForB(argument,X);
		      if(argument!=NULL)
			{
			  //printf("argument is:%s",argument);
			  
			  if(strlen(argument)>0)
			    {
			      if(t == 0)
				printf("no\n");
			    }
			  else if(t==0)
			      printf("yes\n");
			  else
			    {
			      printf("Transformation needs to be done here");
			      // t = 1 and strlen(argument)==0
			      // Do the transformation here.
			    }
			}
		      else
			if(t==0)
			  printf("no\n");
		    }
		  else
		    if(t == 0)
		      printf("no\n");
		}
	      else
		if(t == 0)
		  printf("no\n");
	    }
	  else
	    if(t == 0)
	      printf("no\n");
	}
      else
	if(t == 0)
	  printf("no\n");
    }
}

void matchC(char **argv, int argCount, int t)
{
  int i;
  
  for( i=0; i<argCount; i++)
    {
      char *argument = argv[i];
      
      // Checking the first condition
      argument = oddRepetitions(argument,'f');

      if(argument!=NULL)
	{	  
	  //printf("The argument is:%s",argument);
	  // Checking the second condition
	  char *X[3];
	  memset(X,'\0',sizeof(X));
	  argument = decimalCount(argument,1,3,X);
	  
	  if(argument!=NULL)
	    {
	      // checking the third condition
	      argument = alphabetCount(argument,1,2,'t');
	      
	      if(argument!=NULL)
		{
		  // Checking the forth condition
		  argument = oddUppercase(argument);

		  if(argument!=NULL)
		    {
		      // Checking the fifth condition
		      //printf("The argument is: %s\n",argument);
		      //printf("Sequence X is: %c\n",*X);
		      //printf("Sequence X is: %s\n",X);
		      //printf("I have to check the fifth condition");
		      int equal = checkXForC(argument,X);
		      if(equal == 0)
			{
			  if(t == 0)
			    printf("yes\n");
			  else
			    {
			      printf("Transformation needs to be done");
			    }
			}
		      else if(t == 0)
			printf("no\n");
		    }
		  else if(t == 0)
		    printf("no\n");
		  
		}
	      else if(t == 0)
		printf("no\n");
	    }
	  else if(t==0)
	    printf("no\n");
	}
      else if(t == 0)
	printf("no\n");
    }
}

// Returns a strings pointing to the latest index.
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
  uppercaseCount = uppercaseCount & 1;
  
  if(uppercaseCount == 1)
    return argument;
  return NULL;
}

// between lowerBound and upperBound (inclusive) decimal digits - call this sequence X.
char* decimalCount(char*argument, short lowerBound,short upperBound,char *X)
{
  int decimalCount = 0;
  char* XSequence[3];
  
  memset(XSequence,'\0',sizeof(XSequence));
  memcpy(XSequence,argument,3);
  
  while(*argument>=48 && *argument<=57)
    {
      decimalCount++;
      argument++;
    }
  
  if(decimalCount >= lowerBound && decimalCount <= upperBound)
    {
      // Copy the sequence here.
      memcpy(X,XSequence,decimalCount);
      return argument;
    }
  return NULL;
}

// any odd number of reptitions of the letter testChar
char* oddRepetitions(char*argument,char testChar)
{
  int repetitionCount = 0;
  
  while(*argument == testChar)
    {
      repetitionCount++;
      argument++;
    }
  // Check if the count is odd or even
  repetitionCount = repetitionCount & 1;
  //printf("The repetitions are:%d",repetitionCount);
  if(repetitionCount == 1)
    return argument;
  return NULL;
}

// You can improve this function.
char* checkXForB(char*argument,char *X)
{
  //printf("The argument is: %s\n",argument);
  //  printf("X is: %s",X);
  if(strlen(X)==1 || strlen(X)==2)
    {
      if(*X == *argument) // Checking only 0 position
	{
	  argument++;
	  return argument;
	}
	  return NULL;
    }
  else // length is 3
    {
      if(*X == *argument)
	{
	  X+=2;
	  argument++;
	  if(*X == *argument)
	    {
	      argument++;
	      return argument;
	    }
	  return NULL;
	}
    }
}

int checkXForC(char *argument,char *X)
{
  //printf("The length argument is: %d\n",strlen(argument));
  //printf("The string argument is: %s\n",argument);
  char *testX[strlen(X)*3];
  memset(testX,'\0',sizeof(testX));
  strcat(testX,X);
  strcat(testX,X);
  strcat(testX,X);
  //printf("The length is: %d\n",strlen(testX));
  //printf("The string is: %s\n",testX);

  //printf("The arguments are equal: %d",strcmp(argument,testX));
  return strcmp(argument,testX);
}
