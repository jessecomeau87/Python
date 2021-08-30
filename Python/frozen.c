
/* Frozen modules initializer
 *
 * Frozen modules are written to header files by Programs/_freeze_module.
 * These files are typically put in Python/frozen_modules/.  Each holds
 * an array of bytes named "_Py_M__<module>", which is used below.
 *
 * These files must be regenerated any time the corresponding .pyc
 * file would change (including with changes to the compiler, bytecode
 * format, marshal format).  This can be done with "make regen-frozen".
 * That make target normally does nothing more than run
 * Programs/_freeze_module on each of the modules set to be frozen.
 *
 * Additional modules can be frozen by updating the list at the top of
 * Tools/scripts/freeze_modules.py and then run the script
 * (or run "make regen-frozen").
 *
 * The script does the following (by default):
 *
 * 1. run Programs/_freeze_module on the target modules
 * 2. update the includes and _PyImport_FrozenModules[] in this file
 * 3. update the FROZEN_FILES variable in Makefile.pre.in
 * 4. update the per-module targets in Makefile.pre.in
 * 5. update the lists of modules in PCbuild/_freeze_module.vcxproj and
 *    PCbuild/_freeze_module.vcxproj.filters
 *
 * (Note that most of the data in this file is auto-generated by the script.)
 *
 * Those steps can also be done manually, though this is not recommended.
 * Expect such manual changes to be removed the next time
 * freeze_modules.py runs.
 * */

/* In order to test the support for frozen modules, by default we
   define some simple frozen modules: __hello__, __phello__ (a package),
   and __phello__.spam.  Loading any will print some famous words... */

#include "Python.h"

/* Includes for frozen modules: */
#include "frozen_modules/importlib__bootstrap.h"
#include "frozen_modules/importlib__bootstrap_external.h"
#include "frozen_modules/zipimport.h"
#include "frozen_modules/hello.h"
/* End includes */

/* Note that a negative size indicates a package. */

static const struct _frozen _PyImport_FrozenModules[] = {
    /* importlib */
    {"_frozen_importlib", _Py_M__importlib__bootstrap,
        (int)sizeof(_Py_M__importlib__bootstrap)},
    {"_frozen_importlib_external", _Py_M__importlib__bootstrap_external,
        (int)sizeof(_Py_M__importlib__bootstrap_external)},
    {"zipimport", _Py_M__zipimport, (int)sizeof(_Py_M__zipimport)},

    /* Test module */
    {"__hello__", _Py_M__hello, (int)sizeof(_Py_M__hello)},
    {"__phello__", _Py_M__hello, -(int)sizeof(_Py_M__hello)},
    {"__phello__.spam", _Py_M__hello, (int)sizeof(_Py_M__hello)},
    {0, 0, 0} /* sentinel */
};

/* Embedding apps may change this pointer to point to their favorite
   collection of frozen modules: */

const struct _frozen *PyImport_FrozenModules = _PyImport_FrozenModules;
