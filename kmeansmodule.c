#include <Python.h>
#define PY_SSIZE_T_CLEAN

#include <stdio.h>
#include <math.h>

double k_means(double z, int n)
{
    double geo_sum = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        /* pow(x,y) function raises x to the power of y - it is from <math.h> */
        geo_sum += z;
    }
    return 10;
}

static PyObject *fit(PyObject *self, PyObject *args)
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
    return Py_BuildValue("d", k_means(z, n)); /*  Py_BuildValue(...) returns a PyObject*  */
}

static PyMethodDef geoMethods[] = {
    {"fit",            /* the Python method name that will be used */
     (PyCFunction)fit, /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,     /* flags indicating parameters
accepted for this function */
     PyDoc_STR("Calculates k_means")},
    /*  The docstring for the function */
    {NULL, NULL, 0, NULL}, /* The last entry must be all NULL as shown to act as a
                              sentinel. Python looks for this entry to know that all
                                of the functions for the module have been defined. */
};

static struct PyModuleDef geomodule = {
    PyModuleDef_HEAD_INIT,
    "kmeansmodule", /* name of module */
    NULL,           /* module documentation, may be NULL */
    -1,             /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    geoMethods      /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_kmeansmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&geomodule);
    if (!m)
    {
        return NULL;
    }
    return m;
}
