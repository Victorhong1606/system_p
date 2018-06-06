#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXLINE 100
#define MAXCOMMAND 100
char * commands[MAXCOMMAND];
char buffer[MAXLINE];
int num_command,
    i;

int main()
{
    //the reading and writing for the old and new pipe
	int new_fds[2];
	int old_fds[2];
    
	//first read in all the command
	num_command = 0;
	pid_t pid;
	while(fgets(buffer, MAXLINE-1,stdin))
	{
		
		if (strlen(buffer)>2)
		{
			if (buffer[strlen(buffer)-1] == '\n')
			{
				buffer[strlen(buffer)-1] = '\0';
			}
            //write the commands in
			commands[num_command] = (char *) malloc(MAXLINE);
			strcpy(commands[num_command], buffer);
			num_command ++;
		}
	}
	
	//execute command
	for (i = 0; i < num_command; ++i)
	{	
		
		if (i < num_command-1)
		{
            //the pipe for this command to write
			int ret = pipe(new_fds);
			if (ret != 0 )
			{
				perror("pipe error");
				return 1;
			}
		}
		
		fflush(stdout);
        //strat the fork
		pid = fork();
		if (pid == 0) //in child
		{	
            
            //read from the previous pipe
			if (i > 0 )
			{
				dup2(old_fds[0], 0);
				close(old_fds[0]);
         		close(old_fds[1]);

			}
			close(new_fds[0]);
            //write to the new pipe
			if (i < num_command-1)
			{
				
		         dup2(new_fds[1], 1);
		         close(new_fds[1]);
		         

			}
			//do the commend
			int ret = execlp(commands[i],commands[i] , NULL);
            
            //print error
			if (ret != 0)
			{
				perror("error execlp");
				exit(1);
			}
		}
		else{
            
			if (i > 0 )
			{
				//parent no longer needs the old pipe
				close(old_fds[0]);
         		close(old_fds[1]);

			}
			if (i < num_command-1)
			{
                //parent set the new pipe to become old pipe
				old_fds[0] = new_fds[0];
				old_fds[1] = new_fds[1];		         

			}
		}


	}
    //waiting for the child finish
	waitpid(pid,NULL,0);
    
    //free the commands
	for (i = 0; i < num_command; ++i)
	{
		free(commands[num_command]);
	}
	return 0;

}
