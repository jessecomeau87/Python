
/* Thread and interpreter state structures and their interfaces */


#ifndef Py_PYSTATE_H
#define Py_PYSTATE_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef WITH_THREAD
#ifdef Py_BUILD_CORE
#include "pythread.h"
#endif
#endif

/* This limitation is for performance and simplicity. If needed it can be
removed (with effort). */
#define MAX_CO_EXTRA_USERS 255

/* State shared between threads */

struct _ts; /* Forward */
struct _is; /* Forward */
struct _frame; /* Forward declaration for PyFrameObject. */

#ifdef Py_LIMITED_API
typedef struct _is PyInterpreterState;
#else
typedef PyObject* (*_PyFrameEvalFunction)(struct _frame *, int);


typedef struct {
    int ignore_environment;
    int use_hash_seed;
    unsigned long hash_seed;
    int _disable_importlib; /* Needed by freeze_importlib */
} _PyCoreConfig;

#define _PyCoreConfig_INIT {0, -1, 0, 0}

/* Placeholders while working on the new configuration API
 *
 * See PEP 432 for final anticipated contents
 *
 * For the moment, just handle the args to _Py_InitializeEx
 */
typedef struct {
    int install_signal_handlers;
} _PyMainInterpreterConfig;

#define _PyMainInterpreterConfig_INIT {-1}

typedef struct _is {

    struct _is *next;
    struct _ts *tstate_head;

    int64_t id;

    PyObject *modules_by_index;
    PyObject *sysdict;
    PyObject *builtins;
    PyObject *importlib;

    /* Used in Python/sysmodule.c. */
    int check_interval;
    PyObject *warnoptions;
    PyObject *xoptions;

    /* Used in Modules/_threadmodule.c. */
    long num_threads;
    /* Support for runtime thread stack size tuning.
       A value of 0 means using the platform's default stack size
       or the size specified by the THREAD_STACK_SIZE macro. */
    /* Used in Python/thread.c. */
    size_t pythread_stacksize;

    PyObject *codec_search_path;
    PyObject *codec_search_cache;
    PyObject *codec_error_registry;
    int codecs_initialized;
    int fscodec_initialized;

    _PyCoreConfig core_config;
    _PyMainInterpreterConfig config;
#ifdef HAVE_DLOPEN
    int dlopenflags;
#endif

    PyObject *builtins_copy;
    PyObject *import_func;
    /* Initialized to PyEval_EvalFrameDefault(). */
    _PyFrameEvalFunction eval_frame;

    Py_ssize_t co_extra_user_count;
    freefunc co_extra_freefuncs[MAX_CO_EXTRA_USERS];

#ifdef HAVE_FORK
    PyObject *before_forkers;
    PyObject *after_forkers_parent;
    PyObject *after_forkers_child;
#endif
} PyInterpreterState;
#endif


/* State unique per thread */

#ifndef Py_LIMITED_API
/* Py_tracefunc return -1 when raising an exception, or 0 for success. */
typedef int (*Py_tracefunc)(PyObject *, struct _frame *, int, PyObject *);

/* The following values are used for 'what' for tracefunc functions: */
#define PyTrace_CALL 0
#define PyTrace_EXCEPTION 1
#define PyTrace_LINE 2
#define PyTrace_RETURN 3
#define PyTrace_C_CALL 4
#define PyTrace_C_EXCEPTION 5
#define PyTrace_C_RETURN 6
#endif

#ifdef Py_LIMITED_API
typedef struct _ts PyThreadState;
#else
typedef struct _ts {
    /* See Python/ceval.c for comments explaining most fields */

    struct _ts *prev;
    struct _ts *next;
    PyInterpreterState *interp;

    struct _frame *frame;
    int recursion_depth;
    char overflowed; /* The stack has overflowed. Allow 50 more calls
                        to handle the runtime error. */
    char recursion_critical; /* The current calls must not cause
                                a stack overflow. */
    /* 'tracing' keeps track of the execution depth when tracing/profiling.
       This is to prevent the actual trace/profile code from being recorded in
       the trace/profile. */
    int tracing;
    int use_tracing;

    Py_tracefunc c_profilefunc;
    Py_tracefunc c_tracefunc;
    PyObject *c_profileobj;
    PyObject *c_traceobj;

    PyObject *curexc_type;
    PyObject *curexc_value;
    PyObject *curexc_traceback;

    PyObject *exc_type;
    PyObject *exc_value;
    PyObject *exc_traceback;

    PyObject *dict;  /* Stores per-thread state */

    int gilstate_counter;

    PyObject *async_exc; /* Asynchronous exception to raise */
    unsigned long thread_id; /* Thread id where this tstate was created */

    int trash_delete_nesting;
    PyObject *trash_delete_later;

    /* Called when a thread state is deleted normally, but not when it
     * is destroyed after fork().
     * Pain:  to prevent rare but fatal shutdown errors (issue 18808),
     * Thread.join() must wait for the join'ed thread's tstate to be unlinked
     * from the tstate chain.  That happens at the end of a thread's life,
     * in pystate.c.
     * The obvious way doesn't quite work:  create a lock which the tstate
     * unlinking code releases, and have Thread.join() wait to acquire that
     * lock.  The problem is that we _are_ at the end of the thread's life:
     * if the thread holds the last reference to the lock, decref'ing the
     * lock will delete the lock, and that may trigger arbitrary Python code
     * if there's a weakref, with a callback, to the lock.  But by this time
     * _PyThreadState_Current is already NULL, so only the simplest of C code
     * can be allowed to run (in particular it must not be possible to
     * release the GIL).
     * So instead of holding the lock directly, the tstate holds a weakref to
     * the lock:  that's the value of on_delete_data below.  Decref'ing a
     * weakref is harmless.
     * on_delete points to _threadmodule.c's static release_sentinel() function.
     * After the tstate is unlinked, release_sentinel is called with the
     * weakref-to-lock (on_delete_data) argument, and release_sentinel releases
     * the indirectly held lock.
     */
    void (*on_delete)(void *);
    void *on_delete_data;

    PyObject *coroutine_wrapper;
    int in_coroutine_wrapper;

    PyObject *async_gen_firstiter;
    PyObject *async_gen_finalizer;

    /* XXX signal handlers should also be here */

} PyThreadState;
#endif

#ifndef Py_LIMITED_API
typedef struct _frame *(*PyThreadFrameGetter)(PyThreadState *self_);

#ifdef Py_BUILD_CORE

#include "Python/condvar.h"
#ifndef Py_HAVE_CONDVAR
#error You need either a POSIX-compatible or a Windows system!
#endif

/* Enable if you want to force the switching of threads at least every `interval` */
#undef FORCE_SWITCHING
#define FORCE_SWITCHING
#endif /* Py_BUILD_CORE */

typedef struct pyruntimestate {

    // Python/pylifecycle.c
    int initialized;
    PyThreadState *finalizing;
//    wchar_t env_home[MAXPATHLEN+1];
#define NEXITFUNCS 32
    void (*exitfuncs[NEXITFUNCS])(void);
    int nexitfuncs;
    void (*pyexitfunc)(void);

    struct _mem_globals {
        // Objects/object.c
        /* List of objects that still need to be cleaned up, singly linked
         * via their gc headers' gc_prev pointers.
         */
        PyObject *trash_delete_later;
        /* Current call-stack depth of tp_dealloc calls. */
        int trash_delete_nesting;

        // Objects/obmalloc.c
        PyMemAllocatorEx allocator;
        PyMemAllocatorEx allocator_raw;
        PyMemAllocatorEx allocator_object;
        PyObjectArenaAllocator allocator_arenas;
        /* Array of objects used to track chunks of memory (arenas). */
        struct arena_object* arenas;
        /* The head of the singly-linked, NULL-terminated list of available
         * arena_objects.
         */
        struct arena_object* unused_arena_objects;
        /* The head of the doubly-linked, NULL-terminated at each end, list of
         * arena_objects associated with arenas that have pools available.
         */
        struct arena_object* usable_arenas;
        /* Number of slots currently allocated in the `arenas` vector. */
        uint maxarenas;
        /* Number of arenas allocated that haven't been free()'d. */
        size_t narenas_currently_allocated;
        /* High water mark (max value ever seen) for
         * narenas_currently_allocated. */
        size_t narenas_highwater;
        /* Total number of times malloc() called to allocate an arena. */
        size_t ntimes_arena_allocated;
//        poolp usedpools[MAX_POOLS];
        Py_ssize_t num_allocated_blocks;
        size_t serialno;     /* incremented on each debug {m,re}alloc */
    } mem;

    struct _warnings_globals {
        // Python/_warnings.c
        /* Both 'filters' and 'onceregistry' can be set in warnings.py;
           get_warnings_attr() will reset these variables accordingly. */
        PyObject *filters;  /* List */
        PyObject *once_registry;  /* Dict */
        PyObject *default_action; /* String */
        long filters_version;
    } warnings;

    struct _ceval_globals {
        int recursion_limit;
        int check_recursion_limit;
#ifdef Py_BUILD_CORE
        unsigned long main_thread;
#ifdef WITH_THREAD
        PyThread_type_lock pending_lock;
        /* This single variable consolidates all requests to break out of the fast path
           in the eval loop. */
        _Py_atomic_int eval_breaker;
        /* Request for dropping the GIL */
        _Py_atomic_int gil_drop_request;
        /* Request for running pending calls. */
        _Py_atomic_int pendingcalls_to_do;
        /* Request for looking at the `async_exc` field of the current thread state.
           Guarded by the GIL. */
        int pending_async_exc;
#define NPENDINGCALLS 32
        struct {
            int (*func)(void *);
            void *arg;
        } pendingcalls[NPENDINGCALLS];
        int pendingfirst;
        int pendinglast;
#else /* ! WITH_THREAD */
        _Py_atomic_int eval_breaker;
        _Py_atomic_int pendingcalls_to_do;
#define NPENDINGCALLS 32
        struct {
            int (*func)(void *);
            void *arg;
        } pendingcalls[NPENDINGCALLS];
        volatile int pendingfirst;
        volatile int pendinglast;
#endif /* WITH_THREAD */

        struct _gil_globals {
            /* microseconds (the Python API uses seconds, though) */
            unsigned long interval;
            /* Last PyThreadState holding / having held the GIL. This helps us know
               whether anyone else was scheduled after we dropped the GIL. */
            _Py_atomic_address last_holder;
            /* Whether the GIL is already taken (-1 if uninitialized). This is atomic
               because it can be read without any lock taken in ceval.c. */
            _Py_atomic_int locked;
            /* Number of GIL switches since the beginning. */
            unsigned long switch_number;
#ifdef WITH_THREAD
            /* This condition variable allows one or several threads to wait until
               the GIL is released. In addition, the mutex also protects the above
               variables. */
            PyCOND_T cond;
            PyMUTEX_T mutex;
#ifdef FORCE_SWITCHING
            /* This condition variable helps the GIL-releasing thread wait for
               a GIL-awaiting thread to be scheduled and take the GIL. */
            PyCOND_T switch_cond;
            PyMUTEX_T switch_mutex;
#endif
#endif /* WITH_THREAD */
            int gilstate_check_enabled;
            /* Assuming the current thread holds the GIL, this is the
               PyThreadState for the current thread. */
            _Py_atomic_address tstate_current;
            PyThreadFrameGetter getframe;
#ifdef WITH_THREAD
            /* The single PyInterpreterState used by this process'
               GILState implementation
            */
            /* TODO: Given interp_main, it may be possible to kill this ref */
            PyInterpreterState *autoInterpreterState;
            int autoTLSkey;
#endif /* WITH_THREAD */
        } gil;
#endif /* Py_BUILD_CORE */
    } ceval;

    // XXX Consolidate globals found via the check-c-globals script.
} _PyRuntimeState;

PyAPI_DATA(_PyRuntimeState) _PyRuntime;
#endif /* !Py_LIMITED_API */

#ifndef Py_LIMITED_API
PyAPI_FUNC(void) _PyInterpreterState_Init(void);
#endif /* !Py_LIMITED_API */
PyAPI_FUNC(PyInterpreterState *) PyInterpreterState_New(void);
PyAPI_FUNC(void) PyInterpreterState_Clear(PyInterpreterState *);
PyAPI_FUNC(void) PyInterpreterState_Delete(PyInterpreterState *);
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03070000
/* New in 3.7 */
PyAPI_FUNC(int64_t) PyInterpreterState_GetID(PyInterpreterState *);
#endif
#ifndef Py_LIMITED_API
PyAPI_FUNC(int) _PyState_AddModule(PyObject*, struct PyModuleDef*);
#endif /* !Py_LIMITED_API */
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
/* New in 3.3 */
PyAPI_FUNC(int) PyState_AddModule(PyObject*, struct PyModuleDef*);
PyAPI_FUNC(int) PyState_RemoveModule(struct PyModuleDef*);
#endif
PyAPI_FUNC(PyObject*) PyState_FindModule(struct PyModuleDef*);
#ifndef Py_LIMITED_API
PyAPI_FUNC(void) _PyState_ClearModules(void);
#endif

PyAPI_FUNC(PyThreadState *) PyThreadState_New(PyInterpreterState *);
#ifndef Py_LIMITED_API
PyAPI_FUNC(PyThreadState *) _PyThreadState_Prealloc(PyInterpreterState *);
PyAPI_FUNC(void) _PyThreadState_Init(PyThreadState *);
#endif /* !Py_LIMITED_API */
PyAPI_FUNC(void) PyThreadState_Clear(PyThreadState *);
PyAPI_FUNC(void) PyThreadState_Delete(PyThreadState *);
#ifndef Py_LIMITED_API
PyAPI_FUNC(void) _PyThreadState_DeleteExcept(PyThreadState *tstate);
#endif /* !Py_LIMITED_API */
PyAPI_FUNC(void) PyThreadState_DeleteCurrent(void);
#ifndef Py_LIMITED_API
PyAPI_FUNC(void) _PyGILState_Reinit(void);
#endif /* !Py_LIMITED_API */

/* Return the current thread state. The global interpreter lock must be held.
 * When the current thread state is NULL, this issues a fatal error (so that
 * the caller needn't check for NULL). */
PyAPI_FUNC(PyThreadState *) PyThreadState_Get(void);

#ifndef Py_LIMITED_API
/* Similar to PyThreadState_Get(), but don't issue a fatal error
 * if it is NULL. */
PyAPI_FUNC(PyThreadState *) _PyThreadState_UncheckedGet(void);
#endif /* !Py_LIMITED_API */

PyAPI_FUNC(PyThreadState *) PyThreadState_Swap(PyThreadState *);
PyAPI_FUNC(PyObject *) PyThreadState_GetDict(void);
PyAPI_FUNC(int) PyThreadState_SetAsyncExc(unsigned long, PyObject *);


/* Variable and macro for in-line access to current thread state */

/* Assuming the current thread holds the GIL, this is the
   PyThreadState for the current thread. */
#ifdef Py_BUILD_CORE
#  define _PyThreadState_Current _PyRuntime.ceval.gil.tstate_current
#  define PyThreadState_GET() \
             ((PyThreadState*)_Py_atomic_load_relaxed(&_PyThreadState_Current))
#else
#  define PyThreadState_GET() PyThreadState_Get()
#endif

typedef
    enum {PyGILState_LOCKED, PyGILState_UNLOCKED}
        PyGILState_STATE;


/* Ensure that the current thread is ready to call the Python
   C API, regardless of the current state of Python, or of its
   thread lock.  This may be called as many times as desired
   by a thread so long as each call is matched with a call to
   PyGILState_Release().  In general, other thread-state APIs may
   be used between _Ensure() and _Release() calls, so long as the
   thread-state is restored to its previous state before the Release().
   For example, normal use of the Py_BEGIN_ALLOW_THREADS/
   Py_END_ALLOW_THREADS macros are acceptable.

   The return value is an opaque "handle" to the thread state when
   PyGILState_Ensure() was called, and must be passed to
   PyGILState_Release() to ensure Python is left in the same state. Even
   though recursive calls are allowed, these handles can *not* be shared -
   each unique call to PyGILState_Ensure must save the handle for its
   call to PyGILState_Release.

   When the function returns, the current thread will hold the GIL.

   Failure is a fatal error.
*/
PyAPI_FUNC(PyGILState_STATE) PyGILState_Ensure(void);

/* Release any resources previously acquired.  After this call, Python's
   state will be the same as it was prior to the corresponding
   PyGILState_Ensure() call (but generally this state will be unknown to
   the caller, hence the use of the GILState API.)

   Every call to PyGILState_Ensure must be matched by a call to
   PyGILState_Release on the same thread.
*/
PyAPI_FUNC(void) PyGILState_Release(PyGILState_STATE);

/* Helper/diagnostic function - get the current thread state for
   this thread.  May return NULL if no GILState API has been used
   on the current thread.  Note that the main thread always has such a
   thread-state, even if no auto-thread-state call has been made
   on the main thread.
*/
PyAPI_FUNC(PyThreadState *) PyGILState_GetThisThreadState(void);

#ifndef Py_LIMITED_API
/* Issue #26558: Flag to disable PyGILState_Check().
   If set to non-zero, PyGILState_Check() always return 1. */
#define _PyGILState_check_enabled _PyRuntime.ceval.gil.gilstate_check_enabled

/* Helper/diagnostic function - return 1 if the current thread
   currently holds the GIL, 0 otherwise.

   The function returns 1 if _PyGILState_check_enabled is non-zero. */
PyAPI_FUNC(int) PyGILState_Check(void);

/* Unsafe function to get the single PyInterpreterState used by this process'
   GILState implementation.

   Return NULL before _PyGILState_Init() is called and after _PyGILState_Fini()
   is called. */
PyAPI_FUNC(PyInterpreterState *) _PyGILState_GetInterpreterStateUnsafe(void);
#endif


/* The implementation of sys._current_frames()  Returns a dict mapping
   thread id to that thread's current frame.
*/
#ifndef Py_LIMITED_API
PyAPI_FUNC(PyObject *) _PyThread_CurrentFrames(void);
#endif

/* Routines for advanced debuggers, requested by David Beazley.
   Don't use unless you know what you are doing! */
#ifndef Py_LIMITED_API
PyAPI_FUNC(PyInterpreterState *) PyInterpreterState_Main(void);
PyAPI_FUNC(PyInterpreterState *) PyInterpreterState_Head(void);
PyAPI_FUNC(PyInterpreterState *) PyInterpreterState_Next(PyInterpreterState *);
PyAPI_FUNC(PyThreadState *) PyInterpreterState_ThreadHead(PyInterpreterState *);
PyAPI_FUNC(PyThreadState *) PyThreadState_Next(PyThreadState *);
#endif

/* hook for PyEval_GetFrame(), requested for Psyco */
#ifndef Py_LIMITED_API
#define _PyThreadState_GetFrame _PyRuntime.ceval.gil.getframe
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_PYSTATE_H */
