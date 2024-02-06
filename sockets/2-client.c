#include "socket.h"

/**
* main - Entry point
* @argc: Argument count
* @argv: Argument vector
* Return: 0 on success, 1 on failure
*/
int main(int argc, char *argv[])
{
	int client_socket;
	struct addrinfo hints, *result, *rp;
	/* Initialize hints structure */
	memset(&hints, 0, sizeof(struct addrinfo));

	/* Check command-line arguments */
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Set up hints data structure */
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	/* Resolve host name to IP address */
	if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0)
	{
		fprintf(stderr, "Invalid host address: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Iterate through the list of addresses and try to connect */
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		client_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (client_socket == -1)
			continue;

		if (connect(client_socket, rp->ai_addr, rp->ai_addrlen) != -1)
			break; /* Successfully connected */

		close(client_socket);
	}

	/* Check if connection was successful */
	if (rp == NULL)
	{
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	/* Connection successful */
	printf("Connected to %s:%s\n", argv[1], argv[2]);

	/* Free the memory allocated by getaddrinfo */
	freeaddrinfo(result);

	/* Close the client socket */
	close(client_socket);

	return (0);
}

