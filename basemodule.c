#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

char *
to_base(int num, int base, int max_digits, int to_truncate)
{
    uint32_t digits = max_digits;
    char *dec_num = malloc((sizeof(char) * digits) + 1);
    
    num /= ((base * to_truncate) + 1);

    for(uint32_t i=0;i<digits;++i)
    {
        dec_num[i] = '0';
    }
    dec_num[digits] = '\0';

    char *cur = dec_num + (digits-1);
    while(num > 0)
    {
        uint32_t offset = num % base;
        if(offset >= 10)
        {
            offset += 7;
        }
        *cur = offset + '0';
        num /= base;
        --cur;
    }
    return dec_num;
}

static PyObject *
willmath_to_base(PyObject *self, PyObject *args)
{
    int num, base, digits, cutoff;

    if (!PyArg_ParseTuple(args, "iii", &num, &base, &digits, &cutoff))
        return NULL;

    return PyString_FromString(to_base(num, base, digits, cutoff));
}

PyMODINIT_FUNC
PyInit_base(void)
{
    PyObject *m;

    m = PyModule_Create(&spammodule);
    if (m == NULL)
        return NULL;

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_XINCREF(SpamError);
    if (PyModule_AddObject(m, "error", SpamError) < 0) {
        Py_XDECREF(SpamError);
        Py_CLEAR(SpamError);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

int
main(int argc, char **argv)
{
    if(argc != 5)
    {
        printf("usage: convert <number> <base> <number of digits to print> <number of digits to cutt off of the end>\n");
        return -1;
    }
    
    uint32_t num = atoi(argv[1]);
    uint32_t base = atoi(argv[2]);
    uint32_t num_digits = atoi(argv[3]);
    uint32_t num_cutoff = atoi(argv[4]);
    char * converted_string = to_base(num, base, num_digits, num_cutoff);
    printf("%d\n%s\n", num, converted_string);
    //free(converted_string);

    return 0;
}