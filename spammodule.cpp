#include "Python.h"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

vector<int> getprimesimpl(int n) {
  if (n <= 1) return {};
  if (n == 2) return {2};
  vector<int> primes = {2}; bool mark;
  for (int i = 3; i < n; i+=2) {
    mark = true;
    for (int j = 3; j <= ceil(pow(i, 0.5)); j+=2) {
      if (i % j == 0) {
        mark = false;
        break;
      }
    }
    if (mark) primes.push_back(i);
  }
  return primes;
}

static PyObject* getprimes(PyObject* self, PyObject* args) {
  int n;
  if (!PyArg_ParseTuple(args, "i", &n))
    return NULL;
  vector<int> vec = getprimesimpl(n);
  Py_ssize_t size = vec.size();
  PyObject* result = PyTuple_New(size);
  for (Py_ssize_t i = 0; i < size; i++) {
    PyTuple_SET_ITEM(result, i, PyLong_FromLong(vec.at(i)));
  }
  return result;
}

static PyMethodDef spammethods[] = {
  {"getprimes", getprimes, METH_VARARGS, "get primes til"},
  {NULL, NULL, 0, NULL} /* Sentinel */
};

static PyModuleDef spammodule = {
    PyModuleDef_HEAD_INIT,
    "spam",   /* name of module */
    "spam module",
    -1,
    spammethods
};

PyMODINIT_FUNC
PyInit_spam(void) {
    PyObject* m = PyModule_Create(&spammodule);
    return m;
}
