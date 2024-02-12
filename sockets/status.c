#include "socket.h"

/**
* verify_status - verify response and print response
* @res: response to check
*/
void verify_status(char *res)
{
	char *status;

	status = strchr(res, ' ');
	if (strncmp(status + 1, "404", 3) == 0)
		printf(" -> 404 Not Found\r\n");
	else if (strncmp(status + 1, "411", 3) == 0)
		printf(" -> 411 Length Required\r\n");
	else if (strncmp(status + 1, "422", 3) == 0)
		printf(" -> 422 Unprocessable Entity\r\n");
	else if (strncmp(status + 1, "201", 3) == 0)
	{
		printf(" -> 201 Created\r\n");
		free(res);
	}
	else if (strncmp(status + 1, "200", 3) == 0)
	{
		printf(" -> 200 OK\r\n");
		free(res);
	}
}
