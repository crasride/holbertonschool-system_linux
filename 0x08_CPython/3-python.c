#include <Python.h>
#include <stdio.h>


void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);

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
		printf("  [ERROR] Invalid List Object\n");
		return;
	}

	Py_ssize_t size, alloc, i;
	PyObject *item;

	/* Get size and allocation of the List object */
	size = PyObject_Length(p);
	if (size == -1)
	{
		fprintf(stderr, "  [ERROR] Failed to get list size\n");
		return;
	}

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
		else if (PyFloat_Check(item))
		{
			print_python_float(item);
		}
	}
}

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
	Py_ssize_t size, i;
	char *byteString;

	/* Disable output buffering to ensure immediate printing */
	setbuf(stdout, NULL);
	printf("[.] bytes object info\n");

	/* Check the object is a valid Bytes object */
	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}

	/* Get size of the Bytes object and the pointer to the byte content */
	size = PyBytes_Size(p);
	byteString = ((PyBytesObject *)p)->ob_sval;

	/* Print size of the Bytes object and the byte string*/
	printf("  size: %li\n", size);
	printf("  trying string: %s\n", byteString);
	/* Print the first 10 bytes in hexadecimal format */
	printf("  first %li bytes:", size < 10 ? size + 1 : 10);

	for (i = 0; i <= size && i < 10; ++i)
	{
		printf(" %02hhx", byteString[i]);
	}

	putchar('\n');
}


/**
* print_python_float - Print information about a Python Float object.
*
* This function prints information about a Python Float object, including its
* value formatted as a string.
*
* @p: A pointer to the Python Float object.
*/
void print_python_float(PyObject *p)
{
	/* Cast the PyObject to a PyFloatObject */
	PyFloatObject *floatObj = (PyFloatObject *)p;

	/* Get the floating-point value from the PyFloatObject */
	double val = floatObj->ob_fval;

	/* Buffer to hold the formatted string representation of the float */
	char str[40];

	printf("[.] float object info\n");
	if (!PyFloat_Check(floatObj))
	{
		printf("  [ERROR] Invalid Float Object\n");
		fflush(stdout);
		return;
	}
	/* Format float value as a string with up to 16 significant digits */
	sprintf(str, "%.16g", val);
	/* Check the formatted string contains a decimal point */
	if (strchr(str, '.') != NULL)
	{
		/* Print the value with the full precision */
		printf("  value: %s\n", str);
	}
	else
	{
		/* print the value with one decimal place */
		printf("  value: %.1f\n", val);
	}
}
