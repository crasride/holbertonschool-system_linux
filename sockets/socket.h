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

int setup_server_socket(void);
int accept_cli_connection(int server_socket, struct sockaddr_in *client_addr,
							socklen_t *client_addr_len);
char *remove_whitespace(char *str);

#endif /* _SOCKET_*/
