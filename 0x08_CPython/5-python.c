#include <Python.h>

#define BASE_SHIFT 30
#define CALCULATE_BASE() (1UL << BASE_SHIFT)

/**
* print_python_int - Display a PyLong Object's value.
* @p: A pointer to the PyObject.
*/
void print_python_int(PyObject *p)
{
	/* Cast PyObject to PyLongObject */
	PyLongObject *long_obj = (PyLongObject *)p;

	/* Get the size the integer */
	Py_ssize_t size = long_obj->ob_base.ob_size;
	Py_ssize_t i;
	unsigned long result = 0;
	/* uses base 2^30 for digits */
	/* unsigned long base = (unsigned long)1 << 30; */
	unsigned long base = CALCULATE_BASE();

	/* Check the given object is valid Python integer */
	if (!PyLong_Check(p))
	{
		printf("Invalid Int Object\n");
		return;
	}
	/*
	* If the size is negative, it's a negative integer; print the minus sign
	* and make size positive
	*/
	if (size < 0)
	{
		printf("-");
		size = -size;
	}
	/* Check for C unsigned long int overflow */
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



