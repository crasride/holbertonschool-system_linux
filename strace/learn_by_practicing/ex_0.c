#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <unistd.h>

void print_status(int status);

/**
* main - Entry point
*
* @argc: The number of arguments
* @argv: The array of arguments
*
* Return: 0 on success, error code otherwise
*/
int main(int argc, char *argv[])
{
	pid_t child_pid;

	if (argc < 2)
	{
		/* No command-line arguments */
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* Create a new child process */
	child_pid = fork();

	/* Check if fork() was successful */
	if (child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	/* Check if we're the child process */
	if (child_pid == 0)
	{
		/* Child process */
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
		{
			perror("ptrace");
			exit(EXIT_FAILURE);
		}

		/* Execute the command */
		execvp(argv[1], &argv[1]);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		int status;

		waitpid(child_pid, &status, 0);

		/* Trace the child using PTRACE_SINGLESTEP */
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
		{
			printf("single step\n");
			ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL);
			waitpid(child_pid, &status, 0);
		}

		/* Print detailed status information */
		print_status(status);
	}

	return (0);
}

/**
* print_status - Prints detailed information about a process' status
*
* @status: The status of a process
*/
void print_status(int status)
{
	if (WIFEXITED(status))
	{
		/* Normal exit */
		printf("Exited with status %d\n", WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		/* ctrl + C */
		printf("Terminated by signal %d\n", WTERMSIG(status));
	}
	else if (WIFSTOPPED(status))
	{
		/* ctrl + Z */
		printf("Stopped by signal %d\n", WSTOPSIG(status));
	}
}
