#include "socket.h"

#define BUFFER_SIZE 1024

/**
* setup_server_socket - Create and set up the server socket
* Return: The server socket
*
*/
int setup_server_socket(void)
{
	int server_socket;
	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr));
	/* Create socket */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	/* Configure server address */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8080);

	/* Bind the socket to the address and port */
	if (bind(server_socket, (struct sockaddr *)&server_addr,
			sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	/* Listen for incoming connections */
	if (listen(server_socket, 1) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	fflush(stdout);

	return (server_socket);
}

/**
* accept_cli_connection - Accept a connection with a client
* @server_socket: Server socket
* @client_addr: Client address
* @client_addr_len: Client address length
* Return: The client socket
*/
int accept_cli_connection(int server_socket, struct sockaddr_in *client_addr,
							socklen_t *client_addr_len)
{
	/* Get the client's IP address */
	char client_ip[INET_ADDRSTRLEN];
	/* Accept a connection with a client */
	int client_socket = accept(server_socket, (struct sockaddr *)client_addr,
							client_addr_len);

	/* Check for connection error */
	if (client_socket < 0)
	{
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	inet_ntop(AF_INET, &client_addr->sin_addr, client_ip, INET_ADDRSTRLEN);

	/* Log the client connection */
	printf("Client connected: %s\n", client_ip);
	fflush(stdout);

	return (client_socket);
}
