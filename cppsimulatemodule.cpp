#include <vector>
#include <Python.h>
#include "include/HandParser.h"
#include "include/simulation.h"

PyObject* PyWrapper_simulation(PyObject* self, PyObject* args) {
    vector<vector<int>> cppcards{{0, 0}, {0, 0}};
    int lim; PyObject *pycards;

    if (!PyArg_ParseTuple(args, "O!i", &PyList_Type, &pycards, &lim)) {
        PyErr_SetString(PyExc_TypeError, "argument should be list!");
        return NULL;
    }

    for (int i = 0; i < 2; i++) {
        PyObject *pycard = PyList_GetItem(pycards, i);

        for (int j = 0; j < 2; j++) {
            PyObject *pyval = PyList_GetItem(pycard, j);

            if (!PyLong_Check(pyval)) {
                PyErr_SetString(PyExc_TypeError, "type should be int!!");
                return NULL;
            }

            cppcards[i][j] = (int) PyLong_AsLong(pyval);
        }
    }

    Py_DECREF(pycards);
    double cpp_sim_res = py_ext::simulate(cppcards, lim);
    return Py_BuildValue("f", cpp_sim_res);
}

static PyMethodDef simulation_methods[] = {
        {
            "estimate",
            PyWrapper_simulation,
            METH_VARARGS,
            "estimate(cards, number)"
         },
        {NULL, NULL, 0, NULL}
};
static PyModuleDef simulation_module = {
        PyModuleDef_HEAD_INIT,
        "cppsimulate",
        "cpp simulation extension",
        -1, // module members
        simulation_methods
};

PyMODINIT_FUNC PyInit_cppsimulate( void ) {
    PyObject *module = PyModule_Create(&simulation_module);
    return module;
}