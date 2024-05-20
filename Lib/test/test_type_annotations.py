import textwrap
import types
import unittest
from test.support import run_code

class TypeAnnotationTests(unittest.TestCase):

    def test_lazy_create_annotations(self):
        # type objects lazy create their __annotations__ dict on demand.
        # the annotations dict is stored in type.__dict__.
        # a freshly created type shouldn't have an annotations dict yet.
        foo = type("Foo", (), {})
        for i in range(3):
            self.assertFalse("__annotations__" in foo.__dict__)
            d = foo.__annotations__
            self.assertTrue("__annotations__" in foo.__dict__)
            self.assertEqual(foo.__annotations__, d)
            self.assertEqual(foo.__dict__['__annotations__'], d)
            del foo.__annotations__

    def test_setting_annotations(self):
        foo = type("Foo", (), {})
        for i in range(3):
            self.assertFalse("__annotations__" in foo.__dict__)
            d = {'a': int}
            foo.__annotations__ = d
            self.assertTrue("__annotations__" in foo.__dict__)
            self.assertEqual(foo.__annotations__, d)
            self.assertEqual(foo.__dict__['__annotations__'], d)
            del foo.__annotations__

    def test_annotations_getset_raises(self):
        # builtin types don't have __annotations__ (yet!)
        with self.assertRaises(AttributeError):
            print(float.__annotations__)
        with self.assertRaises(TypeError):
            float.__annotations__ = {}
        with self.assertRaises(TypeError):
            del float.__annotations__

        # double delete
        foo = type("Foo", (), {})
        foo.__annotations__ = {}
        del foo.__annotations__
        with self.assertRaises(AttributeError):
            del foo.__annotations__

    def test_annotations_are_created_correctly(self):
        class C:
            a:int=3
            b:str=4
        self.assertTrue("__annotations__" in C.__dict__)
        del C.__annotations__
        self.assertFalse("__annotations__" in C.__dict__)

    def test_descriptor_still_works(self):
        class C:
            def __init__(self, name=None, bases=None, d=None):
                self.my_annotations = None

            @property
            def __annotations__(self):
                if not hasattr(self, 'my_annotations'):
                    self.my_annotations = {}
                if not isinstance(self.my_annotations, dict):
                    self.my_annotations = {}
                return self.my_annotations

            @__annotations__.setter
            def __annotations__(self, value):
                if not isinstance(value, dict):
                    raise ValueError("can only set __annotations__ to a dict")
                self.my_annotations = value

            @__annotations__.deleter
            def __annotations__(self):
                if getattr(self, 'my_annotations', False) is None:
                    raise AttributeError('__annotations__')
                self.my_annotations = None

        c = C()
        self.assertEqual(c.__annotations__, {})
        d = {'a':'int'}
        c.__annotations__ = d
        self.assertEqual(c.__annotations__, d)
        with self.assertRaises(ValueError):
            c.__annotations__ = 123
        del c.__annotations__
        with self.assertRaises(AttributeError):
            del c.__annotations__
        self.assertEqual(c.__annotations__, {})


        class D(metaclass=C):
            pass

        self.assertEqual(D.__annotations__, {})
        d = {'a':'int'}
        D.__annotations__ = d
        self.assertEqual(D.__annotations__, d)
        with self.assertRaises(ValueError):
            D.__annotations__ = 123
        del D.__annotations__
        with self.assertRaises(AttributeError):
            del D.__annotations__
        self.assertEqual(D.__annotations__, {})


class TestSetupAnnotations(unittest.TestCase):
    def check(self, code: str):
        code = textwrap.dedent(code)
        for scope in ("module", "class"):
            with self.subTest(scope=scope):
                if scope == "class":
                    code = f"class C:\n{textwrap.indent(code, '    ')}"
                ns = run_code(code)
                if scope == "class":
                    annotations = ns["C"].__annotations__
                else:
                    annotations = ns["__annotations__"]
                self.assertEqual(annotations, {"x": int})

    def test_top_level(self):
        self.check("x: int = 1")

    def test_blocks(self):
        self.check("if True:\n    x: int = 1")
        self.check("""
            while True:
                x: int = 1
                break
        """)
        self.check("""
            while False:
                pass
            else:
                x: int = 1
        """)
        self.check("""
            for i in range(1):
                x: int = 1
        """)
        self.check("""
            for i in range(1):
                pass
            else:
                x: int = 1
        """)

    def test_try(self):
        self.check("""
            try:
                x: int = 1
            except:
                pass
        """)
        self.check("""
            try:
                pass
            except:
                pass
            else:
                x: int = 1
        """)
        self.check("""
            try:
                pass
            except:
                pass
            finally:
                x: int = 1
        """)
        self.check("""
            try:
                1/0
            except:
                x: int = 1
        """)

    def test_try_star(self):
        self.check("""
            try:
                x: int = 1
            except* Exception:
                pass
        """)
        self.check("""
            try:
                pass
            except* Exception:
                pass
            else:
                x: int = 1
        """)
        self.check("""
            try:
                pass
            except* Exception:
                pass
            finally:
                x: int = 1
        """)
        self.check("""
            try:
                1/0
            except* Exception:
                x: int = 1
        """)

    def test_match(self):
        self.check("""
            match 0:
                case 0:
                    x: int = 1
        """)


class AnnotateTests(unittest.TestCase):
    """See PEP 649."""
    def test_manual_annotate(self):
        def f():
            pass
        mod = types.ModuleType("mod")
        class X:
            pass

        for obj in (f, mod, X):
            with self.subTest(obj=obj):
                self.check_annotations(obj)

    def check_annotations(self, f):
        self.assertEqual(f.__annotations__, {})
        self.assertIs(f.__annotate__, None)

        with self.assertRaisesRegex(TypeError, "__annotate__ must be callable or None"):
            f.__annotate__ = 42
        f.__annotate__ = lambda: 42
        with self.assertRaisesRegex(TypeError, r"takes 0 positional arguments but 1 was given"):
            print(f.__annotations__)

        f.__annotate__ = lambda x: 42
        with self.assertRaisesRegex(TypeError, r"__annotate__ returned a non-dict"):
            print(f.__annotations__)

        f.__annotate__ = lambda x: {"x": x}
        self.assertEqual(f.__annotations__, {"x": 1})

        # Setting annotate to None does not invalidate the cached __annotations__
        f.__annotate__ = None
        self.assertEqual(f.__annotations__, {"x": 1})

        # But setting it to a new callable does
        f.__annotate__ = lambda x: {"y": x}
        self.assertEqual(f.__annotations__, {"y": 1})

        # Setting f.__annotations__ also clears __annotate__
        f.__annotations__ = {"z": 43}
        self.assertIs(f.__annotate__, None)
