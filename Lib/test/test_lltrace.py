import sys
import textwrap
import unittest

from test.support import os_helper
from test.support.script_helper import assert_python_ok

def example():
    x = "this is"
    y = "an example"
    print(x, y)

Py_DEBUG = hasattr(sys, 'gettotalrefcount')

@unittest.skipUnless(Py_DEBUG, "lltrace requires Py_DEBUG")
class TestLLTrace(unittest.TestCase):

    def run_code(self, code):
        code = textwrap.dedent(code).strip()
        with open(os_helper.TESTFN, 'w', encoding='utf-8') as fd:
            self.addCleanup(os_helper.unlink, os_helper.TESTFN)
            fd.write(code)
        status, stdout, stderr = assert_python_ok(os_helper.TESTFN)
        self.assertEqual(stderr, b"")
        self.assertEqual(status, 0)
        return stdout

    def test_lltrace(self):
        stdout = self.run_code("""
            def dont_trace_1():
                a = "a"
                a = 10 * a
            def trace_me():
                for i in range(3):
                    +i
            def dont_trace_2():
                x = 42
                y = -x
            dont_trace_1()
            __ltrace__ = 1
            trace_me()
            del __ltrace__
            dont_trace_2()
        """)
        self.assertIn(b"GET_ITER", stdout)
        self.assertIn(b"FOR_ITER", stdout)
        self.assertIn(b"UNARY_POSITIVE", stdout)
        self.assertIn(b"POP_TOP", stdout)
        self.assertNotIn(b"BINARY_OP", stdout)
        self.assertNotIn(b"UNARY_NEGATIVE", stdout)

        self.assertIn(b"'trace_me' in module '__main__'", stdout)
        self.assertNotIn(b"dont_trace_1", stdout)
        self.assertNotIn(b"'dont_trace_2' in module", stdout)

    def test_lltrace_different_module(self):
        stdout = self.run_code("""
            from test import test_lltrace
            test_lltrace.__ltrace__ = 1
            test_lltrace.example()
        """)
        self.assertIn(b"'example' in module 'test.test_lltrace'", stdout)
        self.assertIn(b'this is an example', stdout)

    def test_lltrace_does_not_crash_on_subscript_operator(self):
        # If this test fails, it will reproduce a crash reported as
        # bpo-34113. The crash happened at the command line console of
        # debug Python builds with __ltrace__ enabled (only possible in console),
        # when the internal Python stack was negatively adjusted
        stdout = self.run_code("""
            import code

            console = code.InteractiveConsole()
            console.push('__ltrace__ = 1')
            console.push('a = [1, 2, 3]')
            console.push('a[0] = 1')
            print('unreachable if bug exists')
        """)
        self.assertIn(b"unreachable if bug exists", stdout)

if __name__ == "__main__":
    unittest.main()
