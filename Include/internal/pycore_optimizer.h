#ifndef Py_INTERNAL_OPTIMIZER_H
#define Py_INTERNAL_OPTIMIZER_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

#include "pycore_uops.h"

int uop_analyze_and_optimize(_PyUOpInstruction *trace, int trace_len);

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_OPTIMIZER_H */
