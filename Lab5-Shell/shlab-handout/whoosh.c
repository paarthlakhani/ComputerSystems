/* This is the main file for the `whoosh` interpreter and the part
   that you modify. */

#include <stdlib.h>
#include <stdio.h>
#include "csapp.h"
#include "ast.h"
#include "fail.h"

typedef struct file_descriptor
{
  int fds[2];
}file_descriptor;

static void run_script(script *scr);
static void run_group(script_group *group);
static void run_command(script_command *command);
static void set_var(script_var *var, int new_value);
static void exclusive_command(script_command *command);
static void close_pipes(file_descriptor* d, int num_commands);

/* You probably shouldn't change main at all. */
int main(int argc, char **argv) {
  script *scr;
  
  if ((argc != 1) && (argc != 2)) {
    fprintf(stderr, "usage: %s [<script-file>]\n", argv[0]);
    exit(1);
  }

  scr = parse_script_file((argc > 1) ? argv[1] : NULL);
  run_script(scr);

  return 0;
}

static void run_script(script *scr) {
  if (scr->num_groups == 1)
 {
    run_group(&scr->groups[0]);
  } else 
    {
      printf("The number of groups are: %d",scr->num_groups);
    /* You'll have to make run_script do better than this */
    //fail("only 1 group supported");
  }
}

/*
static void close_pipes(  , file_descriptor d[])
{
  
}
*/ 

static void run_group(script_group *group) 
{
  /* You'll have to make run_group do better than this, too */
  if (group->repeats != 1)
    fail("only repeat 1 supported");
  if (group->result_to != NULL)
    fail("setting variables not supported");

  if (group->num_commands == 1) 
{
    run_command(&group->commands[0]);
  } 
  else 
    {
      if(group->mode == 0)
	{
	 printf("I am in group_single");
	}
      else if(group->mode == 1)
	{
	  int num_commands = group->num_commands;
	  //func_descriptor d[num_commands - 1];
	  file_descriptor* d;
	  d = malloc(sizeof(int *) * (2*(num_commands-1)));
	   int i;
	  for( i = 0; i < num_commands - 1; i++)
	    {
	      Pipe(d[i].fds);
	    }

	  for( i = 0; i < num_commands; i++)
	    {
	      if(Fork() == 0)
		{		  
		  script_command *command;
		  if(i == 0 )
		    {
		      command = &group->commands[0];
		      Dup2(d[0].fds[1],1);
		    }
		  else if( i == num_commands -1 )
		    {
		      command = &group->commands[num_commands - 1];
		      Dup2(d[num_commands - 2].fds[0],0); // last file descriptor
		    }
		  else
		    {
		      //Dup2(current_input_output,desired_input_output);
		    }
		  close_pipes(d,num_commands);
		   run_command(command);
		}
	    }
	  
	  
	  close_pipes(d,num_commands);
	  for( i = 0 ; i < num_commands; i++)
	    {
	      int child_status;
	      pid_t pid = Waitpid(-1,&child_status,0);
	    }
	  
	  free(d);
	  /*
	  script_command *command1 = &group->commands[0];
	  int first_fd[2];
	  Pipe(first_fd);
	  exclusive_command(command1,first_fd[1],1); // First command
	  
	  int i;
	    for(i = 1 ; i < group->num_commands-1;i++)
	    {
	      // Use Pipe and Dup2
	      //run_command(&group->commands[i]);
	      }

	  script_command* command_last = &group->commands[group->num_commands-1];
	  exclusive_command(command_last, first_fd[0],0); // Last command
	  */
	}
      else
	{
	   printf("I am in group_or");
	}
      int i = 0;
    }
}

static void close_pipes(file_descriptor* d, int num_commands)
{
  int i;
  for( i = 0 ; i < num_commands -1 ; i++)
    {
      Close(d[i].fds[0]);
      Close(d[i].fds[1]);      
    }
}

static void exclusive_command(script_command *command)
{
  const char **argv;
  int j;
  argv = malloc(sizeof(char *) * (command->num_arguments + 2));
  argv[0] = command->program;
  
  for (j = 0; j < command->num_arguments; j++) 
    {
      if (command->arguments[j].kind == ARGUMENT_LITERAL)
	argv[j+1] = command->arguments[j].u.literal;
      else
	{
	  argv[j+1] = command->arguments[j].u.var->value;
	}
    }

  argv[command->num_arguments + 1] = NULL;
  Execve(argv[0], (char * const *)argv, environ);

}

static void run_command(script_command *command) 
{
  const char **argv;
  int i;

  /*
  if(command->pid_to!=NULL)
    {
      printf("%s",command->pid_to->name);
      printf("%s",command->pid_to->value);
    }
  else
    {
      printf("There's no variable there.\n");
    }
  */

  argv = malloc(sizeof(char *) * (command->num_arguments + 2));
  argv[0] = command->program;

  for (i = 0; i < command->num_arguments; i++) 
    {
    if (command->arguments[i].kind == ARGUMENT_LITERAL)
      argv[i+1] = command->arguments[i].u.literal;
    else
      {
	argv[i+1] = command->arguments[i].u.var->value;
      }
    }
  
  argv[command->num_arguments + 1] = NULL;
  Execve(argv[0], (char * const *)argv, environ);
  
  /*
  printf("Arguments are: %d\n", (command->num_arguments));
  for (i = 0; i <(command->num_arguments); i++) 
    {
      printf("%s\n",argv[i]);
    }
  */
  // You have to create a shell pipeline.
  /* 
    pid_t process_id = Fork();
  
    if(process_id == 0) // child process.
      {
	Execve(argv[0], (char * const *)argv, environ);
      }
    else
      {
	int child_status;
	Waitpid(process_id,&child_status,0);
      }
   free(argv);
  */
}


/* This run_command function is a good start, but note that it runs
   the command as a replacement for the `whoosh` script, instead of
   creating a new process. */
/*
static void run_command(script_command *command)
{
  // I'll be having two pipes.
  // /bin/echo -n rumor | /usr/bin/wc -c

  int fdsA[2]; // process 1.
  //  int fdsB[2]; // process 2.
  
  pid_t pid;

  Pipe(fdsA);
  pid = Fork();
  int argument_count = 2;

  if( pid == 0)
    {
      const char **argv;
      argv = malloc(sizeof(char *) * (argument_count+2));
      argv[0] = "/bin/echo";
      argv[1] = "-n";
      argv[2] = "rumor";
      argv[3] =  NULL;
      Dup2(fdsA[1],1);
      Close(fdsA[1]);
      Execve("/bin/echo", (char * const *)argv, environ);
    }
  else
    {
      int arg_count = 3;
      const char **argv_other;
      argv_other = malloc(sizeof(char *) * (arg_count));
      argv_other[0] = "/usr/bin/wc";
      argv_other[1] = "-c";
      argv_other[2] = NULL;
      Dup2(fdsA[0],0);
      Close(fdsA[1]);
      Execve("/usr/bin/wc", (char * const *)argv_other, environ);
    }
}
*/

/* You'll likely want to use this set_var function for converting a
   numeric value to a string and installing it as a variable's
   value: */

static void set_var(script_var *var, int new_value) {
  char buffer[32];
  free((void *)var->value);
  snprintf(buffer, sizeof(buffer), "%d", new_value);
  var->value = strdup(buffer);
}
