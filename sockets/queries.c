#include "socket.h"

#define BUFFER_SIZE 1024

/**
* handle_http_request - Handle the incoming HTTP request
* @client_socket: Client socket
*/
void handle_http_request(int client_socket)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;
	/* Parse and print the breakdown of the first line of the HTTP request */
	char *path, *query, *key, *value, *next, *rest;
	/* Send back a simple HTTP 200 OK response */
	const char *response = "HTTP/1.1 200 OK\r\n\r\n";

	/* Receive the HTTP request from the client */
	bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_received < 0)
	{
		perror("Error reading from socket");
		close(client_socket);
		return;
	}
	buffer[bytes_received] = '\0';

	/* Print the raw received HTTP request */
	printf("Raw request: \"%s\"\n", buffer);

	/* Extract the path from the buffer*/
	strtok(buffer, " ");
	path = strtok(NULL, " ");
	path = strtok_r(path, "?", &next);
	printf("Path: %s\n", path);

	/* Extract the query from the buffer and print query parameters */
	query = strtok_r(NULL, "&", &next);
	while (query)
	{
		key = strtok_r(query, "=", &rest);
		value = strtok_r(NULL, "=", &rest);
		printf("Query: \"%s\" -> \"%s\"\n", key, value);
		query = strtok_r(NULL, "&", &next);
	}
	/* Send the HTTP response */
	fflush(stdout);
	send(client_socket, response, strlen(response), 0);
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
