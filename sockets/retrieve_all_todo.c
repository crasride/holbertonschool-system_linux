#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "socket.h"

static int id;
static struct todo_t *root;

#define BUFFER_SIZE 1024
#define BUF_SIZE 600

/**
 * res_check - check response and print correct response
 * @res: response to check
 */
void res_check(char *res)
{
	char *code;

	code = strchr(res, ' ');
	if (strncmp(code + 1, "404", 3) == 0)
		printf(" -> 404 Not Found\r\n");
	else if (strncmp(code + 1, "411", 3) == 0)
		printf(" -> 411 Length Required\r\n");
	else if (strncmp(code + 1, "422", 3) == 0)
		printf(" -> 422 Unprocessable Entity\r\n");
	else if (strncmp(code + 1, "201", 3) == 0)
		printf(" -> 201 Created\r\n");
	else if (strncmp(code + 1, "200", 3) == 0)
		printf(" -> 200 OK\r\n");
}


/**
 * trim - trimming given string
 * @str: string to trim
 * Return: trimed string
 */
char *trim(char *str)
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
 * root_check - check if root is NULL
 * @todo: todo to insert
 */
void root_check(struct todo_t *todo)
{
	struct todo_t *node;

	if (!root)
		root = todo;
	else
	{
		node = root;
		while (node->next)
			node = node->next;
		node->next = todo;
	}
}

/**
 * handle_get - handle get request
 * @cs: client socket
 * Return: response
 */
char *handle_get(int cs)
{
	char buffer[BUF_SIZE], header[BUFFER_SIZE];
	char *ok = "HTTP/1.1 200 OK\r\n", *length = "Content-Length: ";
	char *json = "Content-Type: application/json\r\n\r\n";
	char *id = "\"id\"", *t = "\"title\"", *desc = "\"description\"";
	struct todo_t *todo = root;

	memset(buffer, 0, BUFFER_SIZE), memset(header, 0, BUFFER_SIZE);
	buffer[0] = '[';
	while (todo)
	{
		sprintf(buffer + strlen(buffer),
			"{%s:%d,%s:\"%s\",%s:\"%s\"}", id, todo->id, t,
			todo->title, desc, todo->description);
		if (todo->next)
			buffer[strlen(buffer)] = ',';
		todo = todo->next;
	}
	buffer[strlen(buffer)] = ']';
	sprintf(header, "%s%s%lu\r\n%s%s", ok, length, strlen(buffer), json, buffer);
	send(cs, header, strlen(header), 0);
	res_check(header);
	return (NULL);
}

/**
 * handle_post - parses post request
 * @body: the body string
 * @content_length: length of body string
 * @cs: client socket
 * Return: 0 on success else 1
 */
char *handle_post(char *body, short content_length, int cs)
{
	char *query, *k, *v, *ptr, *kv, *title = NULL;
	char buffer[BUFFER_SIZE], res[BUF_SIZE];
	char *created = "HTTP/1.1 201 Created\r\n", *length = "Content-Length: ";
	char *json = "Content-Type: application/json\r\n\r\n";
	char *desc = NULL;
	struct todo_t *todo;

	memset(buffer, 0, BUFFER_SIZE), memset(res, 0, BUF_SIZE);
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
	todo = calloc(1, sizeof(*todo)), todo->id = id;
	if (!todo)
		return ("HTTP/1.1 422 Unprocessable Entity\r\n\r\n");
	id++, todo->title = strdup(title);
	todo->description = strdup(desc), todo->next = NULL, root_check(todo);
	sprintf(res, "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
			todo->id, todo->title, todo->description);
	sprintf(buffer, "%s%s%lu\r\n%s%s", created, length, strlen(res), json, res);
	send(cs, buffer, strlen(buffer), 0);
	res_check(buffer);
	return (NULL);
}

/**
 * process_request - print request infos
 * @cs: socket to refer
 */
void process_request(int cs)
{
	char buffer[BUFFER_SIZE];
	ssize_t content_length = 0;
	char *path, *header, *resp, *start, *ptr, *key, *value, *body;
	char *s, *method;

	memset(buffer, 0, sizeof(buffer));
	recv(cs, buffer, sizeof(buffer) - 1, 0);
	body = strstr(buffer, "\r\n\r\n");
	if (strlen(body) > 0)
		*body = 0, body += strlen("\r\n\r\n");
	s = strtok_r(buffer, "\r\n", &start), method = strtok(s, " ");
	path = strtok(NULL, " "), printf("%s %s", method, path);
	if (strcasecmp(path, "/todos") != 0)
		resp = "HTTP/1.1 404 Not Found\r\n\r\n";
	else
	{
		header = strtok_r(NULL, "\r\n\r\n", &start);
		while (header)
		{
			key = trim(strtok_r(header, ":", &ptr));
			value = trim(strtok_r(NULL, "\r\n", &ptr));
			if (!strcasecmp(key, "Content-Length"))
				content_length = atoi(value);
			header = strtok_r(NULL, "\r\n", &start);
		}
		if (content_length == 0 && strcasecmp(method, "POST") == 0)
			resp = "HTTP/1.1 411 Length Required\r\n\r\n";
		else
		{
			if (strcasecmp(method, "POST") == 0)
				resp = handle_post(body, content_length, cs);
			else if (strcasecmp(method, "GET") == 0)
				resp = handle_get(cs);
		}
	}
	if (resp)
	{
		send(cs, resp, strlen(resp), 0), res_check(resp);
	}
	close(cs);
}


/**
 * main - opens an IPv4/TCP socket, and listens to traffic on port 8080
 *
 * Return: exit status
 */
int main(void)
{
	int server_socket, cs;
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
		cs = accept(server_socket,
				(struct sockaddr *)&client_addr, &client_len);
		if (cs < 0)
			perror("Error accepting connection"), exit(EXIT_FAILURE);
		client_ip = (char *)malloc(sizeof(char) * INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
		printf("%s ", client_ip);
		process_request(cs);
		free(client_ip);
	}
	close(server_socket);
	return (0);
}
