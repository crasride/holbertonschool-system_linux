#include "strace.h"

static void error(const char *error_msg);
static void trace_child(pid_t pid, char **argv, char **envp);
static void trace_parent(pid_t pid);

static void error(const char *error_msg)
{
	fprintf(stderr, "ERROR: %s\n", error_msg);
	exit(EXIT_FAILURE);
}

static void trace_child(pid_t pid, char **argv, char **envp)
{
	if (ptrace(PTRACE_TRACEME, pid, 0, 0) == -1)
		error("ptrace");

	execve(argv[1], &argv[1], envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

static void trace_parent(pid_t pid)
{
	int status;
	struct user_regs_struct user_regs;
	size_t alternator = 0;

	puts("execve");

	while (!WIFEXITED(status))
	{
		alternator++;
		ptrace(PTRACE_SYSCALL, pid, 0, 0);
		wait(&status);

		if (ptrace(PTRACE_GETREGS, pid, 0, &user_regs) == 0 && (alternator % 2 == 0))
		{
			printf("%s", syscalls_64_n[user_regs.orig_rax].name);
			if (user_regs.orig_rax != 1)
				printf("\n");
		}

		if (user_regs.orig_rax == 1 && alternator % 2 == 1)
			printf("\n");

		fflush(stdout);
	}
}

int main(int argc, char **argv, char **envp)
{
	pid_t pid;

	if (argc < 2)
		error("No input programme provided");

	pid = fork();
	if (pid == -1)
		error("Fork failed");
	else if (pid > 0)
	{
		trace_parent(pid);
		ptrace(PTRACE_DETACH, pid, NULL, NULL);
	}
	else if (pid == 0)
	{
		trace_child(pid, argv, envp);
	}

	return (EXIT_SUCCESS);
}
