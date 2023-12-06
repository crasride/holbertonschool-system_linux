#include "strace.h"

int trace_and_print_syscalls(const char *command)
{
	pid_t child_pid;
	int status, print_next_syscall = 0;
	struct user_regs_struct user_registers;

	if (command == NULL)
	{
		fprintf(stderr, "Usage: %s command\n", __FILE__);
		return (-1);
	}

	/* Disable buffering on stdout */
	setvbuf(stdout, NULL, _IONBF, 0);

	/* Create a child process */
	child_pid = fork();

	if (child_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (child_pid == 0)
	{
		/* Child process */
		if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
		{
			perror("ptrace");
			return -1;
		}
		raise(SIGSTOP);
		execlp(command, command, NULL);
		perror("execlp");
		return (-1);
	}
	else
	{
		/* Parent process */
		waitpid(child_pid, &status, 0);

		while (WIFSTOPPED(status)) {
			/* Get the registers of the child process */
			ptrace(PTRACE_GETREGS, child_pid, 0, &user_registers);

			/* Check if the child has stopped due to a system call */
			if (WSTOPSIG(status) == SIGTRAP && user_registers.orig_rax < sizeof(syscalls_64) / sizeof(syscalls_64[0]))
			{
				if (print_next_syscall)
				{
					/* Get the system call number and print its name */
					int syscall_number = user_registers.orig_rax;
					printf("%s\n", syscalls_64[syscall_number].name);
				}
				print_next_syscall = !print_next_syscall;

				/* Continue the execution of the child process */
				ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
				waitpid(child_pid, &status, 0);
			}
			else
			{
				/* Continue the execution of the child process */
				ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
				waitpid(child_pid, &status, 0);
			}
		}
		return (0);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s command\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (trace_and_print_syscalls(argv[1]) == -1)
	{
		fprintf(stderr, "Failed to trace syscalls\n");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
