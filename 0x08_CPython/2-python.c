#include <Python.h>
#include <stdio.h>


void print_python_bytes(PyObject *p)
{
	if (!PyBytes_Check(p))
	{
		fprintf(stderr, "[ERROR] Invalid Bytes Object\n");
		return;
	}

	Py_ssize_t char_size, i;
	unsigned char *str;

	char_size = PyObject_Size(p);

	str = (unsigned char *)PyBytes_AsString(p);

	printf("[.] bytes object info\n");
	printf("  size: %zd\n", char_size);
	printf("  trying string: %s\n", str);

	printf("  first %zd bytes: ", char_size < 10 ? char_size : 10);

	for (i = 0; i < (char_size < 10 ? char_size : 10); i++)
	{
		printf("%02x ", str[i]);
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
		item = PyList_GetItem(p, i);
		/* Print type and index of each element */
		printf("Element %zd: %s\n", i, item->ob_type->tp_name);
		/* If the element is Bytes object, call print_python_bytes function */
		if (PyBytes_Check(item))
		{
			print_python_bytes(item);
		}
	}
}
