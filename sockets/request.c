#include "socket.h"

#define BUFFER_SIZE 1024

/**
* handle_http_request - Handle the incoming HTTP request
* @client_socket: Client socket
*/
void handle_http_request(int client_socket)
{
	char buffer[BUFFER_SIZE];
	char client_ip[INET_ADDRSTRLEN];
	/* Parse and print the breakdown of the first line of the HTTP request */
	char method[16], path[256], version[16];
	/* Send back a simple HTTP 200 OK response */
	const char *response = "HTTP/1.1 200 OK\r\n\r\n";
	ssize_t bytes_received;

	/* Print client IP address */
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	/* Get the client IP address */
	getpeername(client_socket, (struct sockaddr *)&client_addr, &client_addr_len);

	/* Convert client IP address to a string */
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", client_ip);

	/* Wait for incoming message from the client (HTTP request) */
	bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received == -1)
	{
		perror("Error receiving HTTP request");
		exit(EXIT_FAILURE);
	}

	/* Add null-terminator to the received message */
	buffer[bytes_received] = '\0';

	/* Print the raw received HTTP request */
	printf("Raw request: \"%s\"\n", buffer);

	if (sscanf(buffer, "%s %s %s ", method, path, version) != 3)
	{
		fprintf(stderr, "Invalid HTTP request\n");
		exit(EXIT_FAILURE);
	}
	printf("Method: %s\n", method);
	printf("Path: %s\n", path);
	printf("Version: %s\n", version);
	send(client_socket, response, strlen(response), 0);

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
	/* Accept a connection with a client */
	int client_socket = accept(server_socket, (struct sockaddr *)client_addr,
							client_addr_len);

	/* Check for connection error */
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

	/* Create and set up the server socket */
	server_socket = setup_server_socket();

	/* Continuously accept connections and handle HTTP requests */
	while (1)
	{
		/* Accept a connection with a client */
		client_addr_len = sizeof(client_addr);
		client_socket = accept_cli_connection(server_socket, &client_addr,
												&client_addr_len);

		/* Handle the HTTP request from the client */
		handle_http_request(client_socket);
	}

	/* Close the server socket (not reached in the current implementation) */
	close(server_socket);

	return (0);
}

