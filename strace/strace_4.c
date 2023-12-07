#include "strace.h"

#define ENOSYS_ERROR -38

void print_params(struct user_regs_struct *regs);
void print_execve_params(pid_t child_pid, struct user_regs_struct *regs);
pid_t createTracedProcess(char **argv);
void traceSyscalls(pid_t child_pid);

void print_params(struct user_regs_struct *regs)
{
	size_t i;
	unsigned long param;
	syscall_t syscall = syscalls_64[regs->orig_rax];

	if (!regs)
		return;

	for (i = 0; i < syscall.nb_params; i++)
	{
		if (syscall.params[i] == VOID)
			continue;

		switch (i)
		{
		case 0:
			param = (unsigned long)regs->rdi;
			break;
		case 1:
			param = (unsigned long)regs->rsi;
			break;
		case 2:
			param = (unsigned long)regs->rdx;
			break;
		case 3:
			param = (unsigned long)regs->r10;
			break;
		case 4:
			param = (unsigned long)regs->r8;
			break;
		case 5:
			param = (unsigned long)regs->r9;
			break;
		default:
			return;
		}

		if (syscall.params[i] == VARARGS)
			printf("...");
		else
			printf("%#lx%s", param, (i < syscall.nb_params - 1) ? ", " : "");
	}
}

void print_execve_params(pid_t child_pid, struct user_regs_struct *regs)
{
	size_t i;
	char *param;

	if (!regs)
		return;

	printf("\"%s\", [", (char *)regs->rdi);

	for (i = 0; i < regs->rsi; i++)
	{
		if (i > 0)
			printf(", ");

		param = (char *)ptrace(PTRACE_PEEKDATA, child_pid, regs->rdx + i * sizeof(char *), 0);
		printf("\"%s\"", param);
	}

	printf("], [/* %lu vars */]", (unsigned long)regs->rdx);
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
				if (syscall_number == __NR_execve)
				{
					printf("execve(");
					print_execve_params(child_pid, &user_registers);
				}
				else
				{
					printf("%s(", syscalls_64[syscall_number].name);
					print_params(&user_registers);
				}
			}

		if (print_syscall_name && (long)user_registers.rax != ENOSYS_ERROR && call_count)
		{
			printf(") = %s%lx\n", user_registers.rax ? "0x" : "", (long)user_registers.rax);
		}

		ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		waitpid(child_pid, &status, 0);
		call_count = 1;
	}
}

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

	printf(") = ?\n");
	return (EXIT_SUCCESS);
}
