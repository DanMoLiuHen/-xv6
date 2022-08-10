#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

//read a line ,marked by line break
int readline(char *new_argv[32], int current_argc){
	char buf[1024];
	int n = 0;
	while(read(0, buf+n, 1))
	{
		//too many parameters,error
		if (n == 1023)
		{
			fprintf(2, "argument is too long\n");
			exit(1);
		}
		//read a row
		if (buf[n] == '\n')
			break;
		n++;
	}
	buf[n] = 0;
	if (n == 0)
		return 0;
	int offset = 0;
	while(offset < n)
	{
		new_argv[current_argc++] = buf + offset;
		while(buf[offset] != ' ' && offset < n)
			offset++;
		while(buf[offset] == ' ' && offset < n)
			buf[offset++] = 0;
	}
	return current_argc;
}

int main(int argc, char const *argv[])
{
	//if the parameter is less than 2 ,error
	if (argc < 2)
	{
		fprintf(2, "Usage: xargs command (arg ...)\n");
		exit(1);
	}
	char *command = malloc(strlen(argv[1]) + 1);
	char *new_argv[MAXARG];
	//
	strcpy(command, argv[1]);
	for (int i = 1; i < argc; ++i)
	{
		new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
		strcpy(new_argv[i - 1], argv[i]);
	}

	int current_argc;
	//read line by line 
	while((current_argc = readline(new_argv, argc - 1)) != 0)
	{
		new_argv[current_argc] = 0;
		//create a child to execute commands
		if(fork() == 0)
		{
			exec(command, new_argv);
			fprintf(2, "exec failed\n");
			exit(1);
		}
		wait(0);
	}
	exit(0);
}