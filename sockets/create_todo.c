#include "socket.h"

#define BUFFER_SIZE 1024

static int id;

/**
* handle_post - parses post request
* @body: the body string
* @content_length: length of body string
* Return: 0 on success else 1
*/
char *handle_post(char *body, short content_length)
{
	char *query, *k, *v, *ptr, *kv, *title = NULL;
	char buffer[BUFFER_SIZE], res[BUFFER_SIZE];
	char *created = "HTTP/1.1 201 Created\r\n", *length = "Content-Length: ";
	char *json = "Content-Type: application/json\r\n\r\n";
	char *request, *desc = NULL;
	struct todo_t *todo;
	size_t i;

	memset(buffer, 0, BUFFER_SIZE), memset(res, 0, BUFFER_SIZE);
	body[content_length] = 0, query = strtok_r(body, "&", &ptr);
	while (query)
	{
		k = strtok_r(query, "=", &kv), v = strtok_r(NULL, "=", &kv);
		if (strcasecmp(k, "title") == 0)
			title = v;
		else if (strcasecmp(k, "description") == 0)
			desc = v;
		query = strtok_r(NULL, "&", &ptr);
	}
	if (!title || !desc)
		return ("HTTP/1.1 422 Unprocessable Entity\r\n\r\n");
	todo = malloc(sizeof(struct todo_t)), todo->id = id;
	if (!todo)
		return ("HTTP/1.1 422 Unprocessable Entity\r\n\r\n");
	id++, todo->title = strdup(title);
	todo->description = strdup(desc), todo->next = NULL, verify_root(todo);
	sprintf(res, "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
			todo->id, todo->title, todo->description);
	sprintf(buffer, "%s%s%lu\r\n%s", created, length, strlen(res), json);
	request = (char *)malloc(sizeof(char) * (strlen(res) + strlen(buffer)));

	for (i = 0; i < strlen(res) + strlen(buffer); i++)
	{
		if (i >= strlen(buffer))
			request[i] = res[i - strlen(buffer)];
		else
			request[i] = buffer[i];
	}
	request[i] = 0;
	return (request);
}

/**
* handle_http_request - print request infos
* @client_socket: socket to refer
*/
void handle_http_request(int client_socket)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received, bytes_sent, content_length = 0;
	char *path, *header, *response, *start, *ptr, *key, *value, *body;
	char *start_lines, *method;

	memset(buffer, 0, sizeof(buffer));
	bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received == -1)
		perror("recv"), exit(EXIT_FAILURE);
	body = strstr(buffer, "\r\n\r\n");
	if (strlen(body) > 0)
		*body = 0, body += strlen("\r\n\r\n");
	start_lines = strtok_r(buffer, "\r\n", &start);
	method = strtok(start_lines, " ");
	path = strtok(NULL, " ");
	printf("%s %s", method, path);
	if (strcasecmp(method, "POST") != 0 || strcasecmp(path, "/todos") != 0)
		response = "HTTP/1.1 404 Not Found\r\n\r\n";
	else
	{
		header = strtok_r(NULL, "\r\n\r\n", &start);
		while (header)
		{
			key = remove_whitespace(strtok_r(header, ":", &ptr));
			value = remove_whitespace(strtok_r(NULL, "\r\n", &ptr));
			if (!strcasecmp(key, "Content-Length"))
				content_length = atoi(value);
			header = strtok_r(NULL, "\r\n", &start);
		}
		if (content_length == 0)
			response = "HTTP/1.1 411 Length Required\r\n\r\n";
		else
			response = handle_post(body, content_length);
	}
	bytes_sent = send(client_socket, response, strlen(response), 0);
	if (bytes_sent == -1)
		perror("send"), exit(EXIT_FAILURE);
	verify_status(response);
	close(client_socket);
}

/**
* main - opens an IPv4/TCP socket, and listens to traffic on port 8080
*
* Return: exit status
*/
int main(void)
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len;
	char *client_ip;

	setbuf(stdout, NULL);
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);

	if (bind(server_socket, (struct sockaddr *)&server_addr,
				sizeof(server_addr)) < 0)
		perror("Error binding"), exit(EXIT_FAILURE);
	if (listen(server_socket, 5) < 0)
		perror("Error listening"), exit(EXIT_FAILURE);
	printf("Server listening on port 8080\n");
	while (1)
	{
		client_len = sizeof(client_addr);
		client_socket = accept(server_socket,
				(struct sockaddr *)&client_addr, &client_len);
		if (client_socket < 0)
			perror("Error accepting connection"), exit(EXIT_FAILURE);
		client_ip = (char *)malloc(sizeof(char) * INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
		printf("%s ", client_ip);
		handle_http_request(client_socket);
		free(client_ip);
	}
	close(server_socket);
	return (0);
}
