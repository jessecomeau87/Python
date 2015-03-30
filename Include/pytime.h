#ifndef Py_LIMITED_API
#ifndef Py_PYTIME_H
#define Py_PYTIME_H

#include "pyconfig.h" /* include for defines */
#include "object.h"

/**************************************************************************
Symbols and macros to supply platform-independent interfaces to time related
functions and constants
**************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef PY_INT64_T
/* _PyTime_t: Python timestamp with subsecond precision. It can be used to
   store a duration, and so indirectly a date (related to another date, like
   UNIX epoch). */
typedef PY_INT64_T _PyTime_t;
#define _PyTime_MIN PY_LLONG_MIN
#define _PyTime_MAX PY_LLONG_MAX
#else
#  error "_PyTime_t need signed 64-bit integer type"
#endif

typedef enum {
    /* Round towards zero. */
    _PyTime_ROUND_DOWN=0,
    /* Round away from zero.
       For example, used for timeout to wait "at least" N seconds. */
    _PyTime_ROUND_UP,
    /* Round towards minus infinity (-inf).
       For example, used to read a clock. */
    _PyTime_ROUND_FLOOR,
    /* Round towards infinity (+inf).
       For example, used for timeout to wait "at least" N seconds. */
    _PyTime_ROUND_CEILING
} _PyTime_round_t;

/* Convert a time_t to a PyLong. */
PyAPI_FUNC(PyObject *) _PyLong_FromTime_t(
    time_t sec);

/* Convert a PyLong to a time_t. */
PyAPI_FUNC(time_t) _PyLong_AsTime_t(
    PyObject *obj);

/* Convert a number of seconds, int or float, to time_t. */
PyAPI_FUNC(int) _PyTime_ObjectToTime_t(
    PyObject *obj,
    time_t *sec,
    _PyTime_round_t);

/* Convert a number of seconds, int or float, to a timeval structure.
   usec is in the range [0; 999999] and rounded towards zero.
   For example, -1.2 is converted to (-2, 800000). */
PyAPI_FUNC(int) _PyTime_ObjectToTimeval(
    PyObject *obj,
    time_t *sec,
    long *usec,
    _PyTime_round_t);

/* Convert a number of seconds, int or float, to a timespec structure.
   nsec is in the range [0; 999999999] and rounded towards zero.
   For example, -1.2 is converted to (-2, 800000000). */
PyAPI_FUNC(int) _PyTime_ObjectToTimespec(
    PyObject *obj,
    time_t *sec,
    long *nsec,
    _PyTime_round_t);


/* Create a timestamp from a number of nanoseconds (C long). */
PyAPI_FUNC(_PyTime_t) _PyTime_FromNanoseconds(PY_LONG_LONG ns);

/* Convert a Python float or int to a timetamp.
   Raise an exception and return -1 on error, return 0 on success. */
PyAPI_FUNC(int) _PyTime_FromSecondsObject(_PyTime_t *t,
    PyObject *obj,
    _PyTime_round_t round);

/* Convert a timestamp to a number of seconds as a C double. */
PyAPI_FUNC(double) _PyTime_AsSecondsDouble(_PyTime_t t);

/* Convert timestamp to a number of milliseconds (10^-3 seconds). */
PyAPI_FUNC(_PyTime_t) _PyTime_AsMilliseconds(_PyTime_t t,
    _PyTime_round_t round);

/* Convert timestamp to a number of microseconds (10^-6 seconds). */
PyAPI_FUNC(_PyTime_t) _PyTime_AsMicroseconds(_PyTime_t t,
    _PyTime_round_t round);

/* Convert timestamp to a number of nanoseconds (10^-9 seconds) as a Python int
   object. */
PyAPI_FUNC(PyObject *) _PyTime_AsNanosecondsObject(_PyTime_t t);

/* Convert a timestamp to a timeval structure (microsecond resolution).
   tv_usec is always positive.
   Raise an exception and return -1 if the conversion overflowed,
   return 0 on success. */
PyAPI_FUNC(int) _PyTime_AsTimeval(_PyTime_t t,
    struct timeval *tv,
    _PyTime_round_t round);

/* Similar to _PyTime_AsTimeval(), but don't raise an exception on error. */
PyAPI_FUNC(int) _PyTime_AsTimeval_noraise(_PyTime_t t,
    struct timeval *tv,
    _PyTime_round_t round);

#if defined(HAVE_CLOCK_GETTIME) || defined(HAVE_KQUEUE)
/* Convert a timestamp to a timespec structure (nanosecond resolution).
   tv_nsec is always positive.
   Raise an exception and return -1 on error, return 0 on success. */
PyAPI_FUNC(int) _PyTime_AsTimespec(_PyTime_t t, struct timespec *ts);
#endif

/* Get the current time from the system clock.

   The function cannot fail. _PyTime_Init() ensures that the system clock
   works. */
PyAPI_FUNC(_PyTime_t) _PyTime_GetSystemClock(void);

/* Get the time of a monotonic clock, i.e. a clock that cannot go backwards.
   The clock is not affected by system clock updates. The reference point of
   the returned value is undefined, so that only the difference between the
   results of consecutive calls is valid.

   The function cannot fail. _PyTime_Init() ensures that a monotonic clock
   is available and works. */
PyAPI_FUNC(_PyTime_t) _PyTime_GetMonotonicClock(void);


/* Structure used by time.get_clock_info() */
typedef struct {
    const char *implementation;
    int monotonic;
    int adjustable;
    double resolution;
} _Py_clock_info_t;

/* Get the current time from the system clock.
 * Fill clock information if info is not NULL.
 * Raise an exception and return -1 on error, return 0 on success.
 */
PyAPI_FUNC(int) _PyTime_GetSystemClockWithInfo(
    _PyTime_t *t,
    _Py_clock_info_t *info);

/* Get the time of a monotonic clock, i.e. a clock that cannot go backwards.
   The clock is not affected by system clock updates. The reference point of
   the returned value is undefined, so that only the difference between the
   results of consecutive calls is valid.

   Fill info (if set) with information of the function used to get the time.

   Return 0 on success, raise an exception and return -1 on error. */
PyAPI_FUNC(int) _PyTime_GetMonotonicClockWithInfo(
    _PyTime_t *t,
    _Py_clock_info_t *info);


/* Initialize time.
   Return 0 on success, raise an exception and return -1 on error. */
PyAPI_FUNC(int) _PyTime_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* Py_PYTIME_H */
#endif /* Py_LIMITED_API */
