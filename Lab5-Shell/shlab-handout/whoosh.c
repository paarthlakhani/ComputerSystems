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

static void run_script(script *scr) 
{
  int i;

  for( i = 0; i < scr->num_groups; i++)
    {
       int j;
       for( j= 0; j< scr->groups[i].repeats; j++ )
	run_group(&scr->groups[i]);
    }
}

static void run_group(script_group *group) 
{
    script_var* variable = NULL;
  if (group->result_to != NULL)
    {
        variable = group->result_to;
    }


  if (group->num_commands == 1) 
    {

      pid_t pid = Fork();
      if(pid == 0)
	{ 
	  Setpgid(0,0);
	  run_command(&group->commands[0]);
	}
      else
	{
	  int child_status;
	  Waitpid(pid,&child_status,0);

	  int result = 0;
	      
	  if(WIFEXITED(child_status))
	    {
	      result = WEXITSTATUS(child_status);
	    }
	  else if(WIFSIGNALED(child_status))
	    {
	      result = WTERMSIG(child_status);
	      result=result*-1;
	    }    
	      
	  if(variable!=NULL)
	    {
	      set_var( variable , result);
	    }
	}
    } 
  else 
    {
      int num_commands = group->num_commands;
      if(group->mode == 1) // And group
	{
	  //	  int num_commands = group->num_commands;
	  file_descriptor* d;
	  d = malloc(sizeof(int *) * (2*(num_commands-1)));
	   int i;
	  for( i = 0; i < num_commands - 1; i++)
	    {
	      Pipe(d[i].fds);
	    }

	  for( i = 0; i < num_commands; i++)
	    {
	      pid_t pid = Fork();
	      script_command *command = &group->commands[i];

	      if(pid == 0)
		{		  
		  if(i == 0 )
		    {
		      Dup2(d[0].fds[1],1);
		    }
		  else if( i == num_commands -1 )
		    {
		      Dup2(d[num_commands - 2].fds[0],0); // last file descriptor
		    }
		  else
		    {
		      Dup2(d[i-1].fds[0],0);
		      Dup2(d[i].fds[1],1);
		    }

		  close_pipes(d,num_commands);
		  Setpgid(0,0);
		  run_command(command);
		} // end of fork
	      else
		{
		  if(command->pid_to!=NULL)
		    {
		      set_var( command->pid_to , pid );
		    }
		}
	    } // for-loop
	  
	  close_pipes(d,num_commands);
	  for( i = 0 ; i < num_commands; i++)
	    {
	      int child_status;
	      Waitpid(-1,&child_status,0);
	      int result = 0;
	      
	      if(WIFEXITED(child_status))
		{
		  result = WEXITSTATUS(child_status);
		}
	      else if(WIFSIGNALED(child_status))
		{
		  result = WTERMSIG(child_status);
		}
	      
	      if(variable!=NULL)
		{
		  set_var( variable , result);
		}
	    }

	  free(d);
	} // End AND group
      else // OR group
	{
	  int i;
	  pid_t pids[num_commands];
	  
	  for( i = 0 ; i < num_commands; i++)
	    {
	      pid_t current_pid = Fork();
	      script_command *command = &group->commands[i];	      
	      
	      if(current_pid == 0)
		{
		  Setpgid(0,0);
		  run_command(command);	  
		}
	      else
		{
		  pids[i] = current_pid;
		  
		  if(command->pid_to!=NULL)
		    {
		      set_var( command->pid_to , pids[i] );
		    }
		}
	    } // end of the for loop
	  
	  pid_t pid = 0;

	      int child_status;
	      
	      // Which signum ? SIGTERM ? Why ?
	      // What's the exam average.

	      pid = Waitpid(-1,&child_status,0);
	      int result = 0;
	      
	      if(WIFEXITED(child_status))
		{
		  result = WEXITSTATUS(child_status);
		}
	      else if(WIFSIGNALED(child_status))
		{
		  result = -1*WTERMSIG(child_status);
		}
	      
	      if(variable!=NULL)
		{
		  set_var( variable , result);
		}

	  for( i = 0 ; i < num_commands ; i++)
	    {
	      if(pid!=0)
		{
		  if(pids[i]!=pid)
		    {
		      int status = 0;
		      kill(pids[i],SIGTERM);
		      Waitpid(pids[i],&status,0);
		    }
		}
	    }
	}
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

static void run_command(script_command *command) 
{
  const char **argv;  
  int i;

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

   free(argv);
}

/* You'll likely want to use this set_var function for converting a
   numeric value to a string and installing it as a variable's
   value: */

static void set_var(script_var *var, int new_value) {
  char buffer[32];
  free((void *)var->value);
   snprintf(buffer, sizeof(buffer), "%d", new_value);
  var->value = strdup(buffer);
}
