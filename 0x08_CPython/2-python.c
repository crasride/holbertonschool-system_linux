#include <Python.h>
#include <stdio.h>

void print_python_bytes(PyObject *p)
{
	char *byte_string = NULL;
	Py_ssize_t byte_size;

	printf("[.] bytes object info\n");

	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}

	PyBytes_AsStringAndSize(p, &byte_string, &byte_size);
	printf("  size: %zd\n", byte_size);
	printf("  trying string: %s\n", byte_string);
	printf("  first %zd bytes:", byte_size <= 10 ? byte_size + 1 : 10);

	for (int i = 0; i <= byte_size && i < 10; i++)
	{
		printf(" %02hhx", byte_string[i]);
	}
	printf("\n");
}


void print_python_list(PyObject *p)
{
	/* Check if the object is a valid List object */
	if (!PyList_Check(p))
	{
		fprintf(stderr, "[ERROR] Invalid List Object\n");
		return;
	}

	Py_ssize_t size, alloc, i;
	PyObject *item;

	/* Get size and allocation of the List object */
	size = PyList_Size(p);
	alloc = ((PyListObject *)p)->allocated;

	/* Print information about the Python List */
	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %zd\n", size);
	printf("[*] Allocated = %zd\n", alloc);

	/* Iterate through the elements of the List */
	for (i = 0; i < size; i++)
	{
		item = PyList_GET_ITEM(p, i);
		/* Print type and index of each element */
		printf("Element %zd: %s\n", i, item->ob_type->tp_name);
		/* If the element is Bytes object, call print_python_bytes function */
		if (PyBytes_Check(item))
		{
			print_python_bytes(item);
		}
	}
}
