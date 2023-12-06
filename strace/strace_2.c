#include "strace.h"

#define ENOSYS_ERROR -38

/**
* main - Entry point
* @argc: Argument count
* @argv: Argument vector
*
* Return: EXIT_SUCCESS or EXIT_FAILURE
*/
int main(int argc, char **argv)
{
	pid_t child_pid;
	int status, syscall_number, print_syscall_name, call_count = 0;
	struct user_regs_struct user_registers;

	/* Disable buffering of stdout */
	setvbuf(stdout, NULL, _IONBF, 0);

	/* Check usage */
	child_pid = fork();

	/* Fork a child to execute the program to be traced */
	if (child_pid < 0)
		exit(EXIT_FAILURE);

	/* Child: Execute the program specified in argv[1] */
	else if (child_pid == 0)
	{
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		raise(SIGSTOP);
		argv[argc] = NULL;
		execvp(argv[1], argv + 1);
	}
	else
	{
		wait(&status);
		ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		/* Wait for child to stop on its first instruction */
		for (print_syscall_name = 0; !WIFEXITED(status); print_syscall_name ^= 1)
		{
			/* Enter next system call */
			ptrace(PTRACE_GETREGS, child_pid, 0, &user_registers);
			/* Print system call name */
			if (!print_syscall_name && call_count)
			{
				syscall_number = user_registers.orig_rax;
				/* Check if the system call number is valid */
				printf("%s", syscalls_64[syscall_number].name);
			}
			if (print_syscall_name && (long)user_registers.rax != ENOSYS_ERROR && call_count)
				/* Print system call return value */
				printf(" = %s%lx\n", user_registers.rax ? "0x" : "", (long)user_registers.rax);
			ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
			wait(&status);
			call_count = 1;
		}
	}
	printf(" = ?\n");
	return (EXIT_SUCCESS);
}

