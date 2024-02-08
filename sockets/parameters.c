#include "socket.h"

#define BUFFER_SIZE 1024

/**
* remove_whitespace - Remove leading and trailing whitespace from a string
* @str: The string to trim
* Return: The trimmed strin
*/
char *remove_whitespace(char *str)
{
	size_t len = strlen(str);
	char *end;

	if (len == 0)
		return (str);

	end = str + len - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;
	*(end + 1) = '\0';

	while (*str && isspace((unsigned char)*str))
		str++;

	return (str);
}

/**
* handle_http_request - Handle the incoming HTTP request
* @client_socket: Client socket
*/
void handle_http_request(int client_socket)
{
	char buffer[BUFFER_SIZE];
	char client_ip[INET_ADDRSTRLEN];
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	ssize_t bytes_received, encode;
	const char *response = "HTTP/1.1 200 OK\r\n\r\n";
	char *path, *header, *start, *ptr, *key, *value, *query, *body, *start_lines;

	getpeername(client_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", client_ip);
	fflush(stdout);

	memset(buffer, 0, sizeof(buffer));
	bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received == -1)
		perror("Error reading from socket"), exit(EXIT_FAILURE);
	printf("Raw request: \"%s\"\n", buffer), body = strstr(buffer, "\r\n\r\n");
	if (strlen(body) > 0)
		*body = 0, body += strlen("\r\n\r\n");
	start_lines = strtok_r(buffer, "\r\n", &start), strtok(start_lines, " ");
	path = strtok(NULL, " "), printf("Path: %s\n", path);
	header = strtok_r(NULL, "\r\n\r\n", &start);
	while (header)
	{
		key = remove_whitespace(strtok_r(header, ":", &ptr));
		value = remove_whitespace(strtok_r(NULL, "\r\n", &ptr));
		if (!strcasecmp(key, "Content-Type"))
			if (!strcasecmp(value, "application/x-www-form-urlencoded"))
				encode = 1;
		header = strtok_r(NULL, "\r\n", &start);
	}
	if (encode)
	{
		query = strtok_r(body, "&", &start);
		while (query)
		{
			key = strtok_r(query, "=", &ptr);
			value = strtok_r(NULL, "=", &ptr);
			printf("Body param: \"%s\" -> \"%s\"\n", key, value);
			query = strtok_r(NULL, "&", &start);
		}
	}

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
