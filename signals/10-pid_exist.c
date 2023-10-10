#include <signal.h>

/**
* pid_exist -Checks if a process with the specified PID exists.
* The 'kill' function is used to send signals to processes.
* In this case, it is used with signal 0, which does not send any signal
* but is used to check the existence of a process.
*
* @pid: The PID (Process ID) to check.
* Return: 1 if a process with the specified PID exists, 0 if it does not.
*/
int pid_exist(pid_t pid)
{
	return (kill(pid, 0) == 0);
}
