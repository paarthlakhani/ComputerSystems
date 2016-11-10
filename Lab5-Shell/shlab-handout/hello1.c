
  int fdsA[2]; // process 1.
  
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
      Close(fdsA[0]);
      Execve("/usr/bin/wc", (char * const *)argv_other, environ);
    }













// Expected output.
static void exclusive_command(script_command *command, int current_input_output, int desired_input_output)
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
  pid_t pid;
  pid = Fork();

  if(pid == 0)
    {
      Dup2(current_input_output,desired_input_output);
      // How should I close the file
      Close(current_input_output);
      Execve(argv[0], (char * const *)argv, environ);
    }
  else
    {
      Close(current_input_output);
      int child_status;
      Waitpid(pid,&child_status,0);
    }
}














