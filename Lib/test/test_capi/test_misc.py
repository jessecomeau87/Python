# Run the _testcapi module tests (tests for the Python/C API):  by defn,
# these are all functions _testcapi exports whose name begins with 'test_'.

from collections import OrderedDict
import _thread
import importlib.machinery
import importlib.util
import itertools
import os
import pickle
import random
import subprocess
import sys
import textwrap
import threading
import time
import unittest
import warnings
import weakref
import operator
from test import support
from test.support import MISSING_C_DOCSTRINGS
from test.support import import_helper
from test.support import threading_helper
from test.support import warnings_helper
from test.support import requires_limited_api
from test.support.script_helper import assert_python_failure, assert_python_ok, run_python_until_end
try:
    import _posixsubprocess
except ImportError:
    _posixsubprocess = None
try:
    import _testmultiphase
except ImportError:
    _testmultiphase = None
try:
    import _testsinglephase
except ImportError:
    _testsinglephase = None

# Skip this test if the _testcapi module isn't available.
_testcapi = import_helper.import_module('_testcapi')

import _testinternalcapi


def decode_stderr(err):
    return err.decode('utf-8', 'replace').replace('\r', '')


def testfunction(self):
    """some doc"""
    return self


class InstanceMethod:
    id = _testcapi.instancemethod(id)
    testfunction = _testcapi.instancemethod(testfunction)

class CAPITest(unittest.TestCase):

    def test_instancemethod(self):
        inst = InstanceMethod()
        self.assertEqual(id(inst), inst.id())
        self.assertTrue(inst.testfunction() is inst)
        self.assertEqual(inst.testfunction.__doc__, testfunction.__doc__)
        self.assertEqual(InstanceMethod.testfunction.__doc__, testfunction.__doc__)

        InstanceMethod.testfunction.attribute = "test"
        self.assertEqual(testfunction.attribute, "test")
        self.assertRaises(AttributeError, setattr, inst.testfunction, "attribute", "test")

    @support.requires_subprocess()
    def test_no_FatalError_infinite_loop(self):
        run_result, _cmd_line = run_python_until_end(
            '-c', 'import _testcapi; _testcapi.crash_no_current_thread()',
        )
        _rc, out, err = run_result
        self.assertEqual(out, b'')
        # This used to cause an infinite loop.
        msg = ("Fatal Python error: PyThreadState_Get: "
               "the function must be called with the GIL held, "
               "after Python initialization and before Python finalization, "
               "but the GIL is released "
               "(the current Python thread state is NULL)").encode()
        self.assertTrue(err.rstrip().startswith(msg),
                        err)

    def test_memoryview_from_NULL_pointer(self):
        self.assertRaises(ValueError, _testcapi.make_memoryview_from_NULL_pointer)

    @unittest.skipUnless(_posixsubprocess, '_posixsubprocess required for this test.')
    def test_seq_bytes_to_charp_array(self):
        # Issue #15732: crash in _PySequence_BytesToCharpArray()
        class Z(object):
            def __len__(self):
                return 1
        with self.assertRaisesRegex(TypeError, 'indexing'):
            _posixsubprocess.fork_exec(
                          1,Z(),True,(1, 2),5,6,7,8,9,10,11,12,13,14,True,True,17,False,19,20,21,22,False)
        # Issue #15736: overflow in _PySequence_BytesToCharpArray()
        class Z(object):
            def __len__(self):
                return sys.maxsize
            def __getitem__(self, i):
                return b'x'
        self.assertRaises(MemoryError, _posixsubprocess.fork_exec,
                          1,Z(),True,(1, 2),5,6,7,8,9,10,11,12,13,14,True,True,17,False,19,20,21,22,False)

    @unittest.skipUnless(_posixsubprocess, '_posixsubprocess required for this test.')
    def test_subprocess_fork_exec(self):
        class Z(object):
            def __len__(self):
                return 1

        # Issue #15738: crash in subprocess_fork_exec()
        self.assertRaises(TypeError, _posixsubprocess.fork_exec,
                          Z(),[b'1'],True,(1, 2),5,6,7,8,9,10,11,12,13,14,True,True,17,False,19,20,21,22,False)

    @unittest.skipIf(MISSING_C_DOCSTRINGS,
                     "Signature information for builtins requires docstrings")
    def test_docstring_signature_parsing(self):

        self.assertEqual(_testcapi.no_docstring.__doc__, None)
        self.assertEqual(_testcapi.no_docstring.__text_signature__, None)

        self.assertEqual(_testcapi.docstring_empty.__doc__, None)
        self.assertEqual(_testcapi.docstring_empty.__text_signature__, None)

        self.assertEqual(_testcapi.docstring_no_signature.__doc__,
            "This docstring has no signature.")
        self.assertEqual(_testcapi.docstring_no_signature.__text_signature__, None)

        self.assertEqual(_testcapi.docstring_with_invalid_signature.__doc__,
            "docstring_with_invalid_signature($module, /, boo)\n"
            "\n"
            "This docstring has an invalid signature."
            )
        self.assertEqual(_testcapi.docstring_with_invalid_signature.__text_signature__, None)

        self.assertEqual(_testcapi.docstring_with_invalid_signature2.__doc__,
            "docstring_with_invalid_signature2($module, /, boo)\n"
            "\n"
            "--\n"
            "\n"
            "This docstring also has an invalid signature."
            )
        self.assertEqual(_testcapi.docstring_with_invalid_signature2.__text_signature__, None)

        self.assertEqual(_testcapi.docstring_with_signature.__doc__,
            "This docstring has a valid signature.")
        self.assertEqual(_testcapi.docstring_with_signature.__text_signature__, "($module, /, sig)")

        self.assertEqual(_testcapi.docstring_with_signature_but_no_doc.__doc__, None)
        self.assertEqual(_testcapi.docstring_with_signature_but_no_doc.__text_signature__,
            "($module, /, sig)")

        self.assertEqual(_testcapi.docstring_with_signature_and_extra_newlines.__doc__,
            "\nThis docstring has a valid signature and some extra newlines.")
        self.assertEqual(_testcapi.docstring_with_signature_and_extra_newlines.__text_signature__,
            "($module, /, parameter)")

    def test_c_type_with_matrix_multiplication(self):
        M = _testcapi.matmulType
        m1 = M()
        m2 = M()
        self.assertEqual(m1 @ m2, ("matmul", m1, m2))
        self.assertEqual(m1 @ 42, ("matmul", m1, 42))
        self.assertEqual(42 @ m1, ("matmul", 42, m1))
        o = m1
        o @= m2
        self.assertEqual(o, ("imatmul", m1, m2))
        o = m1
        o @= 42
        self.assertEqual(o, ("imatmul", m1, 42))
        o = 42
        o @= m1
        self.assertEqual(o, ("matmul", 42, m1))

    def test_c_type_with_ipow(self):
        # When the __ipow__ method of a type was implemented in C, using the
        # modulo param would cause segfaults.
        o = _testcapi.ipowType()
        self.assertEqual(o.__ipow__(1), (1, None))
        self.assertEqual(o.__ipow__(2, 2), (2, 2))

    def test_return_null_without_error(self):
        # Issue #23571: A function must not return NULL without setting an
        # error
        if support.Py_DEBUG:
            code = textwrap.dedent("""
                import _testcapi
                from test import support

                with support.SuppressCrashReport():
                    _testcapi.return_null_without_error()
            """)
            rc, out, err = assert_python_failure('-c', code)
            err = decode_stderr(err)
            self.assertRegex(err,
                r'Fatal Python error: _Py_CheckFunctionResult: '
                    r'a function returned NULL without setting an exception\n'
                r'Python runtime state: initialized\n'
                r'SystemError: <built-in function return_null_without_error> '
                    r'returned NULL without setting an exception\n'
                r'\n'
                r'Current thread.*:\n'
                r'  File .*", line 6 in <module>\n')
        else:
            with self.assertRaises(SystemError) as cm:
                _testcapi.return_null_without_error()
            self.assertRegex(str(cm.exception),
                             'return_null_without_error.* '
                             'returned NULL without setting an exception')

    def test_return_result_with_error(self):
        # Issue #23571: A function must not return a result with an error set
        if support.Py_DEBUG:
            code = textwrap.dedent("""
                import _testcapi
                from test import support

                with support.SuppressCrashReport():
                    _testcapi.return_result_with_error()
            """)
            rc, out, err = assert_python_failure('-c', code)
            err = decode_stderr(err)
            self.assertRegex(err,
                    r'Fatal Python error: _Py_CheckFunctionResult: '
                        r'a function returned a result with an exception set\n'
                    r'Python runtime state: initialized\n'
                    r'ValueError\n'
                    r'\n'
                    r'The above exception was the direct cause '
                        r'of the following exception:\n'
                    r'\n'
                    r'SystemError: <built-in '
                        r'function return_result_with_error> '
                        r'returned a result with an exception set\n'
                    r'\n'
                    r'Current thread.*:\n'
                    r'  File .*, line 6 in <module>\n')
        else:
            with self.assertRaises(SystemError) as cm:
                _testcapi.return_result_with_error()
            self.assertRegex(str(cm.exception),
                             'return_result_with_error.* '
                             'returned a result with an exception set')

    def test_getitem_with_error(self):
        # Test _Py_CheckSlotResult(). Raise an exception and then calls
        # PyObject_GetItem(): check that the assertion catches the bug.
        # PyObject_GetItem() must not be called with an exception set.
        code = textwrap.dedent("""
            import _testcapi
            from test import support

            with support.SuppressCrashReport():
                _testcapi.getitem_with_error({1: 2}, 1)
        """)
        rc, out, err = assert_python_failure('-c', code)
        err = decode_stderr(err)
        if 'SystemError: ' not in err:
            self.assertRegex(err,
                    r'Fatal Python error: _Py_CheckSlotResult: '
                        r'Slot __getitem__ of type dict succeeded '
                        r'with an exception set\n'
                    r'Python runtime state: initialized\n'
                    r'ValueError: bug\n'
                    r'\n'
                    r'Current thread .* \(most recent call first\):\n'
                    r'  File .*, line 6 in <module>\n'
                    r'\n'
                    r'Extension modules: _testcapi \(total: 1\)\n')
        else:
            # Python built with NDEBUG macro defined:
            # test _Py_CheckFunctionResult() instead.
            self.assertIn('returned a result with an exception set', err)

    def test_buildvalue_N(self):
        _testcapi.test_buildvalue_N()

    def test_mapping_keys_values_items(self):
        class Mapping1(dict):
            def keys(self):
                return list(super().keys())
            def values(self):
                return list(super().values())
            def items(self):
                return list(super().items())
        class Mapping2(dict):
            def keys(self):
                return tuple(super().keys())
            def values(self):
                return tuple(super().values())
            def items(self):
                return tuple(super().items())
        dict_obj = {'foo': 1, 'bar': 2, 'spam': 3}

        for mapping in [{}, OrderedDict(), Mapping1(), Mapping2(),
                        dict_obj, OrderedDict(dict_obj),
                        Mapping1(dict_obj), Mapping2(dict_obj)]:
            self.assertListEqual(_testcapi.get_mapping_keys(mapping),
                                 list(mapping.keys()))
            self.assertListEqual(_testcapi.get_mapping_values(mapping),
                                 list(mapping.values()))
            self.assertListEqual(_testcapi.get_mapping_items(mapping),
                                 list(mapping.items()))

    def test_mapping_keys_values_items_bad_arg(self):
        self.assertRaises(AttributeError, _testcapi.get_mapping_keys, None)
        self.assertRaises(AttributeError, _testcapi.get_mapping_values, None)
        self.assertRaises(AttributeError, _testcapi.get_mapping_items, None)

        class BadMapping:
            def keys(self):
                return None
            def values(self):
                return None
            def items(self):
                return None
        bad_mapping = BadMapping()
        self.assertRaises(TypeError, _testcapi.get_mapping_keys, bad_mapping)
        self.assertRaises(TypeError, _testcapi.get_mapping_values, bad_mapping)
        self.assertRaises(TypeError, _testcapi.get_mapping_items, bad_mapping)

    def test_mapping_has_key(self):
        dct = {'a': 1}
        self.assertTrue(_testcapi.mapping_has_key(dct, 'a'))
        self.assertFalse(_testcapi.mapping_has_key(dct, 'b'))

        class SubDict(dict):
            pass

        dct2 = SubDict({'a': 1})
        self.assertTrue(_testcapi.mapping_has_key(dct2, 'a'))
        self.assertFalse(_testcapi.mapping_has_key(dct2, 'b'))

    def test_sequence_set_slice(self):
        # Correct case:
        data = [1, 2, 3, 4, 5]
        data_copy = data.copy()

        _testcapi.sequence_set_slice(data, 1, 3, [8, 9])
        data_copy[1:3] = [8, 9]
        self.assertEqual(data, data_copy)
        self.assertEqual(data, [1, 8, 9, 4, 5])

        # Custom class:
        class Custom:
            def __setitem__(self, index, value):
                self.index = index
                self.value = value

        c = Custom()
        _testcapi.sequence_set_slice(c, 0, 5, 'abc')
        self.assertEqual(c.index, slice(0, 5))
        self.assertEqual(c.value, 'abc')

        # Immutable sequences must raise:
        bad_seq1 = (1, 2, 3, 4)
        with self.assertRaises(TypeError):
            _testcapi.sequence_set_slice(bad_seq1, 1, 3, (8, 9))
        self.assertEqual(bad_seq1, (1, 2, 3, 4))

        bad_seq2 = 'abcd'
        with self.assertRaises(TypeError):
            _testcapi.sequence_set_slice(bad_seq2, 1, 3, 'xy')
        self.assertEqual(bad_seq2, 'abcd')

        # Not a sequence:
        with self.assertRaises(TypeError):
            _testcapi.sequence_set_slice(None, 1, 3, 'xy')

    def test_sequence_del_slice(self):
        # Correct case:
        data = [1, 2, 3, 4, 5]
        data_copy = data.copy()

        _testcapi.sequence_del_slice(data, 1, 3)
        del data_copy[1:3]
        self.assertEqual(data, data_copy)
        self.assertEqual(data, [1, 4, 5])

        # Custom class:
        class Custom:
            def __delitem__(self, index):
                self.index = index

        c = Custom()
        _testcapi.sequence_del_slice(c, 0, 5)
        self.assertEqual(c.index, slice(0, 5))

        # Immutable sequences must raise:
        bad_seq1 = (1, 2, 3, 4)
        with self.assertRaises(TypeError):
            _testcapi.sequence_del_slice(bad_seq1, 1, 3)
        self.assertEqual(bad_seq1, (1, 2, 3, 4))

        bad_seq2 = 'abcd'
        with self.assertRaises(TypeError):
            _testcapi.sequence_del_slice(bad_seq2, 1, 3)
        self.assertEqual(bad_seq2, 'abcd')

        # Not a sequence:
        with self.assertRaises(TypeError):
            _testcapi.sequence_del_slice(None, 1, 3)

        mapping = {1: 'a', 2: 'b', 3: 'c'}
        with self.assertRaises(KeyError):
            _testcapi.sequence_del_slice(mapping, 1, 3)
        self.assertEqual(mapping, {1: 'a', 2: 'b', 3: 'c'})

    @unittest.skipUnless(hasattr(_testcapi, 'negative_refcount'),
                         'need _testcapi.negative_refcount')
    def test_negative_refcount(self):
        # bpo-35059: Check that Py_DECREF() reports the correct filename
        # when calling _Py_NegativeRefcount() to abort Python.
        code = textwrap.dedent("""
            import _testcapi
            from test import support

            with support.SuppressCrashReport():
                _testcapi.negative_refcount()
        """)
        rc, out, err = assert_python_failure('-c', code)
        self.assertRegex(err,
                         br'_testcapimodule\.c:[0-9]+: '
                         br'_Py_NegativeRefcount: Assertion failed: '
                         br'object has negative ref count')

    def test_trashcan_subclass(self):
        # bpo-35983: Check that the trashcan mechanism for "list" is NOT
        # activated when its tp_dealloc is being called by a subclass
        from _testcapi import MyList
        L = None
        for i in range(1000):
            L = MyList((L,))

    @support.requires_resource('cpu')
    def test_trashcan_python_class1(self):
        self.do_test_trashcan_python_class(list)

    @support.requires_resource('cpu')
    def test_trashcan_python_class2(self):
        from _testcapi import MyList
        self.do_test_trashcan_python_class(MyList)

    def do_test_trashcan_python_class(self, base):
        # Check that the trashcan mechanism works properly for a Python
        # subclass of a class using the trashcan (this specific test assumes
        # that the base class "base" behaves like list)
        class PyList(base):
            # Count the number of PyList instances to verify that there is
            # no memory leak
            num = 0
            def __init__(self, *args):
                __class__.num += 1
                super().__init__(*args)
            def __del__(self):
                __class__.num -= 1

        for parity in (0, 1):
            L = None
            # We need in the order of 2**20 iterations here such that a
            # typical 8MB stack would overflow without the trashcan.
            for i in range(2**20):
                L = PyList((L,))
                L.attr = i
            if parity:
                # Add one additional nesting layer
                L = (L,)
            self.assertGreater(PyList.num, 0)
            del L
            self.assertEqual(PyList.num, 0)

    def test_heap_ctype_doc_and_text_signature(self):
        self.assertEqual(_testcapi.HeapDocCType.__doc__, "somedoc")
        self.assertEqual(_testcapi.HeapDocCType.__text_signature__, "(arg1, arg2)")

    def test_null_type_doc(self):
        self.assertEqual(_testcapi.NullTpDocType.__doc__, None)

    def test_subclass_of_heap_gc_ctype_with_tpdealloc_decrefs_once(self):
        class HeapGcCTypeSubclass(_testcapi.HeapGcCType):
            def __init__(self):
                self.value2 = 20
                super().__init__()

        subclass_instance = HeapGcCTypeSubclass()
        type_refcnt = sys.getrefcount(HeapGcCTypeSubclass)

        # Test that subclass instance was fully created
        self.assertEqual(subclass_instance.value, 10)
        self.assertEqual(subclass_instance.value2, 20)

        # Test that the type reference count is only decremented once
        del subclass_instance
        self.assertEqual(type_refcnt - 1, sys.getrefcount(HeapGcCTypeSubclass))

    def test_subclass_of_heap_gc_ctype_with_del_modifying_dunder_class_only_decrefs_once(self):
        class A(_testcapi.HeapGcCType):
            def __init__(self):
                self.value2 = 20
                super().__init__()

        class B(A):
            def __init__(self):
                super().__init__()

            def __del__(self):
                self.__class__ = A
                A.refcnt_in_del = sys.getrefcount(A)
                B.refcnt_in_del = sys.getrefcount(B)

        subclass_instance = B()
        type_refcnt = sys.getrefcount(B)
        new_type_refcnt = sys.getrefcount(A)

        # Test that subclass instance was fully created
        self.assertEqual(subclass_instance.value, 10)
        self.assertEqual(subclass_instance.value2, 20)

        del subclass_instance

        # Test that setting __class__ modified the reference counts of the types
        if support.Py_DEBUG:
            # gh-89373: In debug mode, _Py_Dealloc() keeps a strong reference
            # to the type while calling tp_dealloc()
            self.assertEqual(type_refcnt, B.refcnt_in_del)
        else:
            self.assertEqual(type_refcnt - 1, B.refcnt_in_del)
        self.assertEqual(new_type_refcnt + 1, A.refcnt_in_del)

        # Test that the original type already has decreased its refcnt
        self.assertEqual(type_refcnt - 1, sys.getrefcount(B))

        # Test that subtype_dealloc decref the newly assigned __class__ only once
        self.assertEqual(new_type_refcnt, sys.getrefcount(A))

    def test_heaptype_with_dict(self):
        inst = _testcapi.HeapCTypeWithDict()
        inst.foo = 42
        self.assertEqual(inst.foo, 42)
        self.assertEqual(inst.dictobj, inst.__dict__)
        self.assertEqual(inst.dictobj, {"foo": 42})

        inst = _testcapi.HeapCTypeWithDict()
        self.assertEqual({}, inst.__dict__)

    def test_heaptype_with_managed_dict(self):
        inst = _testcapi.HeapCTypeWithManagedDict()
        inst.foo = 42
        self.assertEqual(inst.foo, 42)
        self.assertEqual(inst.__dict__, {"foo": 42})

        inst = _testcapi.HeapCTypeWithManagedDict()
        self.assertEqual({}, inst.__dict__)

        a = _testcapi.HeapCTypeWithManagedDict()
        b = _testcapi.HeapCTypeWithManagedDict()
        a.b = b
        b.a = a
        del a, b

    def test_sublclassing_managed_dict(self):

        class C(_testcapi.HeapCTypeWithManagedDict):
            pass

        i = C()
        i.spam = i
        del i

    def test_heaptype_with_negative_dict(self):
        inst = _testcapi.HeapCTypeWithNegativeDict()
        inst.foo = 42
        self.assertEqual(inst.foo, 42)
        self.assertEqual(inst.dictobj, inst.__dict__)
        self.assertEqual(inst.dictobj, {"foo": 42})

        inst = _testcapi.HeapCTypeWithNegativeDict()
        self.assertEqual({}, inst.__dict__)

    def test_heaptype_with_weakref(self):
        inst = _testcapi.HeapCTypeWithWeakref()
        ref = weakref.ref(inst)
        self.assertEqual(ref(), inst)
        self.assertEqual(inst.weakreflist, ref)

    def test_heaptype_with_managed_weakref(self):
        inst = _testcapi.HeapCTypeWithManagedWeakref()
        ref = weakref.ref(inst)
        self.assertEqual(ref(), inst)

    def test_sublclassing_managed_weakref(self):

        class C(_testcapi.HeapCTypeWithManagedWeakref):
            pass

        inst = C()
        ref = weakref.ref(inst)
        self.assertEqual(ref(), inst)

    def test_sublclassing_managed_both(self):

        class C1(_testcapi.HeapCTypeWithManagedWeakref, _testcapi.HeapCTypeWithManagedDict):
            pass

        class C2(_testcapi.HeapCTypeWithManagedDict, _testcapi.HeapCTypeWithManagedWeakref):
            pass

        for cls in (C1, C2):
            inst = cls()
            ref = weakref.ref(inst)
            self.assertEqual(ref(), inst)
            inst.spam = inst
            del inst
            ref = weakref.ref(cls())
            self.assertIs(ref(), None)

    def test_heaptype_with_buffer(self):
        inst = _testcapi.HeapCTypeWithBuffer()
        b = bytes(inst)
        self.assertEqual(b, b"1234")

    def test_c_subclass_of_heap_ctype_with_tpdealloc_decrefs_once(self):
        subclass_instance = _testcapi.HeapCTypeSubclass()
        type_refcnt = sys.getrefcount(_testcapi.HeapCTypeSubclass)

        # Test that subclass instance was fully created
        self.assertEqual(subclass_instance.value, 10)
        self.assertEqual(subclass_instance.value2, 20)

        # Test that the type reference count is only decremented once
        del subclass_instance
        self.assertEqual(type_refcnt - 1, sys.getrefcount(_testcapi.HeapCTypeSubclass))

    def test_c_subclass_of_heap_ctype_with_del_modifying_dunder_class_only_decrefs_once(self):
        subclass_instance = _testcapi.HeapCTypeSubclassWithFinalizer()
        type_refcnt = sys.getrefcount(_testcapi.HeapCTypeSubclassWithFinalizer)
        new_type_refcnt = sys.getrefcount(_testcapi.HeapCTypeSubclass)

        # Test that subclass instance was fully created
        self.assertEqual(subclass_instance.value, 10)
        self.assertEqual(subclass_instance.value2, 20)

        # The tp_finalize slot will set __class__ to HeapCTypeSubclass
        del subclass_instance

        # Test that setting __class__ modified the reference counts of the types
        if support.Py_DEBUG:
            # gh-89373: In debug mode, _Py_Dealloc() keeps a strong reference
            # to the type while calling tp_dealloc()
            self.assertEqual(type_refcnt, _testcapi.HeapCTypeSubclassWithFinalizer.refcnt_in_del)
        else:
            self.assertEqual(type_refcnt - 1, _testcapi.HeapCTypeSubclassWithFinalizer.refcnt_in_del)
        self.assertEqual(new_type_refcnt + 1, _testcapi.HeapCTypeSubclass.refcnt_in_del)

        # Test that the original type already has decreased its refcnt
        self.assertEqual(type_refcnt - 1, sys.getrefcount(_testcapi.HeapCTypeSubclassWithFinalizer))

        # Test that subtype_dealloc decref the newly assigned __class__ only once
        self.assertEqual(new_type_refcnt, sys.getrefcount(_testcapi.HeapCTypeSubclass))

    def test_heaptype_with_setattro(self):
        obj = _testcapi.HeapCTypeSetattr()
        self.assertEqual(obj.pvalue, 10)
        obj.value = 12
        self.assertEqual(obj.pvalue, 12)
        del obj.value
        self.assertEqual(obj.pvalue, 0)

    def test_heaptype_with_custom_metaclass(self):
        self.assertTrue(issubclass(_testcapi.HeapCTypeMetaclass, type))
        self.assertTrue(issubclass(_testcapi.HeapCTypeMetaclassCustomNew, type))

        t = _testcapi.pytype_fromspec_meta(_testcapi.HeapCTypeMetaclass)
        self.assertIsInstance(t, type)
        self.assertEqual(t.__name__, "HeapCTypeViaMetaclass")
        self.assertIs(type(t), _testcapi.HeapCTypeMetaclass)

        msg = "Metaclasses with custom tp_new are not supported."
        with self.assertRaisesRegex(TypeError, msg):
            t = _testcapi.pytype_fromspec_meta(_testcapi.HeapCTypeMetaclassCustomNew)

    def test_heaptype_with_custom_metaclass_deprecation(self):
        # gh-103968: a metaclass with custom tp_new is deprecated, but still
        # allowed for functions that existed in 3.11
        # (PyType_FromSpecWithBases is used here).
        class Base(metaclass=_testcapi.HeapCTypeMetaclassCustomNew):
            pass

        with warnings_helper.check_warnings(
                ('.*custom tp_new.*in Python 3.14.*', DeprecationWarning),
                ):
            sub = _testcapi.make_type_with_base(Base)
        self.assertTrue(issubclass(sub, Base))
        self.assertIsInstance(sub, _testcapi.HeapCTypeMetaclassCustomNew)

    def test_multiple_inheritance_ctypes_with_weakref_or_dict(self):

        with self.assertRaises(TypeError):
            class Both1(_testcapi.HeapCTypeWithWeakref, _testcapi.HeapCTypeWithDict):
                pass
        with self.assertRaises(TypeError):
            class Both2(_testcapi.HeapCTypeWithDict, _testcapi.HeapCTypeWithWeakref):
                pass

    def test_multiple_inheritance_ctypes_with_weakref_or_dict_and_other_builtin(self):

        with self.assertRaises(TypeError):
            class C1(_testcapi.HeapCTypeWithDict, list):
                pass

        with self.assertRaises(TypeError):
            class C2(_testcapi.HeapCTypeWithWeakref, list):
                pass

        class C3(_testcapi.HeapCTypeWithManagedDict, list):
            pass
        class C4(_testcapi.HeapCTypeWithManagedWeakref, list):
            pass

        inst = C3()
        inst.append(0)
        str(inst.__dict__)

        inst = C4()
        inst.append(0)
        str(inst.__weakref__)

        for cls in (_testcapi.HeapCTypeWithManagedDict, _testcapi.HeapCTypeWithManagedWeakref):
            for cls2 in (_testcapi.HeapCTypeWithDict, _testcapi.HeapCTypeWithWeakref):
                class S(cls, cls2):
                    pass
            class B1(C3, cls):
                pass
            class B2(C4, cls):
                pass

    def test_pytype_fromspec_with_repeated_slots(self):
        for variant in range(2):
            with self.subTest(variant=variant):
                with self.assertRaises(SystemError):
                    _testcapi.create_type_from_repeated_slots(variant)

    @warnings_helper.ignore_warnings(category=DeprecationWarning)
    def test_immutable_type_with_mutable_base(self):
        # Add deprecation warning here so it's removed in 3.14
        warnings._deprecated(
            'creating immutable classes with mutable bases', remove=(3, 14))

        class MutableBase:
            def meth(self):
                return 'original'

        with self.assertWarns(DeprecationWarning):
            ImmutableSubclass = _testcapi.make_immutable_type_with_base(
                MutableBase)
        instance = ImmutableSubclass()

        self.assertEqual(instance.meth(), 'original')

        # Cannot override the static type's method
        with self.assertRaisesRegex(
                TypeError,
                "cannot set 'meth' attribute of immutable type"):
            ImmutableSubclass.meth = lambda self: 'overridden'
        self.assertEqual(instance.meth(), 'original')

        # Can change the method on the mutable base
        MutableBase.meth = lambda self: 'changed'
        self.assertEqual(instance.meth(), 'changed')

    def test_pynumber_tobase(self):
        from _testcapi import pynumber_tobase
        small_number = 123
        large_number = 2**64
        class IDX:
            def __init__(self, val):
                self.val = val
            def __index__(self):
                return self.val

        test_cases = ((2, '0b1111011', '0b10000000000000000000000000000000000000000000000000000000000000000'),
                      (8, '0o173', '0o2000000000000000000000'),
                      (10, '123', '18446744073709551616'),
                      (16, '0x7b', '0x10000000000000000'))
        for base, small_target, large_target in test_cases:
            with self.subTest(base=base, st=small_target, lt=large_target):
                # Test for small number
                self.assertEqual(pynumber_tobase(small_number, base), small_target)
                self.assertEqual(pynumber_tobase(-small_number, base), '-' + small_target)
                self.assertEqual(pynumber_tobase(IDX(small_number), base), small_target)
                # Test for large number(out of range of a longlong,i.e.[-2**63, 2**63-1])
                self.assertEqual(pynumber_tobase(large_number, base), large_target)
                self.assertEqual(pynumber_tobase(-large_number, base), '-' + large_target)
                self.assertEqual(pynumber_tobase(IDX(large_number), base), large_target)
        self.assertRaises(TypeError, pynumber_tobase, IDX(123.0), 10)
        self.assertRaises(TypeError, pynumber_tobase, IDX('123'), 10)
        self.assertRaises(TypeError, pynumber_tobase, 123.0, 10)
        self.assertRaises(TypeError, pynumber_tobase, '123', 10)
        self.assertRaises(SystemError, pynumber_tobase, 123, 0)

    def test_pyobject_repr_from_null(self):
        s = _testcapi.pyobject_repr_from_null()
        self.assertEqual(s, '<NULL>')

    def test_pyobject_str_from_null(self):
        s = _testcapi.pyobject_str_from_null()
        self.assertEqual(s, '<NULL>')

    def test_pyobject_bytes_from_null(self):
        s = _testcapi.pyobject_bytes_from_null()
        self.assertEqual(s, b'<NULL>')

    def test_Py_CompileString(self):
        # Check that Py_CompileString respects the coding cookie
        _compile = _testcapi.Py_CompileString
        code = b"# -*- coding: latin1 -*-\nprint('\xc2\xa4')\n"
        result = _compile(code)
        expected = compile(code, "<string>", "exec")
        self.assertEqual(result.co_consts, expected.co_consts)

    def test_export_symbols(self):
        # bpo-44133: Ensure that the "Py_FrozenMain" and
        # "PyThread_get_thread_native_id" symbols are exported by the Python
        # (directly by the binary, or via by the Python dynamic library).
        ctypes = import_helper.import_module('ctypes')
        names = []

        # Test if the PY_HAVE_THREAD_NATIVE_ID macro is defined
        if hasattr(_thread, 'get_native_id'):
            names.append('PyThread_get_thread_native_id')

        # Python/frozenmain.c fails to build on Windows when the symbols are
        # missing:
        # - PyWinFreeze_ExeInit
        # - PyWinFreeze_ExeTerm
        # - PyInitFrozenExtensions
        if os.name != 'nt':
            names.append('Py_FrozenMain')

        for name in names:
            with self.subTest(name=name):
                self.assertTrue(hasattr(ctypes.pythonapi, name))

    def test_clear_managed_dict(self):

        class C:
            def __init__(self):
                self.a = 1

        c = C()
        _testcapi.clear_managed_dict(c)
        self.assertEqual(c.__dict__, {})
        c = C()
        self.assertEqual(c.__dict__, {'a':1})
        _testcapi.clear_managed_dict(c)
        self.assertEqual(c.__dict__, {})

    def test_eval_get_func_name(self):
        def function_example(): ...

        class A:
            def method_example(self): ...

        self.assertEqual(_testcapi.eval_get_func_name(function_example),
                         "function_example")
        self.assertEqual(_testcapi.eval_get_func_name(A.method_example),
                         "method_example")
        self.assertEqual(_testcapi.eval_get_func_name(A().method_example),
                         "method_example")
        self.assertEqual(_testcapi.eval_get_func_name(sum), "sum")  # c function
        self.assertEqual(_testcapi.eval_get_func_name(A), "type")

    def test_eval_get_func_desc(self):
        def function_example(): ...

        class A:
            def method_example(self): ...

        self.assertEqual(_testcapi.eval_get_func_desc(function_example),
                         "()")
        self.assertEqual(_testcapi.eval_get_func_desc(A.method_example),
                         "()")
        self.assertEqual(_testcapi.eval_get_func_desc(A().method_example),
                         "()")
        self.assertEqual(_testcapi.eval_get_func_desc(sum), "()")  # c function
        self.assertEqual(_testcapi.eval_get_func_desc(A), " object")

    def test_function_get_code(self):
        import types

        def some():
            pass

        code = _testcapi.function_get_code(some)
        self.assertIsInstance(code, types.CodeType)
        self.assertEqual(code, some.__code__)

        with self.assertRaises(SystemError):
            _testcapi.function_get_code(None)  # not a function

    def test_function_get_globals(self):
        def some():
            pass

        globals_ = _testcapi.function_get_globals(some)
        self.assertIsInstance(globals_, dict)
        self.assertEqual(globals_, some.__globals__)

        with self.assertRaises(SystemError):
            _testcapi.function_get_globals(None)  # not a function

    def test_function_get_module(self):
        def some():
            pass

        module = _testcapi.function_get_module(some)
        self.assertIsInstance(module, str)
        self.assertEqual(module, some.__module__)

        with self.assertRaises(SystemError):
            _testcapi.function_get_module(None)  # not a function

    def test_function_get_defaults(self):
        def some(
            pos_only1, pos_only2='p',
            /,
            zero=0, optional=None,
            *,
            kw1,
            kw2=True,
        ):
            pass

        defaults = _testcapi.function_get_defaults(some)
        self.assertEqual(defaults, ('p', 0, None))
        self.assertEqual(defaults, some.__defaults__)

        with self.assertRaises(SystemError):
            _testcapi.function_get_defaults(None)  # not a function

    def test_function_set_defaults(self):
        def some(
            pos_only1, pos_only2='p',
            /,
            zero=0, optional=None,
            *,
            kw1,
            kw2=True,
        ):
            pass

        old_defaults = ('p', 0, None)
        self.assertEqual(_testcapi.function_get_defaults(some), old_defaults)
        self.assertEqual(some.__defaults__, old_defaults)

        with self.assertRaises(SystemError):
            _testcapi.function_set_defaults(some, 1)  # not tuple or None
        self.assertEqual(_testcapi.function_get_defaults(some), old_defaults)
        self.assertEqual(some.__defaults__, old_defaults)

        with self.assertRaises(SystemError):
            _testcapi.function_set_defaults(1, ())    # not a function
        self.assertEqual(_testcapi.function_get_defaults(some), old_defaults)
        self.assertEqual(some.__defaults__, old_defaults)

        new_defaults = ('q', 1, None)
        _testcapi.function_set_defaults(some, new_defaults)
        self.assertEqual(_testcapi.function_get_defaults(some), new_defaults)
        self.assertEqual(some.__defaults__, new_defaults)

        # Empty tuple is fine:
        new_defaults = ()
        _testcapi.function_set_defaults(some, new_defaults)
        self.assertEqual(_testcapi.function_get_defaults(some), new_defaults)
        self.assertEqual(some.__defaults__, new_defaults)

        class tuplesub(tuple): ...  # tuple subclasses must work

        new_defaults = tuplesub(((1, 2), ['a', 'b'], None))
        _testcapi.function_set_defaults(some, new_defaults)
        self.assertEqual(_testcapi.function_get_defaults(some), new_defaults)
        self.assertEqual(some.__defaults__, new_defaults)

        # `None` is special, it sets `defaults` to `NULL`,
        # it needs special handling in `_testcapi`:
        _testcapi.function_set_defaults(some, None)
        self.assertEqual(_testcapi.function_get_defaults(some), None)
        self.assertEqual(some.__defaults__, None)

    def test_function_get_kw_defaults(self):
        def some(
            pos_only1, pos_only2='p',
            /,
            zero=0, optional=None,
            *,
            kw1,
            kw2=True,
        ):
            pass

        defaults = _testcapi.function_get_kw_defaults(some)
        self.assertEqual(defaults, {'kw2': True})
        self.assertEqual(defaults, some.__kwdefaults__)

        with self.assertRaises(SystemError):
            _testcapi.function_get_kw_defaults(None)  # not a function

    def test_function_set_kw_defaults(self):
        def some(
            pos_only1, pos_only2='p',
            /,
            zero=0, optional=None,
            *,
            kw1,
            kw2=True,
        ):
            pass

        old_defaults = {'kw2': True}
        self.assertEqual(_testcapi.function_get_kw_defaults(some), old_defaults)
        self.assertEqual(some.__kwdefaults__, old_defaults)

        with self.assertRaises(SystemError):
            _testcapi.function_set_kw_defaults(some, 1)  # not dict or None
        self.assertEqual(_testcapi.function_get_kw_defaults(some), old_defaults)
        self.assertEqual(some.__kwdefaults__, old_defaults)

        with self.assertRaises(SystemError):
            _testcapi.function_set_kw_defaults(1, {})    # not a function
        self.assertEqual(_testcapi.function_get_kw_defaults(some), old_defaults)
        self.assertEqual(some.__kwdefaults__, old_defaults)

        new_defaults = {'kw2': (1, 2, 3)}
        _testcapi.function_set_kw_defaults(some, new_defaults)
        self.assertEqual(_testcapi.function_get_kw_defaults(some), new_defaults)
        self.assertEqual(some.__kwdefaults__, new_defaults)

        # Empty dict is fine:
        new_defaults = {}
        _testcapi.function_set_kw_defaults(some, new_defaults)
        self.assertEqual(_testcapi.function_get_kw_defaults(some), new_defaults)
        self.assertEqual(some.__kwdefaults__, new_defaults)

        class dictsub(dict): ...  # dict subclasses must work

        new_defaults = dictsub({'kw2': None})
        _testcapi.function_set_kw_defaults(some, new_defaults)
        self.assertEqual(_testcapi.function_get_kw_defaults(some), new_defaults)
        self.assertEqual(some.__kwdefaults__, new_defaults)

        # `None` is special, it sets `kwdefaults` to `NULL`,
        # it needs special handling in `_testcapi`:
        _testcapi.function_set_kw_defaults(some, None)
        self.assertEqual(_testcapi.function_get_kw_defaults(some), None)
        self.assertEqual(some.__kwdefaults__, None)

    def test_unstable_gc_new_with_extra_data(self):
        class Data(_testcapi.ObjExtraData):
            __slots__ = ('x', 'y')

        d = Data()
        d.x = 10
        d.y = 20
        d.extra = 30
        self.assertEqual(d.x, 10)
        self.assertEqual(d.y, 20)
        self.assertEqual(d.extra, 30)
        del d.extra
        self.assertIsNone(d.extra)


@requires_limited_api
class TestHeapTypeRelative(unittest.TestCase):
    """Test API for extending opaque types (PEP 697)"""

    @requires_limited_api
    def test_heaptype_relative_sizes(self):
        # Test subclassing using "relative" basicsize, see PEP 697
        def check(extra_base_size, extra_size):
            Base, Sub, instance, data_ptr, data_offset, data_size = (
                _testcapi.make_sized_heaptypes(
                    extra_base_size, -extra_size))

            # no alignment shenanigans when inheriting directly
            if extra_size == 0:
                self.assertEqual(Base.__basicsize__, Sub.__basicsize__)
                self.assertEqual(data_size, 0)

            else:
                # The following offsets should be in increasing order:
                offsets = [
                    (0, 'start of object'),
                    (Base.__basicsize__, 'end of base data'),
                    (data_offset, 'subclass data'),
                    (data_offset + extra_size, 'end of requested subcls data'),
                    (data_offset + data_size, 'end of reserved subcls data'),
                    (Sub.__basicsize__, 'end of object'),
                ]
                ordered_offsets = sorted(offsets, key=operator.itemgetter(0))
                self.assertEqual(
                    offsets, ordered_offsets,
                    msg=f'Offsets not in expected order, got: {ordered_offsets}')

                # end of reserved subcls data == end of object
                self.assertEqual(Sub.__basicsize__, data_offset + data_size)

                # we don't reserve (requested + alignment) or more data
                self.assertLess(data_size - extra_size,
                                _testcapi.ALIGNOF_MAX_ALIGN_T)

            # The offsets/sizes we calculated should be aligned.
            self.assertEqual(data_offset % _testcapi.ALIGNOF_MAX_ALIGN_T, 0)
            self.assertEqual(data_size % _testcapi.ALIGNOF_MAX_ALIGN_T, 0)

        sizes = sorted({0, 1, 2, 3, 4, 7, 8, 123,
                        object.__basicsize__,
                        object.__basicsize__-1,
                        object.__basicsize__+1})
        for extra_base_size in sizes:
            for extra_size in sizes:
                args = dict(extra_base_size=extra_base_size,
                            extra_size=extra_size)
                with self.subTest(**args):
                    check(**args)

    def test_HeapCCollection(self):
        """Make sure HeapCCollection works properly by itself"""
        collection = _testcapi.HeapCCollection(1, 2, 3)
        self.assertEqual(list(collection), [1, 2, 3])

    def test_heaptype_inherit_itemsize(self):
        """Test HeapCCollection subclasses work properly"""
        sizes = sorted({0, 1, 2, 3, 4, 7, 8, 123,
                        object.__basicsize__,
                        object.__basicsize__-1,
                        object.__basicsize__+1})
        for extra_size in sizes:
            with self.subTest(extra_size=extra_size):
                Sub = _testcapi.subclass_var_heaptype(
                    _testcapi.HeapCCollection, -extra_size, 0, 0)
                collection = Sub(1, 2, 3)
                collection.set_data_to_3s()

                self.assertEqual(list(collection), [1, 2, 3])
                mem = collection.get_data()
                self.assertGreaterEqual(len(mem), extra_size)
                self.assertTrue(set(mem) <= {3}, f'got {mem!r}')

    def test_heaptype_invalid_inheritance(self):
        with self.assertRaises(SystemError,
                               msg="Cannot extend variable-size class without "
                               + "Py_TPFLAGS_ITEMS_AT_END"):
            _testcapi.subclass_heaptype(int, -8, 0)

    def test_heaptype_relative_members(self):
        """Test HeapCCollection subclasses work properly"""
        sizes = sorted({0, 1, 2, 3, 4, 7, 8, 123,
                        object.__basicsize__,
                        object.__basicsize__-1,
                        object.__basicsize__+1})
        for extra_base_size in sizes:
            for extra_size in sizes:
                for offset in sizes:
                    with self.subTest(extra_base_size=extra_base_size, extra_size=extra_size, offset=offset):
                        if offset < extra_size:
                            Sub = _testcapi.make_heaptype_with_member(
                                extra_base_size, -extra_size, offset, True)
                            Base = Sub.mro()[1]
                            instance = Sub()
                            self.assertEqual(instance.memb, instance.get_memb())
                            instance.set_memb(13)
                            self.assertEqual(instance.memb, instance.get_memb())
                            self.assertEqual(instance.get_memb(), 13)
                            instance.memb = 14
                            self.assertEqual(instance.memb, instance.get_memb())
                            self.assertEqual(instance.get_memb(), 14)
                            self.assertGreaterEqual(instance.get_memb_offset(), Base.__basicsize__)
                            self.assertLess(instance.get_memb_offset(), Sub.__basicsize__)
                            with self.assertRaises(SystemError):
                                instance.get_memb_relative()
                            with self.assertRaises(SystemError):
                                instance.set_memb_relative(0)
                        else:
                            with self.assertRaises(SystemError):
                                Sub = _testcapi.make_heaptype_with_member(
                                    extra_base_size, -extra_size, offset, True)
                        with self.assertRaises(SystemError):
                            Sub = _testcapi.make_heaptype_with_member(
                                extra_base_size, extra_size, offset, True)
                with self.subTest(extra_base_size=extra_base_size, extra_size=extra_size):
                    with self.assertRaises(SystemError):
                        Sub = _testcapi.make_heaptype_with_member(
                            extra_base_size, -extra_size, -1, True)

    def test_heaptype_relative_members_errors(self):
        with self.assertRaisesRegex(
                SystemError,
                r"With Py_RELATIVE_OFFSET, basicsize must be negative"):
            _testcapi.make_heaptype_with_member(0, 1234, 0, True)
        with self.assertRaisesRegex(
                SystemError, r"Member offset out of range \(0\.\.-basicsize\)"):
            _testcapi.make_heaptype_with_member(0, -8, 1234, True)
        with self.assertRaisesRegex(
                SystemError, r"Member offset out of range \(0\.\.-basicsize\)"):
            _testcapi.make_heaptype_with_member(0, -8, -1, True)

        Sub = _testcapi.make_heaptype_with_member(0, -8, 0, True)
        instance = Sub()
        with self.assertRaisesRegex(
                SystemError, r"PyMember_GetOne used with Py_RELATIVE_OFFSET"):
            instance.get_memb_relative()
        with self.assertRaisesRegex(
                SystemError, r"PyMember_SetOne used with Py_RELATIVE_OFFSET"):
            instance.set_memb_relative(0)

    def test_pyobject_getitemdata_error(self):
        """Test PyObject_GetItemData fails on unsupported types"""
        with self.assertRaises(TypeError):
            # None is not variable-length
            _testcapi.pyobject_getitemdata(None)
        with self.assertRaises(TypeError):
            # int is variable-length, but doesn't have the
            # Py_TPFLAGS_ITEMS_AT_END layout (and flag)
            _testcapi.pyobject_getitemdata(0)


class TestPendingCalls(unittest.TestCase):

    def pendingcalls_submit(self, l, n):
        def callback():
            #this function can be interrupted by thread switching so let's
            #use an atomic operation
            l.append(None)

        for i in range(n):
            time.sleep(random.random()*0.02) #0.01 secs on average
            #try submitting callback until successful.
            #rely on regular interrupt to flush queue if we are
            #unsuccessful.
            while True:
                if _testcapi._pending_threadfunc(callback):
                    break

    def pendingcalls_wait(self, l, n, context = None):
        #now, stick around until l[0] has grown to 10
        count = 0
        while len(l) != n:
            #this busy loop is where we expect to be interrupted to
            #run our callbacks.  Note that callbacks are only run on the
            #main thread
            if False and support.verbose:
                print("(%i)"%(len(l),),)
            for i in range(1000):
                a = i*i
            if context and not context.event.is_set():
                continue
            count += 1
            self.assertTrue(count < 10000,
                "timeout waiting for %i callbacks, got %i"%(n, len(l)))
        if False and support.verbose:
            print("(%i)"%(len(l),))

    @threading_helper.requires_working_threading()
    def test_pendingcalls_threaded(self):

        #do every callback on a separate thread
        n = 32 #total callbacks
        threads = []
        class foo(object):pass
        context = foo()
        context.l = []
        context.n = 2 #submits per thread
        context.nThreads = n // context.n
        context.nFinished = 0
        context.lock = threading.Lock()
        context.event = threading.Event()

        threads = [threading.Thread(target=self.pendingcalls_thread,
                                    args=(context,))
                   for i in range(context.nThreads)]
        with threading_helper.start_threads(threads):
            self.pendingcalls_wait(context.l, n, context)

    def pendingcalls_thread(self, context):
        try:
            self.pendingcalls_submit(context.l, context.n)
        finally:
            with context.lock:
                context.nFinished += 1
                nFinished = context.nFinished
                if False and support.verbose:
                    print("finished threads: ", nFinished)
            if nFinished == context.nThreads:
                context.event.set()

    def test_pendingcalls_non_threaded(self):
        #again, just using the main thread, likely they will all be dispatched at
        #once.  It is ok to ask for too many, because we loop until we find a slot.
        #the loop can be interrupted to dispatch.
        #there are only 32 dispatch slots, so we go for twice that!
        l = []
        n = 64
        self.pendingcalls_submit(l, n)
        self.pendingcalls_wait(l, n)

    def test_gen_get_code(self):
        def genf(): yield
        gen = genf()
        self.assertEqual(_testcapi.gen_get_code(gen), gen.gi_code)


class SubinterpreterTest(unittest.TestCase):

    @unittest.skipUnless(hasattr(os, "pipe"), "requires os.pipe()")
    def test_subinterps(self):
        import builtins
        r, w = os.pipe()
        code = """if 1:
            import sys, builtins, pickle
            with open({:d}, "wb") as f:
                pickle.dump(id(sys.modules), f)
                pickle.dump(id(builtins), f)
            """.format(w)
        with open(r, "rb") as f:
            ret = support.run_in_subinterp(code)
            self.assertEqual(ret, 0)
            self.assertNotEqual(pickle.load(f), id(sys.modules))
            self.assertNotEqual(pickle.load(f), id(builtins))

    @unittest.skipUnless(hasattr(os, "pipe"), "requires os.pipe()")
    def test_subinterps_recent_language_features(self):
        r, w = os.pipe()
        code = """if 1:
            import pickle
            with open({:d}, "wb") as f:

                @(lambda x:x)  # Py 3.9
                def noop(x): return x

                a = (b := f'1{{2}}3') + noop('x')  # Py 3.8 (:=) / 3.6 (f'')

                async def foo(arg): return await arg  # Py 3.5

                pickle.dump(dict(a=a, b=b), f)
            """.format(w)

        with open(r, "rb") as f:
            ret = support.run_in_subinterp(code)
            self.assertEqual(ret, 0)
            self.assertEqual(pickle.load(f), {'a': '123x', 'b': '123'})

    def test_py_config_isoloated_per_interpreter(self):
        # A config change in one interpreter must not leak to out to others.
        #
        # This test could verify ANY config value, it just happens to have been
        # written around the time of int_max_str_digits. Refactoring is okay.
        code = """if 1:
        import sys, _testinternalcapi

        # Any config value would do, this happens to be the one being
        # double checked at the time this test was written.
        config = _testinternalcapi.get_config()
        config['int_max_str_digits'] = 55555
        _testinternalcapi.set_config(config)
        sub_value = _testinternalcapi.get_config()['int_max_str_digits']
        assert sub_value == 55555, sub_value
        """
        before_config = _testinternalcapi.get_config()
        assert before_config['int_max_str_digits'] != 55555
        self.assertEqual(support.run_in_subinterp(code), 0,
                         'subinterp code failure, check stderr.')
        after_config = _testinternalcapi.get_config()
        self.assertIsNot(
                before_config, after_config,
                "Expected get_config() to return a new dict on each call")
        self.assertEqual(before_config, after_config,
                         "CAUTION: Tests executed after this may be "
                         "running under an altered config.")
        # try:...finally: calling set_config(before_config) not done
        # as that results in sys.argv, sys.path, and sys.warnoptions
        # "being modified by test_capi" per test.regrtest.  So if this
        # test fails, assume that the environment in this process may
        # be altered and suspect.

    @unittest.skipUnless(hasattr(os, "pipe"), "requires os.pipe()")
    def test_configured_settings(self):
        """
        The config with which an interpreter is created corresponds
        1-to-1 with the new interpreter's settings.  This test verifies
        that they match.
        """
        import json

        OBMALLOC = 1<<5
        EXTENSIONS = 1<<8
        THREADS = 1<<10
        DAEMON_THREADS = 1<<11
        FORK = 1<<15
        EXEC = 1<<16
        ALL_FLAGS = (OBMALLOC | FORK | EXEC | THREADS | DAEMON_THREADS
                     | EXTENSIONS);

        features = [
            'obmalloc',
            'fork',
            'exec',
            'threads',
            'daemon_threads',
            'extensions',
            'own_gil',
        ]
        kwlist = [f'allow_{n}' for n in features]
        kwlist[0] = 'use_main_obmalloc'
        kwlist[-2] = 'check_multi_interp_extensions'
        kwlist[-1] = 'own_gil'

        # expected to work
        for config, expected in {
            (True, True, True, True, True, True, True):
                (ALL_FLAGS, True),
            (True, False, False, False, False, False, False):
                (OBMALLOC, False),
            (False, False, False, True, False, True, False):
                (THREADS | EXTENSIONS, False),
        }.items():
            kwargs = dict(zip(kwlist, config))
            exp_flags, exp_gil = expected
            expected = {
                'feature_flags': exp_flags,
                'own_gil': exp_gil,
            }
            with self.subTest(config):
                r, w = os.pipe()
                script = textwrap.dedent(f'''
                    import _testinternalcapi, json, os
                    settings = _testinternalcapi.get_interp_settings()
                    with os.fdopen({w}, "w") as stdin:
                        json.dump(settings, stdin)
                    ''')
                with os.fdopen(r) as stdout:
                    ret = support.run_in_subinterp_with_config(script, **kwargs)
                    self.assertEqual(ret, 0)
                    out = stdout.read()
                settings = json.loads(out)

                self.assertEqual(settings, expected)

        # expected to fail
        for config in [
            (False, False, False, False, False, False, False),
        ]:
            kwargs = dict(zip(kwlist, config))
            with self.subTest(config):
                script = textwrap.dedent(f'''
                    import _testinternalcapi
                    _testinternalcapi.get_interp_settings()
                    raise NotImplementedError('unreachable')
                    ''')
                with self.assertRaises(RuntimeError):
                    support.run_in_subinterp_with_config(script, **kwargs)

    @unittest.skipIf(_testsinglephase is None, "test requires _testsinglephase module")
    @unittest.skipUnless(hasattr(os, "pipe"), "requires os.pipe()")
    def test_overridden_setting_extensions_subinterp_check(self):
        """
        PyInterpreterConfig.check_multi_interp_extensions can be overridden
        with PyInterpreterState.override_multi_interp_extensions_check.
        This verifies that the override works but does not modify
        the underlying setting.
        """
        import json

        OBMALLOC = 1<<5
        EXTENSIONS = 1<<8
        THREADS = 1<<10
        DAEMON_THREADS = 1<<11
        FORK = 1<<15
        EXEC = 1<<16
        BASE_FLAGS = OBMALLOC | FORK | EXEC | THREADS | DAEMON_THREADS
        base_kwargs = {
            'use_main_obmalloc': True,
            'allow_fork': True,
            'allow_exec': True,
            'allow_threads': True,
            'allow_daemon_threads': True,
            'own_gil': False,
        }

        def check(enabled, override):
            kwargs = dict(
                base_kwargs,
                check_multi_interp_extensions=enabled,
            )
            flags = BASE_FLAGS | EXTENSIONS if enabled else BASE_FLAGS
            settings = {
                'feature_flags': flags,
                'own_gil': False,
            }

            expected = {
                'requested': override,
                'override__initial': 0,
                'override_after': override,
                'override_restored': 0,
                # The override should not affect the config or settings.
                'settings__initial': settings,
                'settings_after': settings,
                'settings_restored': settings,
                # These are the most likely values to be wrong.
                'allowed__initial': not enabled,
                'allowed_after': not ((override > 0) if override else enabled),
                'allowed_restored': not enabled,
            }

            r, w = os.pipe()
            script = textwrap.dedent(f'''
                from test.test_capi.check_config import run_singlephase_check
                run_singlephase_check({override}, {w})
                ''')
            with os.fdopen(r) as stdout:
                ret = support.run_in_subinterp_with_config(script, **kwargs)
                self.assertEqual(ret, 0)
                out = stdout.read()
            results = json.loads(out)

            self.assertEqual(results, expected)

        self.maxDiff = None

        # setting: check disabled
        with self.subTest('config: check disabled; override: disabled'):
            check(False, -1)
        with self.subTest('config: check disabled; override: use config'):
            check(False, 0)
        with self.subTest('config: check disabled; override: enabled'):
            check(False, 1)

        # setting: check enabled
        with self.subTest('config: check enabled; override: disabled'):
            check(True, -1)
        with self.subTest('config: check enabled; override: use config'):
            check(True, 0)
        with self.subTest('config: check enabled; override: enabled'):
            check(True, 1)

    def test_mutate_exception(self):
        """
        Exceptions saved in global module state get shared between
        individual module instances. This test checks whether or not
        a change in one interpreter's module gets reflected into the
        other ones.
        """
        import binascii

        support.run_in_subinterp("import binascii; binascii.Error.foobar = 'foobar'")

        self.assertFalse(hasattr(binascii.Error, "foobar"))

    @unittest.skipIf(_testmultiphase is None, "test requires _testmultiphase module")
    def test_module_state_shared_in_global(self):
        """
        bpo-44050: Extension module state should be shared between interpreters
        when it doesn't support sub-interpreters.
        """
        r, w = os.pipe()
        self.addCleanup(os.close, r)
        self.addCleanup(os.close, w)

        script = textwrap.dedent(f"""
            import importlib.machinery
            import importlib.util
            import os

            fullname = '_test_module_state_shared'
            origin = importlib.util.find_spec('_testmultiphase').origin
            loader = importlib.machinery.ExtensionFileLoader(fullname, origin)
            spec = importlib.util.spec_from_loader(fullname, loader)
            module = importlib.util.module_from_spec(spec)
            attr_id = str(id(module.Error)).encode()

            os.write({w}, attr_id)
            """)
        exec(script)
        main_attr_id = os.read(r, 100)

        ret = support.run_in_subinterp(script)
        self.assertEqual(ret, 0)
        subinterp_attr_id = os.read(r, 100)
        self.assertEqual(main_attr_id, subinterp_attr_id)


class TestThreadState(unittest.TestCase):

    @threading_helper.reap_threads
    @threading_helper.requires_working_threading()
    def test_thread_state(self):
        # some extra thread-state tests driven via _testcapi
        def target():
            idents = []

            def callback():
                idents.append(threading.get_ident())

            _testcapi._test_thread_state(callback)
            a = b = callback
            time.sleep(1)
            # Check our main thread is in the list exactly 3 times.
            self.assertEqual(idents.count(threading.get_ident()), 3,
                             "Couldn't find main thread correctly in the list")

        target()
        t = threading.Thread(target=target)
        t.start()
        t.join()

    @threading_helper.reap_threads
    @threading_helper.requires_working_threading()
    def test_gilstate_ensure_no_deadlock(self):
        # See https://github.com/python/cpython/issues/96071
        code = textwrap.dedent("""
            import _testcapi

            def callback():
                print('callback called')

            _testcapi._test_thread_state(callback)
            """)
        ret = assert_python_ok('-X', 'tracemalloc', '-c', code)
        self.assertIn(b'callback called', ret.out)

    def test_gilstate_matches_current(self):
        _testcapi.test_current_tstate_matches()


class Test_testcapi(unittest.TestCase):
    locals().update((name, getattr(_testcapi, name))
                    for name in dir(_testcapi)
                    if name.startswith('test_') and not name.endswith('_code'))

    # Suppress warning from PyUnicode_FromUnicode().
    @warnings_helper.ignore_warnings(category=DeprecationWarning)
    def test_widechar(self):
        _testcapi.test_widechar()

    def test_version_api_data(self):
        self.assertEqual(_testcapi.Py_Version, sys.hexversion)


class Test_testinternalcapi(unittest.TestCase):
    locals().update((name, getattr(_testinternalcapi, name))
                    for name in dir(_testinternalcapi)
                    if name.startswith('test_'))


@unittest.skipIf(_testmultiphase is None, "test requires _testmultiphase module")
class Test_ModuleStateAccess(unittest.TestCase):
    """Test access to module start (PEP 573)"""

    # The C part of the tests lives in _testmultiphase, in a module called
    # _testmultiphase_meth_state_access.
    # This module has multi-phase initialization, unlike _testcapi.

    def setUp(self):
        fullname = '_testmultiphase_meth_state_access'  # XXX
        origin = importlib.util.find_spec('_testmultiphase').origin
        loader = importlib.machinery.ExtensionFileLoader(fullname, origin)
        spec = importlib.util.spec_from_loader(fullname, loader)
        module = importlib.util.module_from_spec(spec)
        loader.exec_module(module)
        self.module = module

    def test_subclass_get_module(self):
        """PyType_GetModule for defining_class"""
        class StateAccessType_Subclass(self.module.StateAccessType):
            pass

        instance = StateAccessType_Subclass()
        self.assertIs(instance.get_defining_module(), self.module)

    def test_subclass_get_module_with_super(self):
        class StateAccessType_Subclass(self.module.StateAccessType):
            def get_defining_module(self):
                return super().get_defining_module()

        instance = StateAccessType_Subclass()
        self.assertIs(instance.get_defining_module(), self.module)

    def test_state_access(self):
        """Checks methods defined with and without argument clinic

        This tests a no-arg method (get_count) and a method with
        both a positional and keyword argument.
        """

        a = self.module.StateAccessType()
        b = self.module.StateAccessType()

        methods = {
            'clinic': a.increment_count_clinic,
            'noclinic': a.increment_count_noclinic,
        }

        for name, increment_count in methods.items():
            with self.subTest(name):
                self.assertEqual(a.get_count(), b.get_count())
                self.assertEqual(a.get_count(), 0)

                increment_count()
                self.assertEqual(a.get_count(), b.get_count())
                self.assertEqual(a.get_count(), 1)

                increment_count(3)
                self.assertEqual(a.get_count(), b.get_count())
                self.assertEqual(a.get_count(), 4)

                increment_count(-2, twice=True)
                self.assertEqual(a.get_count(), b.get_count())
                self.assertEqual(a.get_count(), 0)

                with self.assertRaises(TypeError):
                    increment_count(thrice=3)

                with self.assertRaises(TypeError):
                    increment_count(1, 2, 3)

    def test_get_module_bad_def(self):
        # PyType_GetModuleByDef fails gracefully if it doesn't
        # find what it's looking for.
        # see bpo-46433
        instance = self.module.StateAccessType()
        with self.assertRaises(TypeError):
            instance.getmodulebydef_bad_def()

    def test_get_module_static_in_mro(self):
        # Here, the class PyType_GetModuleByDef is looking for
        # appears in the MRO after a static type (Exception).
        # see bpo-46433
        class Subclass(BaseException, self.module.StateAccessType):
            pass
        self.assertIs(Subclass().get_defining_module(), self.module)


SUFFICIENT_TO_DEOPT_AND_SPECIALIZE = 100

class Test_Pep523API(unittest.TestCase):

    def do_test(self, func, names):
        calls = []
        start = SUFFICIENT_TO_DEOPT_AND_SPECIALIZE
        count = start + SUFFICIENT_TO_DEOPT_AND_SPECIALIZE
        try:
            for i in range(count):
                if i == start:
                    _testinternalcapi.set_eval_frame_record(calls)
                func()
        finally:
            _testinternalcapi.set_eval_frame_default()
        self.assertEqual(len(calls), SUFFICIENT_TO_DEOPT_AND_SPECIALIZE * len(names))
        for name, call in zip(itertools.cycle(names), calls):
            self.assertEqual(name, call)

    def test_inlined_binary_subscr(self):
        class C:
            def __getitem__(self, other):
                return None
        def func():
            C()[42]
        names = ["func", "__getitem__"]
        self.do_test(func, names)

    def test_inlined_call(self):
        def inner(x=42):
            pass
        def func():
            inner()
            inner(42)
        names = ["func", "inner", "inner"]
        self.do_test(func, names)

    def test_inlined_call_function_ex(self):
        def inner(x):
            pass
        def func():
            inner(*[42])
        names = ["func", "inner"]
        self.do_test(func, names)

    def test_inlined_for_iter(self):
        def gen():
            yield 42
        def func():
            for _ in gen():
                pass
        names = ["func", "gen", "gen", "gen"]
        self.do_test(func, names)

    def test_inlined_load_attr(self):
        class C:
            @property
            def a(self):
                return 42
        class D:
            def __getattribute__(self, name):
                return 42
        def func():
            C().a
            D().a
        names = ["func", "a", "__getattribute__"]
        self.do_test(func, names)

    def test_inlined_send(self):
        def inner():
            yield 42
        def outer():
            yield from inner()
        def func():
            list(outer())
        names = ["func", "outer", "outer", "inner", "inner", "outer", "inner"]
        self.do_test(func, names)


if __name__ == "__main__":
    unittest.main()
