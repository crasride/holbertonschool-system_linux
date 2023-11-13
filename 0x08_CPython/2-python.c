#include <Python.h>
#include <stdio.h>


/**
* print_python_bytes - Print information about a Python Bytes object.
*
* This function prints information about a Python Bytes object, including its
* size and the first 10 bytes in hexadecimal format.
*
* @p: A pointer to the Python Bytes object.
*/
void print_python_bytes(PyObject *p)
{
	/* pointer to store byte string */
	char *byte_string = NULL;
	/* Variable to store the size of the byte string */
	Py_ssize_t byte_size;

	printf("[.] bytes object info\n");

	/* Check the object is a valid Bytes object */
	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}

	/* Retrieve byte string and its size */
	PyBytes_AsStringAndSize(p, &byte_string, &byte_size);
	printf("  size: %zd\n", byte_size);
	printf("  trying string: %s\n", byte_string);
	printf("  first %zd bytes:", byte_size <= 10 ? byte_size + 1 : 10);

	/* Iterate through the bytes to print in hexadecimal */
	for (int i = 0; i <= byte_size && i < 10; i++)
	{
		printf(" %02hhx", byte_string[i]);
	}
	printf("\n");
}


/**
* print_python_list - Print information about a Python List object.
*
* This function prints information about a Python List object, including its
* size, allocation, and the types of its elements.
*
* @p: A pointer to the Python List object.
*/
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
