#ifndef Py_LIMITED_API
#ifndef Py_INTERNAL_IMPORT_H
#define Py_INTERNAL_IMPORT_H
#ifdef __cplusplus
extern "C" {
#endif


struct _import_runtime_state {
    /* The builtin modules (defined in config.c). */
    struct _inittab *inittab;
    /* The most recent value assigned to a PyModuleDef.m_base.m_index.
       This is incremented each time PyModuleDef_Init() is called,
       which is just about every time an extension module is imported.
       See PyInterpreterState.modules_by_index for more info. */
    Py_ssize_t last_module_index;
    /* A dict mapping (filename, name) to PyModuleDef for modules.
       Only legacy (single-phase init) extension modules are added
       and only if they support multiple initialization (m_size >- 0)
       or are imported in the main interpreter.
       This is initialized lazily in _PyImport_FixupExtensionObject().
       Modules are added there and looked up in _imp.find_extension(). */
    PyObject *extensions;
    /* The global import lock. */
    struct {
        PyThread_type_lock mutex;
        unsigned long thread;
        int level;
    } lock;
    struct {
        int import_level;
        _PyTime_t accumulated;
        int header;
    } find_and_load;
    /* Package context -- the full module name for package imports */
    const char * pkgcontext;
};

extern void _PyImport_ClearCore(PyInterpreterState *interp);

extern Py_ssize_t _PyImport_GetNextModuleIndex(void);
extern const char * _PyImport_ResolveNameWithPackageContext(const char *name);
extern const char * _PyImport_SwapPackageContext(const char *newcontext);

extern int _PyImport_GetDLOpenFlags(PyInterpreterState *interp);
extern void _PyImport_SetDLOpenFlags(PyInterpreterState *interp, int new_val);

extern PyObject * _PyImport_InitModules(PyInterpreterState *interp);
extern PyObject * _PyImport_GetModules(PyInterpreterState *interp);
extern void _PyImport_ClearModules(PyInterpreterState *interp);

extern void _PyImport_ClearModulesByIndex(PyInterpreterState *interp);

extern int _PyImport_InitDefaultImportFunc(PyInterpreterState *interp);
extern int _PyImport_IsDefaultImportFunc(
        PyInterpreterState *interp,
        PyObject *func);

extern PyObject * _PyImport_GetImportlibLoader(
        PyInterpreterState *interp,
        const char *loader_name);
extern PyObject * _PyImport_GetImportlibExternalLoader(
        PyInterpreterState *interp,
        const char *loader_name);
extern PyObject * _PyImport_BlessMyLoader(
        PyInterpreterState *interp,
        PyObject *module_globals);
extern PyObject * _PyImport_ImportlibModuleRepr(
        PyInterpreterState *interp,
        PyObject *module);


extern PyStatus _PyImport_Init(void);
extern void _PyImport_Fini(void);
extern void _PyImport_Fini2(void);

extern PyStatus _PyImport_InitCore(
        PyThreadState *tstate,
        PyObject *sysmod,
        int importlib);
extern PyStatus _PyImport_InitExternal(PyThreadState *tstate);
extern void _PyImport_FiniCore(PyInterpreterState *interp);
extern void _PyImport_FiniExternal(PyInterpreterState *interp);


#ifdef HAVE_FORK
extern PyStatus _PyImport_ReInitLock(void);
#endif


extern PyObject* _PyImport_GetBuiltinModuleNames(void);

struct _module_alias {
    const char *name;                 /* ASCII encoded string */
    const char *orig;                 /* ASCII encoded string */
};

PyAPI_DATA(const struct _frozen *) _PyImport_FrozenBootstrap;
PyAPI_DATA(const struct _frozen *) _PyImport_FrozenStdlib;
PyAPI_DATA(const struct _frozen *) _PyImport_FrozenTest;
extern const struct _module_alias * _PyImport_FrozenAliases;

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_IMPORT_H */
#endif /* !Py_LIMITED_API */
