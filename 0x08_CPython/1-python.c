#include <Python.h>
#include <stdio.h>

void print_python_list(PyObject *p)
{
    PyListObject *list = (PyListObject *)p;
    Py_ssize_t size, alloc, i;
    PyObject *item;

    size = PyList_Size(p);
    alloc = list->allocated;

    printf("[*] Información de la lista de Python\n");
    printf("[*] Tamaño de la lista de Python = %zd\n", size);
    printf("[*] Asignado = %zd\n", alloc);

    for (i = 0; i < size; i++)
    {
        item = PyList_GetItem(p, i);
        printf("Elemento %zd: %s\n", i, Py_TYPE(item)->tp_name);
    }
}



