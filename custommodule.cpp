#include "Python.h"

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
} CustomObject;

static PyTypeObject CustomType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "custom.Custom", /* .tp_name */
    sizeof(CustomObject) /* .tp_basicsize */
};

static PyModuleDef custommodule = {
    PyModuleDef_HEAD_INIT,
    "custom",
    "Example module that creates an extension type.",
    -1
};

PyMODINIT_FUNC
PyInit_custom(void)
{
    PyObject *m;
    if (PyType_Ready(&CustomType) < 0)
        return NULL;

    m = PyModule_Create(&custommodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&CustomType);
    PyModule_AddObject(m, "Custom", (PyObject *) &CustomType);
    return m;
}
