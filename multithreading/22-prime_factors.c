#include "multithreading.h"

static pthread_mutex_t mutex;

/**
* mutex_init - Constructor function to initialize the mutex.
* It is registered as a constructor to ensure proper initialization at program
* start
*/
__attribute__((constructor))void initialize_mutex(void)
{
	pthread_mutex_init(&mutex, NULL);
}

/**
* mutex_destroy - Destructor function to destroy the mutex.
* It is registered as a destructor to ensure cleanup at program exit
*/
__attribute__((destructor))void _destroy(void)
{
	if (pthread_mutex_destroy(&mutex))
		perror(NULL);
}

/**
* create_task - create a task
*
* @entry: pointer to the entry function of the task
* @param: parameter to be passed to the entry function
*
* Return: pointer to the created task structure
*/
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *create = malloc(sizeof(task_t));

	if (!create)
		return (NULL);

	create->entry = entry; /* pointer to function serve as the task entry*/
	create->param = param; /* parameter to be passed to the entry function */
	create->status = PENDING; /* task status, default to PENDING */
	create->result = NULL; /* stores the return value of the entry function */
	create->lock = mutex; /* assign the shared mutex directly */

	return (create);
}

/**
* destroy_task - destroy a task
*
* @task: pointer to the task to be destroyed
*/
void destroy_task(task_t *task)
{
	list_destroy(task->result, free);
	free(task->result);
	free(task);
}

/**
* exec_tasks - execute tasks in a thread-safe manner
*
* @tasks: pointer to the list of tasks to be executed
*
* Return: NULL (as its return value will not be retrieved)
*/
void *exec_tasks(list_t const *tasks)
{
	node_t *current = NULL;
	task_t *task = NULL;
	size_t i = 0;

	/* Lock the mutex at the beginning */
	pthread_mutex_lock(&mutex);

	current = tasks->head;
	while (current)
	{
		task = current->content;
		if (task->status == PENDING)
		{
			task->status = STARTED;
			tprintf("[%02zd] Started\n", i);
			task->result = task->entry(task->param);
			if (task->result)
				tprintf("[%02zd] Success\n", i);
			else
				tprintf("[%02zd] Failed\n", i);
			task->status = task->result ? SUCCESS : FAILURE;
		}
		current = current->next;
		i++;
	}
	/* Unlock the mutex at the end */
	pthread_mutex_unlock(&mutex);

	return (NULL);
}
