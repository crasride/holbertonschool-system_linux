#include <Python.h>


void print_python_int(PyObject *p)
{
	PyLongObject *long_obj = (PyLongObject *)p;
	Py_ssize_t size = long_obj->ob_base.ob_size;
	Py_ssize_t i;
	unsigned long result = 0;
	/* uses base 2^30 for digits */
	unsigned long base = (unsigned long)1 << 30;

	if (!PyLong_Check(p))
	{
		printf("Invalid Int Object\n");
		return;
	}

	if (size < 0)
	{
		printf("-");
		size = -size;
	}

	if (size > 3 || (size == 3 && long_obj->ob_digit[2] > 15))
	{
		printf("C unsigned long int overflow\n");
		return;
	}

	for (i = size - 1; i >= 0; i--)
	{
		result = result * base + long_obj->ob_digit[i];
	}

	printf("%lu\n", result);
}



