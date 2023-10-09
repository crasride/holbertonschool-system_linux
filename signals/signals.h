#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Functions for Tasks*/
int handle_signal(void);

void (*current_handler_signal(void))(int);
void print_hello(int signum);
void set_print_hello(void);

int handle_sigaction(void);
void (*current_handler_sigaction(void))(int);
int trace_signal_sender(void);
int pid_exist(pid_t pid);

#endif /* SIGNALS_H */
