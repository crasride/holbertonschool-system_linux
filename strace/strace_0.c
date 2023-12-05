#include "strace.h"

int main(int argc, char *argv[])
{

	pid_t child;
	int status;
	long orig_rax;
	struct user_regs_struct regs;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* Create a child process */
	child = fork();

	if (child == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child == 0) {
		/* Child process */
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
			perror("ptrace");
			exit(EXIT_FAILURE);
		}
		execvp(argv[1], &argv[1]);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		waitpid(child, &status, 0);

		while (WIFSTOPPED(status))
		{
			/* Get the system call number */
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			orig_rax = regs.orig_rax;

			/* Print the system call number */
			printf("%ld\n", orig_rax);

			/* Continue the execution of the child process */
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
			waitpid(child, &status, 0);
		}
	}

	return (0);
}
