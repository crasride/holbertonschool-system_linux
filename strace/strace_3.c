#include "strace.h"

#define ENOSYS_ERROR -38

void print_syscall_params(struct user_regs_struct regs, int syscall_number);

pid_t createTracedProcess(char **argv);

void traceSyscalls(pid_t child_pid);

int main(int argc, char **argv)
{
	pid_t child_pid;

	setvbuf(stdout, NULL, _IONBF, 0);

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	child_pid = createTracedProcess(argv + 1);
	traceSyscalls(child_pid);

	printf(" = ?\n");
	return (EXIT_SUCCESS);
}

void print_syscall_params(struct user_regs_struct regs, int syscall_number)
{
    size_t i;
    unsigned long params[6];

    params[0] = regs.rdi;
    params[1] = regs.rsi;
    params[2] = regs.rdx;
    params[3] = regs.r10;
    params[4] = regs.r8;
    params[5] = regs.r9;

    printf("(");
    for (i = 0; i < syscalls_64[syscall_number].nb_params; ++i)
    {
        if (i > 0)
            printf(", ");
        printf("%s%lx", i == 0 ? "" : " ", params[i]);
    }
    printf(")");
}



pid_t createTracedProcess(char **argv)
{
	pid_t child_pid;

	child_pid = fork();

	if (child_pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (child_pid == 0)
	{
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		raise(SIGSTOP);
		execvp(argv[0], argv);
		perror("execvp");
		exit(EXIT_FAILURE);
	}

	return (child_pid);
}

void traceSyscalls(pid_t child_pid)
{
	int status, syscall_number, print_syscall_name, call_count = 0;
	struct user_regs_struct user_registers;

	waitpid(child_pid, &status, 0);
	ptrace(PTRACE_SYSCALL, child_pid, 0, 0);

	for (print_syscall_name = 0; !WIFEXITED(status); print_syscall_name ^= 1)
	{
		ptrace(PTRACE_GETREGS, child_pid, 0, &user_registers);

		if (!print_syscall_name && call_count)
		{
			syscall_number = user_registers.orig_rax;
			printf("%s", syscalls_64[syscall_number].name);
			print_syscall_params(user_registers, syscall_number);
		}

		if (print_syscall_name && (long)user_registers.rax != ENOSYS_ERROR && call_count)
			printf(" = %s%lx\n", user_registers.rax ? "0x" : "",
					(long)user_registers.rax);

		ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		waitpid(child_pid, &status, 0);
		call_count = 1;
	}
}
