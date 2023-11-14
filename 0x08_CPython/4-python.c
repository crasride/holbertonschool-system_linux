#include <Python.h>
#include <stdio.h>


void print_python_string(PyObject *p)
{
	Py_ssize_t size;
	wchar_t *wide_str;

	/* Disable output buffering to ensure immediate printing */
	setbuf(stdout, NULL);

	/* Check if the object is a valid string */
	if (!PyUnicode_Check(p))
	{
		puts("[ERROR] Invalid String Object");
		return;
	}

	/* Get size of the string and the wide string pointer */
	size = PyUnicode_GET_SIZE(p);
	wide_str = PyUnicode_AsWideCharString(p, NULL);

	/* Print type, size, value, and address of the string */
	printf("[.] string object info\n");
	printf("  type: %s\n", PyUnicode_IS_COMPACT_ASCII(p) ?
			"compact ascii" : "compact unicode object");
	printf("  length: %li\n", size);
	printf("  value: %ls\n", wide_str);

	/* Release the wide string memory */
	PyMem_Free(wide_str);
}
