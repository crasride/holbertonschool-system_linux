#include "socket.h"

/**
* main - Entry point
*
* Return: 0 on success, otherwise 1
*/
int main(void)
{
	int server_socket;
	struct sockaddr_in server_addr;

	/* Initialize server_addr structure */
	memset(&server_addr, 0, sizeof(server_addr));

	/* Create socket */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	/*Set up server address structure */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12345);

	/* Bind the socket to the address and port */
	if (bind(server_socket, (struct sockaddr *)&server_addr,
							sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}
	/* Listen for incoming connections */
	if (listen(server_socket, 0) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}
	/* Output server information */
	printf("Server listening on port 12345\n");
	/* Hang indefinitely (will be killed by a signal during correction) */
	while (1)
	{
		sleep(1);
	}
	/* Close the server socket (unreachable due to the infinite loop) */
	close(server_socket);

	return (0);
}
