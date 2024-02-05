#include "socket.h"

int main(void)
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len;

	/* Initialize server_addr structure */
	memset(&server_addr, 0, sizeof(server_addr));

	/* Create socket */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	/* Set up server address structure */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12345);

	/* Bind the socket to the address and port */
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
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

	/* Output server information */
	printf("Server listening on port 12345\n");

	/* Accept incoming connection */
	client_addr_len = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_socket < 0)
	{
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	/* Print client IP address */
	printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

	/* Close the connection */
	close(client_socket);
	close(server_socket);

	return (0);
}
