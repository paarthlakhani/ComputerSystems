/*
    Author: Paarth Lakhani
    uid: u0936913

    Purpose:
    The purpose of the assignment is to match a sequence with a given set of conditions based on the tag provided.
    The tag might be -a, -b, or -c. Every tag has a different set of conditions.
    If a -t is provided, then the matching sequence needs to undergo the transformation according to the provided.

    Compile the program:
    gcc -O2 -o matchlab matchlab.c

    Run the Program
    ./matchlab kxxxxE
    ./matchlab bbbbbbbxxxxEHDF
    ./matchlab -t -b g523vvvvvvvvvHEB53
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

/*
  Main function that sets the flags.
 */
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

      if(a!=1 && b!=1 && c!=1)
	{
	  // not passd any flags. Default is -a
	  a = 1;
	}

      matchSequence(argv,argc,a,b,c,t);
    }

  return 0;
}

/*
  Depending on the flag set, it calls the appropriate function for checking the matching conditions.
 */
void matchSequence(char **argv,int argCount,int a, int b, int c, int t)
{

  if( a==1 )
    {
      matchA(argv,argCount,t);
    }
  else if( b==1 )
    {
      matchB(argv,argCount,t);
    }
  else if( c==1 )
    {
      matchC(argv,argCount,t);
    }
}

/*
  argv is a list of arguments given.
  For every argument in the argv array, it checks the validity of all the conditions of mode a.
  For mode a, we have,

  Match a sequence of (with nothing else before or after)

    any number (including zero) repetitions of the letter “k”;
    between 4 and 5 repetitions (inclusive) of the letter “x”; and
    an odd number of uppercase letters.

For matches, perform the following conversion:

    replace each character by the first one.

 */

void matchA(char **argv, int argCount, int t)
{
  int i;

  for(i=0; i<argCount; i++)
    {
      char *argument = argv[i];

      // Storing this for later transformation.
      char firstAl = *argument;
      int length = strlen(argument);
      char transformedData[length];
      memset(transformedData, '\0', sizeof(transformedData));
      strcpy(transformedData, argument);

      int index = 0;
      char compareAlphabet;
      
      // Checking the first condition
      while(*argument == 'k')
	{
	  argument++;
	}

      // Checking the second condition
      argument = alphabetCount(argument,4,5,'x');
      
      if(argument!= NULL)
	{
	  // Check the third condition
	  argument = oddUppercase(argument);
	  
	  if(argument!= NULL && strlen(argument) == 0)
	    {
	      // Do the conversion here.
	      if(t == 1)
		{
		  memset(transformedData,firstAl,strlen(transformedData));
		  printf("%s\n",transformedData);
		}
	      else
		printf("yes\n");
	    }
	  else
	    {
	      if(t == 0)
		printf("no\n");
	    }
	}
      else
	{
	  if(t == 0)
	    printf("no\n");
	}
    }
}

/*
  argv is a list of arguments given.
  For every argument in the argv array, it checks the validity of all the conditions of mode b.
  For mode b, we have,

  Match a sequence of (with nothing else before or after)

    between 1 and 2 repetitions (inclusive) of the letter “g”;
    between 1 and 3 (inclusive) decimal digits — call this sequence X;
    any odd number of repetitions of the letter “v”;
    an odd number of uppercase letters; and
    the same characters as the even-positioned characters in X.

For matches, perform the following conversion:

    add one “E” before each “G”.

 */
void matchB(char **argv, int argCount, int t)
{
  int i;

  for(i=0; i<argCount; i++)
    {
      char *argument = argv[i];
      char transformedString[strlen(argument)];
      memset(transformedString,'\0',sizeof(transformedString));
      strcpy(transformedString, argument);
      
      // Checking the first condition
      argument = alphabetCount(argument,1,2,'g');

      if(argument!=NULL)
	{
	  // Checking the second condition
	  char X[3];
	  memset(X,'\0',sizeof(X));
	  argument = decimalCount(argument,1,3,X);

	  if(argument!=NULL)
	    {
	      // Checking the third condition
	      argument = oddRepetitions(argument,'v');
	      if(argument!=NULL)
		{
		  // Checking the forth condition
		  argument = oddUppercase(argument);

		  if(argument!=NULL)
		    {
		      // Checking the fifth condition
		      int j = 0;

		      argument = checkXForB(argument,X);

		      if(argument!=NULL)
			{ 
			  if(strlen(argument)>0)
			    {
			      if(t == 0)
				printf("no\n");
			    }
			  else if(t == 0)
			      printf("yes\n");
			  else
			    {
			      // t = 1 and strlen(argument)==0
			      // Do the transformation here.
			      char *previous = transformedString;
			      char *current = transformedString;
			      char *finalTransform = malloc(sizeof(current)*2);
			      char *finalTransformInitial = finalTransform;

			      while(*previous!='\0')
				{
				  current++;
				  memset(finalTransform,*previous,1);
				  finalTransform++;
				  if(*current == 'G')
				    {
				      memset(finalTransform,'E',1);
				      finalTransform++;
				    }

				  previous++;
				}
			      
			      printf("%s\n",finalTransformInitial);
			    }
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
      else
	{
	if(t == 0)
	  printf("no\n");
	}
    } // End of for loop
} // End of function


/*
  argv is a list of arguments given.
  For every argument in the argv array, it checks the validity of all the conditions of mode c.
  For mode c, we have,

  Match a sequence of (with nothing else before or after)

    any odd number of repetitions of the letter “f”;
    between 1 and 3 (inclusive) decimal digits — call this sequence X;
    between 1 and 2 repetitions (inclusive) of the letter “t”;
    an odd number of uppercase letters; and
    the same characters as X repeated 3 times.

For matches, perform the following conversion:

    remove every “G”.

 */
void matchC(char **argv, int argCount, int t)
{
  int i;
  
  for( i=0; i<argCount; i++)
    {
      char *argument = argv[i];
      char *transformedString = argv[i]; // This would be the transformed string.

      // Checking the first condition
      argument = oddRepetitions(argument,'f');

      if(argument!=NULL)
	{	  
	  // Checking the second condition
	  char X[3];
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
		      int equal = checkXForC(argument,X);
		      if(equal == 0)
			{
			  if(t == 0)
			    printf("yes\n");
			  else
			    {
			      //printf("Transformation needs to be done");
			      //printf("Transformed String is: %s",transformedString);
			      // Transformed String; finalTransform and finalTransformInitial

			      char *finalTransform = malloc(sizeof(transformedString));
			      char *finalTransformInitial = finalTransform;

			      while(*transformedString!='\0')
				{
				  if(*transformedString!='G')
				    {
				      memset(finalTransform,*transformedString,1);
				      finalTransform++;
				    }
				  transformedString++;
				}
			      printf("%s\n",finalTransformInitial);
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

/*
  Checks whether the lowerBounds<=(alphabetCount)<=upperBound.
  if the above is true then returns a strings pointing to the latest index, else returns null
 */

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

/*
  Checks whether the sequence has contains an odd number of uppercase alphabets.
 */
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

/*
  Checks whether the sequence contains between lowerBound and upperBound (inclusive) decimal digits - call this sequence X.
 */
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
  if(repetitionCount == 1)
    return argument;
  return NULL;
}

/*
  For mode b, for the last condition we need to check is whether the sequence has the same characters as the even-positioned characters in X.
 */
char* checkXForB(char*argument,char *X)
{

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
      return NULL;
    }
}

/*
  For mode b, for the last condition, we need to check whether the sequence has same characters as X repeated 3 times.
 */
int checkXForC(char *argument,char *X)
{
  char testX[(strlen(X)<<1)+strlen(X)];
  memset(testX,'\0',sizeof(testX));
  strcat(testX,X);
  strcat(testX,X);
  strcat(testX,X);
  return strcmp(argument,testX);
}
