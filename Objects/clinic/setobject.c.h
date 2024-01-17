/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(set_pop__doc__,
"pop($self, /)\n"
"--\n"
"\n"
"Remove and return an arbitrary set element.\n"
"\n"
"Raises KeyError if the set is empty.");

#define SET_POP_METHODDEF    \
    {"pop", (PyCFunction)set_pop, METH_NOARGS, set_pop__doc__},

static PyObject *
set_pop_impl(PySetObject *self);

static PyObject *
set_pop(PySetObject *self, PyObject *Py_UNUSED(ignored))
{
    return set_pop_impl(self);
}

PyDoc_STRVAR(set_copy__doc__,
"copy($self, /)\n"
"--\n"
"\n"
"Return a shallow copy of a set.");

#define SET_COPY_METHODDEF    \
    {"copy", (PyCFunction)set_copy, METH_NOARGS, set_copy__doc__},

static PyObject *
set_copy_impl(PySetObject *self);

static PyObject *
set_copy(PySetObject *self, PyObject *Py_UNUSED(ignored))
{
    return set_copy_impl(self);
}

PyDoc_STRVAR(frozenset_copy__doc__,
"copy($self, /)\n"
"--\n"
"\n"
"Return a shallow copy of a set.");

#define FROZENSET_COPY_METHODDEF    \
    {"copy", (PyCFunction)frozenset_copy, METH_NOARGS, frozenset_copy__doc__},

static PyObject *
frozenset_copy_impl(PySetObject *self);

static PyObject *
frozenset_copy(PySetObject *self, PyObject *Py_UNUSED(ignored))
{
    return frozenset_copy_impl(self);
}

PyDoc_STRVAR(set_clear__doc__,
"clear($self, /)\n"
"--\n"
"\n"
"Remove all elements from this set.");

#define SET_CLEAR_METHODDEF    \
    {"clear", (PyCFunction)set_clear, METH_NOARGS, set_clear__doc__},

static PyObject *
set_clear_impl(PySetObject *self);

static PyObject *
set_clear(PySetObject *self, PyObject *Py_UNUSED(ignored))
{
    return set_clear_impl(self);
}

PyDoc_STRVAR(set_isdisjoint__doc__,
"isdisjoint($self, other, /)\n"
"--\n"
"\n"
"Return True if two sets have a null intersection.");

#define SET_ISDISJOINT_METHODDEF    \
    {"isdisjoint", (PyCFunction)set_isdisjoint, METH_O, set_isdisjoint__doc__},

PyDoc_STRVAR(set_symmetric_difference_update__doc__,
"symmetric_difference_update($self, other, /)\n"
"--\n"
"\n"
"Update the set, keeping only elements found in either set, but not in both.");

#define SET_SYMMETRIC_DIFFERENCE_UPDATE_METHODDEF    \
    {"symmetric_difference_update", (PyCFunction)set_symmetric_difference_update, METH_O, set_symmetric_difference_update__doc__},

PyDoc_STRVAR(set_symmetric_difference__doc__,
"symmetric_difference($self, other, /)\n"
"--\n"
"\n"
"Return a new set with elements in either the set or other but not both.");

#define SET_SYMMETRIC_DIFFERENCE_METHODDEF    \
    {"symmetric_difference", (PyCFunction)set_symmetric_difference, METH_O, set_symmetric_difference__doc__},

PyDoc_STRVAR(set_issubset__doc__,
"issubset($self, other, /)\n"
"--\n"
"\n"
"Report whether another set contains this set.");

#define SET_ISSUBSET_METHODDEF    \
    {"issubset", (PyCFunction)set_issubset, METH_O, set_issubset__doc__},

PyDoc_STRVAR(set_issuperset__doc__,
"issuperset($self, other, /)\n"
"--\n"
"\n"
"Report whether this set contains another set.");

#define SET_ISSUPERSET_METHODDEF    \
    {"issuperset", (PyCFunction)set_issuperset, METH_O, set_issuperset__doc__},

PyDoc_STRVAR(set_add__doc__,
"add($self, object, /)\n"
"--\n"
"\n"
"Add an element to a set.\n"
"\n"
"This has no effect if the element is already present.");

#define SET_ADD_METHODDEF    \
    {"add", (PyCFunction)set_add, METH_O, set_add__doc__},

PyDoc_STRVAR(set___contains____doc__,
"__contains__($self, object, /)\n"
"--\n"
"\n"
"x.__contains__(y) <==> y in x.");

#define SET___CONTAINS___METHODDEF    \
    {"__contains__", (PyCFunction)set___contains__, METH_O, set___contains____doc__},

PyDoc_STRVAR(set_remove__doc__,
"remove($self, key, /)\n"
"--\n"
"\n"
"Remove an element from a set; it must be a member.\n"
"\n"
"If the element is not a member, raise a KeyError.");

#define SET_REMOVE_METHODDEF    \
    {"remove", (PyCFunction)set_remove, METH_O, set_remove__doc__},

PyDoc_STRVAR(set_discard__doc__,
"discard($self, key, /)\n"
"--\n"
"\n"
"Remove an element from a set if it is a member.\n"
"\n"
"Unlike set.remove(), the discard() method does not raise\n"
"an exception when an element is missing from the set.");

#define SET_DISCARD_METHODDEF    \
    {"discard", (PyCFunction)set_discard, METH_O, set_discard__doc__},

PyDoc_STRVAR(set___reduce____doc__,
"__reduce__($self, /)\n"
"--\n"
"\n");

#define SET___REDUCE___METHODDEF    \
    {"__reduce__", (PyCFunction)set___reduce__, METH_NOARGS, set___reduce____doc__},

static PyObject *
set___reduce___impl(PySetObject *self);

static PyObject *
set___reduce__(PySetObject *self, PyObject *Py_UNUSED(ignored))
{
    return set___reduce___impl(self);
}

PyDoc_STRVAR(set___sizeof____doc__,
"__sizeof__($self, /)\n"
"--\n"
"\n"
"S.__sizeof__() -> size of S in memory, in bytes.");

#define SET___SIZEOF___METHODDEF    \
    {"__sizeof__", (PyCFunction)set___sizeof__, METH_NOARGS, set___sizeof____doc__},

static PyObject *
set___sizeof___impl(PySetObject *self);

static PyObject *
set___sizeof__(PySetObject *self, PyObject *Py_UNUSED(ignored))
{
    return set___sizeof___impl(self);
}
/*[clinic end generated code: output=37e660dbf15cd755 input=a9049054013a1b77]*/
