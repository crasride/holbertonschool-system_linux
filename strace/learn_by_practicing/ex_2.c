#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>
#include <signal.h>

void print_syscall_numbers(pid_t child_pid);

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
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	child_pid = fork();

	if (child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

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

		/* Trace the child using PTRACE_SYSCALL */
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
		{
			ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
			waitpid(child_pid, &status, 0);

			/* Check if it's a syscall entry */
			if (WIFSTOPPED(status))
			{
				/* Print the syscall number */
				print_syscall_numbers(child_pid);
			}
		}

		/* Print detailed status information */
		if (WIFEXITED(status))
		{
			printf("Exit status: %d\n", WEXITSTATUS(status));
		}
	}

	return (0);
}

/**
* print_syscall_numbers - Prints the syscall number of a process
*
* @child_pid: The PID of the process to be traced
*/
void print_syscall_numbers(pid_t child_pid)
{
	struct user_regs_struct regs;

	/* Get the register values using PTRACE_GETREGS */
	if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
	{
		perror("ptrace");
		exit(EXIT_FAILURE);
	}

	/* Print the syscall number */
	printf("%ld\n", (long)regs.orig_rax);
}
