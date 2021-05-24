#ifndef Py_INTERNAL_CODE_H
#define Py_INTERNAL_CODE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


typedef struct {
    PyObject *ptr;  /* Cached pointer (borrowed reference) */
    uint64_t globals_ver;  /* ma_version of global dict */
    uint64_t builtins_ver; /* ma_version of builtin dict */
} _PyOpcache_LoadGlobal;

typedef struct {
    PyTypeObject *type;
    Py_ssize_t hint;
    unsigned int tp_version_tag;
} _PyOpCodeOpt_LoadAttr;

struct _PyOpcache {
    union {
        _PyOpcache_LoadGlobal lg;
        _PyOpCodeOpt_LoadAttr la;
    } u;
    char optimized;
};


// We would use an enum if C let us specify the storage type.
typedef unsigned char _PyFastLocalKind;
// Note that these all fit within _PyFastLocalKind, as do combinations.
#define CO_FAST_POSONLY     0x01
#define CO_FAST_POSORKW     0x02
#define CO_FAST_VARARGS     0x04
#define CO_FAST_KWONLY      0x08
#define CO_FAST_VARKWARGS   0x10
#define CO_FAST_LOCALONLY   0x20
#define CO_FAST_CELL        0x40
#define CO_FAST_FREE        0x80

#define CO_FAST_ARG (CO_FAST_POSONLY | CO_FAST_POSORKW | CO_FAST_VARARGS | \
                     CO_FAST_KWONLY | CO_FAST_VARKWARGS)
#define CO_FAST_LOCAL (CO_FAST_ARG | CO_FAST_LOCALONLY)
#define CO_FAST_ANY (CO_FAST_LOCAL | CO_FAST_CELL | CO_FAST_FREE)


struct _PyCodeConstructor {
    /* metadata */
    PyObject *filename;
    PyObject *name;
    int flags;

    /* the code */
    PyObject *code;
    int firstlineno;
    PyObject *linetable;

    /* used by the code */
    PyObject *consts;
    PyObject *names;

    /* mapping frame offsets to information */
    PyObject *varnames;
    PyObject *cellvars;
    PyObject *freevars;

    /* args (within varnames) */
    int argcount;
    int posonlyargcount;
    int kwonlyargcount;

    /* needed to create the frame */
    int stacksize;

    /* used by the eval loop */
    PyObject *exceptiontable;
};

// Using an "arguments struct" like this is helpful for maintainability
// in a case such as this with many parameters.  It does bear a risk:
// if the struct changes and callers are not updated properly then the
// compiler will not catch problems (like a missing argument).  This can
// cause hard-to-debug problems.  The risk is mitigated by the use of
// check_code() in codeobject.c.  However, we may decide to switch
// back to a regular function signature.  Regardless, this approach
// wouldn't be appropriate if this weren't a strictly internal API.
// (See the comments in https://github.com/python/cpython/pull/26258.)
PyAPI_FUNC(PyCodeObject *) _PyCode_New(struct _PyCodeConstructor *);


/* Private API */

int _PyCode_InitOpcache(PyCodeObject *co);

PyAPI_FUNC(bool) _PyCode_HasFastlocals(PyCodeObject *, _PyFastLocalKind);
PyAPI_FUNC(void) _PyCode_FastInfoFromOffset(PyCodeObject *, int,
                                            _PyFastLocalKind,
                                            PyObject **, _PyFastLocalKind *);
PyAPI_FUNC(int) _PyCode_OffsetFromIndex(PyCodeObject *, int, _PyFastLocalKind);
PyAPI_FUNC(int) _PyCode_CellForLocal(PyCodeObject *, int);

/* This does not fail.  A negative result means "no match". */
PyAPI_FUNC(int) _PyCode_FastOffsetFromId(PyCodeObject *,
                                                _Py_Identifier *,
                                                _PyFastLocalKind);

static inline PyObject *
_PyCode_GetLocalvar(PyCodeObject *co, int index)
{
    assert(index >= 0 && index < co->co_nlocals);
    return PyTuple_GetItem(co->co_varnames, index);
}

static inline PyObject *
_PyCode_GetCellvar(PyCodeObject *co, int index)
{
    assert(index >= 0 && index < co->co_ncellvars);
    return PyTuple_GetItem(co->co_cellvars, index);
}

static inline PyObject *
_PyCode_GetFreevar(PyCodeObject *co, int index)
{
    assert(index >= 0 && index < co->co_nfreevars);
    return PyTuple_GetItem(co->co_freevars, index);
}

static inline bool
_PyCode_CodeIsValid(PyCodeObject *co)
{
    return PyBytes_Check(co->co_code) &&
           PyBytes_GET_SIZE(co->co_code) <= INT_MAX &&
           PyBytes_GET_SIZE(co->co_code) % sizeof(_Py_CODEUNIT) == 0 &&
           _Py_IS_ALIGNED(PyBytes_AS_STRING(co->co_code),
                          sizeof(_Py_CODEUNIT));
}

static inline _Py_CODEUNIT *
_PyCode_GetInstructions(PyCodeObject *co)
{
    return (_Py_CODEUNIT *)PyBytes_AS_STRING(co->co_code);
}

static inline Py_ssize_t
_PyCode_NumInstructions(PyCodeObject *co)
{
    return PyBytes_Size(co->co_code) / sizeof(_Py_CODEUNIT);
}

/* Speed hacks for very specific performance-sensitive locations. */

// For _PyEval_MakeFrameVector() in ceval.c.
static inline PyObject **
_PyCode_LocalvarsArray(PyCodeObject *co)
{
    return ((PyTupleObject *)(co->co_varnames))->ob_item;
}

// For LOAD_CLASSDEREF in ceval.c.
// Normally we'd just use _PyCode_FastInfoFromOffset().
static inline PyObject *
_PyCode_GetFastFreevar(PyCodeObject *co, int offset)
{
    return _PyCode_GetFreevar(co, offset - co->co_ncellvars);
}


#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_CODE_H */
