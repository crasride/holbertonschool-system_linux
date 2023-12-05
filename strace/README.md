<h1 align="center">C - Strace</h1>

### Concepts
For this project, we expect you to look at these concepts:

* [[EYNTK] x86-64 Assembly](./x86-64%20Assembly.md)
* [[EYNTK] Strace](./strace.md)

## Resources
Read or watch:

* [Ptrace Tutorial](https://webdocs.cs.ualberta.ca/~paullu/C498/meng.ptrace.slides.pdf)
* [How does ptrace work](https://stackoverflow.com/questions/23928530/how-does-ptrace-work-in-linux/23929059#23929059)

man or help:

* `strace`
* `ptrace`

## Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

### General
* What is strace, and what is its purpose
* What is ptrace, and what possibilties it offers
* What are the differences between `PTRACE_SINGLESTEP` and `PTRACE_SYSCALL`
* How to read from and/or write to the memory of a process being traced

## Requirements
### General

* Allowed editors: `vi, vim, emacs`
* All your files will be compiled on Ubuntu 14.04 LTS
* Your programs and functions will be compiled with `gcc 4.8.4 `using the flags` -Wall -Werror -Wextra` and `-pedantic`
* All your files should end with a new line
* A README.md file, at the root of the folder of the project, is mandatory
* Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/holbertonschool/Betty/blob/master/betty-style.pl) and [betty-doc.pl](https://github.com/holbertonschool/Betty/blob/master/betty-doc.pl)
* You are not allowed to use global variables
* No more than 5 functions per file
* You are allowed to use the standard library
* All your header files should be include guarded
* In the examples given, please remember that some values may be specific to the computer they are tested on, especially for addresses

### More Info
## Data Structure
This project requires you to parse the files `/usr/include/asm/unistd_64.h`and `/usr/include/asm/unistd_32.h`, in order to get the number corresponding to each syscall. It also requires you to do some man-scraping to get information about syscalls prototypes, in order to know the return type of each syscall, as well as its number of arguments and their type.

Since this is a very time-consuming task, you are given a [header file](https://github.com/hs-hq/0x0B-strace.c), containing all these informations, allowing you to focus on the `ptrace` part of this project.

Please note that you are completely allowed to use your own data structure(s) and/or to modify the one provided to you, which is only here to help you getting started with `strace`, it does not contain everything you need for a complete `strace`.

### Quiz questions

<details>
<summary>Show quiz</summary>

## Question #0
### What is `strace`?


- [ ] `strace` is a tool one can use to trace all the function calls made by a running process


- [x] `strace` is a tool one can use to trace all the system calls made by a running process


- [ ] `strace` is a tool one can use to disassemble an executable file (elf format only)

## Question #1
### What is a system call?

- [x] The programmatic way in which a computer program requests a service from the kernel of the operating system it is executed on

- [ ] System calls are much faster than regular library functions

- [x] System calls provide an essential interface between a process and the operating system.

- [x] `execve` is a system call

- [x] `fork` is a system call

- [x] `read` is a system call

- [ ] `malloc` is a system call

- [ ] `printf` is a system call

- [ ] System calls only exist on Linux and Unix

- [X] Since the system calls are part of the OS, the application programs need to context switch from user space to kernel space whenever a system call is invoked

## Question #2
### What system call can we use to trace a running process?

- [ ] `strace`

- [x] `ptrace`

- [ ] `ktrace`

## Question #3
### What is `ptrace`?

- [x] A system call

- [x] The `ptrace` system call provides a means by which one process (the “tracer”) may observe and control the execution of another process (the “tracee”)

- [x] The `ptrace` system call provides a means by which one process (the “tracer”) may examine and change the tracee’s memory and registers

- [x] `ptrace` is primarily used to implement breakpoint debugging and system call tracing

- [x] It is an abbreviation of “process trace”

- [x] `ptrace` can only be found in some of Unix and Unix-like operating systems

</details>

## Tasks

### [0. Step #0 - Syscall number]
---
Write a program that executes and traces a given command.

* Usage:` ./strace_0 command [args...]`
* Each time a syscall is intercepted, you must print its number, followed by a new line
* You don’t have to handle the `PATH: command` will be a full path to a binary (Ex: `/bin/ls` and not `ls`)
* Your Makefile must implement a rule `strace_0`

```
alex@~/0x09-strace$ make strace_0
[...]
alex@~/0x09-strace$ ./strace_0 /bin/echo Holberton
59
12
21
9
21
2
5
9
3
21
2
0
5
9
10
9
9
3
9
9
158
10
10
10
11
12
12
2
5
9
3
5
9
1
Holberton
3
11
3
231
alex@~/0x09-strace$
```
### Repo:

* GitHub repository: `holbertonschool-system_linux`
* Directory: `strace`
* File: `Makefile`


### [1. Step #1 - Syscall name]
---
Write a program that executes and traces a given command.

* Usage: `./strace_1 command [args...]`
* Each time a syscall is intercepted, you must print its name, followed by a new line
* You don’t have to handle the `PATH: command `will be a full path to a binary (Ex: `/bin/ls` and not `ls`)
* Your Makefile must implement a rule `strace_1`

```
alex@~/strace$ make strace_1
[...]
alex@~/strace$ ./strace_1 /bin/echo Holberton
execve
brk
access
mmap
access
open
fstat
mmap
close
access
open
read
fstat
mmap
mprotect
mmap
mmap
close
mmap
mmap
arch_prctl
mprotect
mprotect
mprotect
munmap
brk
brk
open
fstat
mmap
close
fstat
mmap
writeHolberton

close
munmap
close
exit_group
alex@~/strace$
```
### Repo:

* GitHub repository: `holbertonschool-system_linux`
* Directory: `strace`
* File: `Makefile`

### [2. Step #2 - Return value]
--- 
Write a program that executes and traces a given command.
* Usage: `./strace_2 command [args...]`
* Each time a syscall is intercepted, you must print its name and its return value in hexadecimal, followed by a new line (see example)
* You don’t have to handle the `PATH: command` will be a full path to a binary (Ex:i `/bin/ls` and not `ls`)
* Your Makefile must implement a `rule strace_2`
* NOTES: It is impossible to retrieve the last system call return value, simply because it doesn’t return. So, just print a `?` like `strace` does.

```
alex@~/0x09-strace$ make strace_2
[...]
alex@~/0x09-strace$ ./strace_2 /bin/echo Holberton
execve = 0
brk = 0xa98000
access = 0xfffffffffffffffe
mmap = 0x7fead97b8000
access = 0xfffffffffffffffe
open = 0x3
fstat = 0
mmap = 0x7fead97ad000
close = 0
access = 0xfffffffffffffffe
open = 0x3
read = 0x340
fstat = 0
mmap = 0x7fead91cf000
mprotect = 0
mmap = 0x7fead958d000
mmap = 0x7fead9593000
close = 0
mmap = 0x7fead97ac000
mmap = 0x7fead97aa000
arch_prctl = 0
mprotect = 0
mprotect = 0
mprotect = 0
munmap = 0
brk = 0xa98000
brk = 0xab9000
open = 0x3
fstat = 0
mmap = 0x7fead9621000
close = 0
fstat = 0
mmap = 0x7fead97b7000
writeHolberton
 = 0xa
close = 0
munmap = 0
close = 0
exit_group = ?
alex@~/0x09-strace$
```
### Repo:

* GitHub repository: `holbertonschool-system_linux`
* Directory: `strace`
* File: `Makefile`

### [3. Step #3 - Raw parameters]
---
Write a program that executes and traces a given command.
* Usage: `./strace_3 command [args...]`
* Each time a syscall is intercepted, you must print its name, its parameters in hexadecimal, and its return value in hexadecimal, followed by a new line (see example)
* There’s a difficulty: The number of parameters is not the same for all syscalls…
* You DON’T have to handle varargs:
	* In case of a variadic system call, just print ...
	* Example with the `ioctl` syscall: `ioctl(0x3, 0x1, ...) = 0 `(These are random values…)
* You don’t have to handle the PATH: command will be a full path to a binary (Ex: `/bin/ls` and not `ls`)
* Your Makefile must implement a rule `strace_3`
* NOTE: Refer to the previous`x86-64_Assembly` project to know where to find the arguments
```
alex@~/0x09-strace$ make strace_3
[...]
alex@~/0x09-strace$ ./strace_3 /bin/echo Holberton
execve(0, 0, 0) = 0
brk(0) = 0x9a2000
access(0x7f9c5add9aa0, 0) = 0xfffffffffffffffe
mmap(0, 0x2000, 0x3, 0xffffffffffffffff, 0xffffffff, 0) = 0x7f9c5afdd000
access(0x7f9c5addc5b0, 0x4) = 0xfffffffffffffffe
open(0x7f9c5add9fd6, 0x80000) = 0x3
fstat(0x3, 0x7ffdec09fc80) = 0
mmap(0, 0xa484, 0x1, 0xffffffffffffffff, 0x3, 0) = 0x7f9c5afd2000
close(0x3) = 0
access(0x7f9c5add9aa0, 0) = 0xfffffffffffffffe
open(0x7f9c5afdb377, 0x80000) = 0x3
read(0x3, 0x7ffdec09fe20, 0x340) = 0x340
fstat(0x3, 0x7ffdec09fcd0) = 0
mmap(0, 0x3c82c0, 0x5, 0xffffffffffffffff, 0x3, 0) = 0x7f9c5a9f4000
mprotect(0x7f9c5abb2000, 0x200000, 0) = 0
mmap(0x7f9c5adb2000, 0x6000, 0x3, 0xffffffffffffffff, 0x3, 0x1be000) = 0x7f9c5adb2000
mmap(0x7f9c5adb8000, 0x42c0, 0x3, 0xffffffffffffffff, 0xffffffff, 0) = 0x7f9c5adb8000
close(0x3) = 0
mmap(0, 0x1000, 0x3, 0xffffffffffffffff, 0xffffffff, 0) = 0x7f9c5afd1000
mmap(0, 0x2000, 0x3, 0xffffffffffffffff, 0xffffffff, 0) = 0x7f9c5afcf000
arch_prctl(0x1002, 0x7f9c5afcf740) = 0
mprotect(0x7f9c5adb2000, 0x4000, 0x1) = 0
mprotect(0x606000, 0x1000, 0x1) = 0
mprotect(0x7f9c5afdf000, 0x1000, 0x1) = 0
munmap(0x7f9c5afd2000, 0xa484) = 0
brk(0) = 0x9a2000
brk(0x9c3000) = 0x9c3000
open(0x7f9c5ab7cae0, 0x80000) = 0x3
fstat(0x3, 0x7f9c5adb7b60) = 0
mmap(0, 0x1887f0, 0x1, 0xffffffffffffffff, 0x3, 0) = 0x7f9c5ae46000
close(0x3) = 0
fstat(0x1, 0x7ffdec0a03e0) = 0
mmap(0, 0x1000, 0x3, 0xffffffffffffffff, 0xffffffff, 0) = 0x7f9c5afdc000
write(0x1, 0x7f9c5afdc000, 0xaHolberton
) = 0xa
close(0x1) = 0
munmap(0x7f9c5afdc000, 0x1000) = 0
close(0x2) = 0
exit_group(0) = ?
alex@~/0x09-strace$
```
### Repo:

* GitHub repository: `holbertonschool-system_linux`
* Directory: `strace`
* File: `Makefile`
