#ifndef _SOCKET_
#define _SOCKET_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
/* Include for inet_ntoa */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#include <ctype.h>

/**
* struct addrinfo - structure to store address information
* @ai_family: the address family for the new socket
* @ai_socktype: the type of socket
* @ai_protocol: the protocol for the new socket
* @ai_addrlen: the length of the socket address
* @ai_addr: the socket address
* @ai_next: pointer to the next node in the linked list
*/
struct addrinfo
{
	int ai_family;
	int ai_socktype;
	int ai_protocol;
	socklen_t ai_addrlen;
	struct sockaddr *ai_addr;
	struct addrinfo *ai_next;
	};

int setup_server_socket(void);
int accept_cli_connection(int server_socket, struct sockaddr_in *client_addr,
							socklen_t *client_addr_len);
char *remove_whitespace(char *str);

/**
* struct todo_t - todo linked list structure
* @id: id of current todo item
* @title: title of current todo item
* @description: description of current todo item
* @next: next todo item in the linked lists
*/
typedef struct todo_t
{
	int id;
	char *title;
	char *description;
	struct todo_t *next;
} todo_s;

void verify_status(char *res);
void verify_root(struct todo_t *todo);

#endif /* _SOCKET_*/
