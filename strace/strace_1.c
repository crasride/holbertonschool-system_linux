#include "strace.h"
#include "syscalls.h"

/**
 * main - executes an trace a given command in argv[1]
 * @argc: argument count
 * @argv: argument vector
 * @envp: env string array
 * Return: 0 success, 1 on failure
 */

int main(int argc, char *argv[], char *envp[])
{
	pid_t child;
	int retval = 0, flag = 0;
	struct user_regs_struct regs;

	if (argc < 2)
		return (fprintf(stderr, "Bad usage dude\n"), -1);
	/* disable buffering */
	setbuf(stdout, NULL);
	/* create child process */
	child = fork();
	if (child == 0)
	{
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		raise(SIGSTOP);
		execve(argv[1], argv + 1, envp);
	}
	else
	{
		/* parent process */
		wait(&retval);
		if (WIFEXITED(retval))
			return (0);
		/* move on child process */
		ptrace(PTRACE_SYSCALL, child, 0, 0);
		while (wait(&retval) && !WIFEXITED(retval))
		{
			if (flag)
				printf("\n"), flag = 0;
			memset(&regs, 0, sizeof(regs));
			ptrace(PTRACE_GETREGS, child, 0, &regs);
			if (WSTOPSIG(retval) == SIGTRAP && (long)regs.rax == -38)
			{
				printf("%s", (char *)syscalls_64[(unsigned long)regs.orig_rax].name);
				flag = 1;
			}
			ptrace(PTRACE_SYSCALL, child, 0, 0);
		}
		printf("\n");
	}
	return (0);
}
