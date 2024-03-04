#include <Python.h>
#define PY_SSIZE_T_CLEAN
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define EPSILON 0.01
#define ITER 200
#define ERROR_K "Invalid number of clusters!"
#define ERROR_N "Invalid number of points!"
#define ERROR_d "Invalid dimension of point!"
#define ERROR_iter "Invalid maximum iteration!"


struct Cluster
{
    double *centroid;
    int size;
};
typedef struct Cluster Cluster;

/*check if a string represents an integer*/
int isStringDigit(const char *str)
{
    /* Iterate through each character in the string*/
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        /* Check if the character is not a digit*/
        if (!isdigit(str[i]))
        {
            /* If any character is not a digit, return false*/
            return 0;
        }
    }
    /* If all characters are digits, return true*/
    return 1;
}

/*creates an array*/
void *createArray(int n, int size)
{
    void *array = malloc(n * size);
    if (array == NULL)
    {
        printf("An Error Has Occurred");
        exit(1);
    }
    return array;
}

/*creates a submatrix*/
double **sub_matrix_k(double **matrix, int k, int d)
{
    int i;
    int j;
    double **sub_array;
    sub_array = malloc(k * sizeof(double *));
    for (i = 0; i < k; i++)
    {
        sub_array[i] = (double *)createArray(d, sizeof(double));
        for (j = 0; j < d; j++)
        {
            sub_array[i][j] = matrix[i][j];
        }
    }

    return sub_array;
}

/*used to free memory*/
void free_matrix(double **matrix, int k)
{
    int i;
    for (i = 0; i < k; i++)
        free(matrix[i]);
    free(matrix);
}

/*used to free memory*/
void free_clusters(Cluster *clusters, int k)
{
    int i;
    for (i = 0; i < k; ++i)
    {
        free(clusters[i].centroid);
    }
    free(clusters);
}

/*calculates the euc distance between two given vectors*/
double euc_l2(double *v1, double *v2, int d)
{
    int i;
    double dist;
    dist = 0.0;
    for (i = 0; i < d; i++)
    {

        dist += pow(v1[i] - v2[i], 2.0);
    }

    return sqrt(dist);
}

/*finds the closest cluster to a given vector by calculating the euc dist from it*/
int find_closest_centroid_index(double **centroids, double *v, int k, int d)
{
    double min_dist = euc_l2(v, centroids[0], d);
    int index = 0;
    int i;
    for (i = 0; i < k; i++)
    {
        double dist = euc_l2(v, centroids[i], d);
        if (dist < min_dist)
        {
            index = i;
            min_dist = dist;
        }
    }
    return index;
}

/*calculates the average of all vectors in a cluster*/
double *calc_centroid_average(Cluster cluster, int d)
{
    int i, size;
    double *centroid, *averaged_vector;
    centroid = cluster.centroid;
    size = cluster.size;
    averaged_vector = calloc(d, sizeof(double));
    if (size == 0)
    {
        return averaged_vector;
    }
    for (i = 0; i < d; i++)
    {
        averaged_vector[i] = (double)centroid[i] / size;
    }
    return averaged_vector;
}

/*checks if the last change of centroids is less than epsilon for each*/
int check_centroid_convergence(double **centroids, double **new_centroids, int k, int d)
{
    int i, convergent_centroids;
    double *centroid_old, *centroid_new;
    convergent_centroids = 0;
    for (i = 0; i < k; i++)
    {
        centroid_old = centroids[i];
        centroid_new = new_centroids[i];
        if (euc_l2(centroid_old, centroid_new, d) <= EPSILON)
            convergent_centroids += 1;
    }
    return (convergent_centroids == k);
}

/*adds vector to a cluster*/
void add_vector_to_centroid(Cluster *clus, double const vec[], int d)
{
    int i;
    double updated_entry_i;
    for (i = 0; i < d; i++)
    {
        updated_entry_i = clus->centroid[i] + vec[i];
        clus->centroid[i] = updated_entry_i;
    }
    clus->size = clus->size + 1;
}

double **k_means(int k, int n, int d, int iter, double **data)
{
    /*creates the initial centroids according to the first k vectors given in the input*/
    int i, j, data_i, convergence, closest_centroid_index;
    double **centroids, **updated_centroids;
    double *x;

    centroids = sub_matrix_k(data, k, d);
    for (i = 0; i < iter; i++)
    {
        struct Cluster *new_centroids = (struct Cluster *)createArray(k, sizeof(Cluster));
        for (j = 0; j < k; j++)
        {
            new_centroids[j].size = 0;
            new_centroids[j].centroid = calloc(d, sizeof(double));
        }
        for (data_i = 0; data_i < n; data_i++)
        {
            x = data[data_i];
            closest_centroid_index = find_closest_centroid_index(centroids, x, k, d);
            add_vector_to_centroid(&new_centroids[closest_centroid_index], x, d);
        }
        updated_centroids = (double **)createArray(k, sizeof(double *));
        for (j = 0; j < k; j++)
        {
            updated_centroids[j] = calc_centroid_average(new_centroids[j], d);
        }
        convergence = check_centroid_convergence(updated_centroids, centroids, k, d);
        free_matrix(centroids, k);
        free_clusters(new_centroids, k);
        centroids = updated_centroids;
        if (convergence)
            break;
    }
    return centroids;
}

// 8**********************************8



static PyObject *get_k_means(PyObject *self, PyObject *args)
{
    double z;
    int n_1;
    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if (!PyArg_ParseTuple(args, "di", &z, &n_1))
    {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

    int k = 4;
    int n = 6;
    int d = 2;
    int iter = 3;
    int rows = 6;
    int cols = 2;
    double **array = (double **)createArray(rows, sizeof(double *));
    int i;
    for (i = 0; i < rows; i++)
    {
        array[i] = (double *)createArray(cols, sizeof(double));
    }
    //[(1, 2), (5, 8), (1, 5), (6, 9), (2, 2), (8, 1)]
    array[0][0] = 1;
    array[0][1] = 2;
    array[1][0] = 5;
    array[1][1] = 8;
    array[2][0] = 1;
    array[2][1] = 5;
    array[3][0] = 6;
    array[3][1] = 9;
    array[4][0] = 2;
    array[4][1] = 2;
    array[5][0] = 8;
    array[5][1] = 1;
    k_means(k, n, d, iter, array);

    /* This builds the answer ("d" = Convert a C double to a Python floating point number) back into a python object */
    return Py_BuildValue("d", 5); /*  Py_BuildValue(...) returns a PyObject*  */
}

static PyMethodDef geoMethods[] = {
    {"fit",                                                            /* the Python method name that will be used */
     (PyCFunction)get_k_means,                                         /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,                                                     /* flags indicating parameters
                                         accepted for this function */
     PyDoc_STR("A C function that implements the k-means algorithm")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL},                                             /* The last entry must be all NULL as shown to act as a
                                                                          sentinel. Python looks for this entry to know that all
                                                                            of the functions for the module have been defined. */
};

static struct PyModuleDef geomodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", /* name of module */
    NULL,         /* module documentation, may be NULL */
    -1,           /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    geoMethods    /* the PyMethodDef array from before containing the methods of the extension */
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

