#include "socket.h"

#define BUFFER_SIZE 1024

/**
* handle_connection - Handle the connection with the client
* @client_socket: Client socket
*/
void handle_connection(int client_socket)
{
	char buffer[BUFFER_SIZE];
	char client_ip[INET_ADDRSTRLEN];
	ssize_t bytes_received;

	/* Print client IP address */
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	getpeername(client_socket, (struct sockaddr *)&client_addr, &client_addr_len);

	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", client_ip);

	/* Wait for incoming message from the client */
	bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received == -1)
	{
		perror("Error receiving message");
		exit(EXIT_FAILURE);
	}

	buffer[bytes_received] = '\0';

	/* Print the received message */
	printf("Message received: \"%s\"\n", buffer);

	/* Close the connection with the client */
	close(client_socket);
}

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
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12345);

	if (bind(server_socket, (struct sockaddr *)&server_addr,
								sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_socket, 1) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 12345\n");

	return (server_socket);
}

/**
* accept_client_connection - Accept a connection with a client
* @server_socket: Server socket
* @client_addr: Client address
* @client_addr_len: Client address length
* Return: The client socket
*/
int accept_client_connection(int server_socket, struct sockaddr_in *client_addr
							, socklen_t *client_addr_len)
{
	int client_socket = accept(server_socket, (struct sockaddr *)client_addr,
								client_addr_len);

	if (client_socket < 0)
	{
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	return (client_socket);
}

/**
* main - Create a server socket and handle the connection with a client
* Return: 0 on success, 1 on failure
*/
int main(void)
{
	int server_socket, client_socket;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;

	server_socket = setup_server_socket();

	client_addr_len = sizeof(client_addr);
	client_socket = accept_client_connection(server_socket, &client_addr,
					&client_addr_len);

	handle_connection(client_socket);
	close(server_socket);

	return (0);
}
