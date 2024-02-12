#include "socket.h"

static struct todo_t *root;

/**
* verify_root - check if root is NULL
* @todo: todo to insert
*/
void verify_root(struct todo_t *todo)
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
