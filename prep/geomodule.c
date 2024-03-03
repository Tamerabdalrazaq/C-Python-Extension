#include <Python.h>
#define PY_SSIZE_T_CLEAN

#include <stdio.h>
#include <math.h>

double geo_c(double z, int n)
{
    double geo_sum = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        /* pow(x,y) function raises x to the power of y - it is from <math.h> */
        geo_sum += pow(z, i);
    }
    return geo_sum;
}

double sum(double z, int n)
{
    double geo_sum = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        /* pow(x,y) function raises x to the power of y - it is from <math.h> */
        geo_sum += z;
    }
    return geo_sum;
}

static PyObject *geo_sum(PyObject *self, PyObject *args)
{
    double z;
    int n;
    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if (!PyArg_ParseTuple(args, "di", &z, &n))
    {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

    /* This builds the answer ("d" = Convert a C double to a Python floating point number) back into a python object */
    return Py_BuildValue("d", geo_c(z, n)); /*  Py_BuildValue(...) returns a PyObject*  */
}

static PyObject *regular_sum(PyObject *self, PyObject *args)
{
    double z;
    int n;
    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if (!PyArg_ParseTuple(args, "di", &z, &n))
    {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

    /* This builds the answer ("d" = Convert a C double to a Python floating point number) back into a python object */
    return Py_BuildValue("d", sum(z, n)); /*  Py_BuildValue(...) returns a PyObject*  */
}

static PyMethodDef geoMethods[] = {
    {"geo_sum",                                                  /* the Python method name that will be used */
     (PyCFunction)geo_sum,                                       /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,                                               /* flags indicating parameters
                                   accepted for this function */
     PyDoc_STR("A geometric series up to n. sum_up_to_n(z^n)")}, /*  The docstring for the function */
    {
        "regular_sum",
        (PyCFunction)regular_sum,
        METH_VARARGS,
        PyDoc_STR("A regular sum function.")},
    {NULL, NULL, 0, NULL}, /* The last entry must be all NULL as shown to act as a
                              sentinel. Python looks for this entry to know that all
                                of the functions for the module have been defined. */
};

static struct PyModuleDef geomodule = {
    PyModuleDef_HEAD_INIT,
    "geo_capi", /* name of module */
    NULL,       /* module documentation, may be NULL */
    -1,         /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    geoMethods  /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_geo_capi(void)
{
    PyObject *m;
    m = PyModule_Create(&geomodule);
    if (!m)
    {
        return NULL;
    }
    return m;
}
