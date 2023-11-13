#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

void print_python_list(PyObject *p)
{

    PyListObject *list = (PyListObject *)p;
    Py_ssize_t size, alloc, i;
    PyObject *item;


    size = PyList_Size(p);
    alloc = list->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", alloc);


    for (i = 0; i < size; i++)
    {
        item = PyList_GetItem(p, i);
        printf("Element %zd: %s\n", i, Py_TYPE(item)->tp_name);
    }
}


