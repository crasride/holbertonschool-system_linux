#include "socket.h"

/* Assuming IPv4 o IPv6 addresses */
#define BUFFER_SIZE 46

/**
* initialize_Server - Initialize the server
* @server_addr: Server address structure
* @server_socket: Server socket
*/
void initialize_Server(struct sockaddr_in *server_addr, int *server_socket)
{
	/* Initialize server_addr structure */
	memset(server_addr, 0, sizeof(*server_addr));

	/* Create socket */
	*server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (*server_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	/* Configure the server address structure */
	server_addr->sin_family = AF_INET;
	server_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr->sin_port = htons(12345);

	/* Bind socket to address and port */
	if (bind(*server_socket, (struct sockaddr *)server_addr,
		sizeof(*server_addr)) < 0)
	{
		perror("Linking failure");
		exit(EXIT_FAILURE);
	}

	/* Listen for incoming connections */
	if (listen(*server_socket, 1) < 0)
	{
		perror("Listening failure");
		exit(EXIT_FAILURE);
	}

	/* Print server information */
	printf("Server listening on port 12345\n");
}

/**
* accept_Connection - Accept incoming connections
* @server_socket: Server socket
*/
void accept_Connection(int server_socket)
{
	while (1)
	{
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_socket;
		char client_ip[INET6_ADDRSTRLEN];
		char buffer[BUFFER_SIZE];

		/* Accept incoming connection */
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
							&client_addr_len);
		if (client_socket < 0)
		{
			perror("Failure to accept");
			exit(EXIT_FAILURE);
		}

		/* Convert client IP address to display format */
		if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip,
					INET6_ADDRSTRLEN) == NULL)
		{
			perror("Inet_ntop falló");
			exit(EXIT_FAILURE);
		}
		/* Print the client's IP address */
		printf("Client connected: %s\n", client_ip);
		/* Receive data from the client */
		ssize_t bytesRead = recv(client_socket, buffer, BUFFER_SIZE, 0);

		if (bytesRead < 0)
		{
			perror("Error receiving data");
			exit(EXIT_FAILURE);
		}

		/* Close the connection */
		close(client_socket);
	}
}

/**
* main - Entry point
* Return: 0 on success, 1 on failure
*/
int main(void)
{
	int server_socket;
	struct sockaddr_in server_addr;

	/* Initialize the server */
	initialize_Server(&server_addr, &server_socket);

	/* Accept incoming connections continuously */
	accept_Connection(server_socket);

	/* Note: The server will not reach this point in the current implementation */
	close(server_socket);

	return (0);
}
