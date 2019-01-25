import ast
import unittest


funcdef = """\
def foo():
    # type: () -> int
    pass

def bar():  # type: () -> None
    pass
"""

asyncdef = """\
async def foo():
    # type: () -> int
    return await bar()

async def bar():  # type: () -> int
    return await bar()
"""

forstmt = """\
for a in []:  # type: int
    pass
"""


withstmt = """\
with context():  # type: int
    pass
"""

vardecl = """\
a = 0  # type: int
a  # type: int
"""

ignores = """\
def foo():
    pass  # type: ignore

def bar():
    x = 1  # type: ignore
"""

# Test for long-form type-comments in arguments.  A test function
# named 'fabvk' would have two positional args, a and b, plus a
# var-arg *v, plus a kw-arg **k.  It is verified in test_longargs()
# that it has exactly these arguments, no more, no fewer.
longargs = """\
def fa(
    a = 1,  # type: A
):
    pass

def fa(
    a = 1  # type: A
):
    pass

def fab(
    a,  # type: A
    b,  # type: B
):
    pass

def fab(
    a,  # type: A
    b  # type: B
):
    pass

def fv(
    *v,  # type: V
):
    pass

def fv(
    *v  # type: V
):
    pass

def fk(
    **k,  # type: K
):
    pass

def fk(
    **k  # type: K
):
    pass

def fvk(
    *v,  # type: V
    **k,  # type: K
):
    pass

def fvk(
    *v,  # type: V
    **k  # type: K
):
    pass

def fav(
    a,  # type: A
    *v,  # type: V
):
    pass

def fav(
    a,  # type: A
    *v  # type: V
):
    pass

def fak(
    a,  # type: A
    **k,  # type: K
):
    pass

def fak(
    a,  # type: A
    **k  # type: K
):
    pass

def favk(
    a,  # type: A
    *v,  # type: V
    **k,  # type: K
):
    pass

def favk(
    a,  # type: A
    *v,  # type: V
    **k  # type: K
):
    pass
"""


class TypeCommentTests(unittest.TestCase):

    def parse(self, source):
        return ast.parse(source, type_comments=True)

    def test_funcdef(self):
        tree = self.parse(funcdef)
        self.assertEqual(tree.body[0].type_comment, "() -> int")
        self.assertEqual(tree.body[1].type_comment, "() -> None")

    def test_asyncdef(self):
        tree = self.parse(asyncdef)
        self.assertEqual(tree.body[0].type_comment, "() -> int")
        self.assertEqual(tree.body[1].type_comment, "() -> int")

    def test_forstmt(self):
        tree = self.parse(forstmt)
        self.assertEqual(tree.body[0].type_comment, "int")

    def test_withstmt(self):
        tree = self.parse(withstmt)
        self.assertEqual(tree.body[0].type_comment, "int")

    def test_vardecl(self):
        tree = self.parse(vardecl)
        self.assertEqual(tree.body[0].type_comment, "int")
        # Curious fact: an expression can have a type comment but it
        # is lost in the AST, so we have this in the example source
        # code but don't test it here.

    def test_ignores(self):
        tree = self.parse(ignores)
        self.assertEqual([ti.lineno for ti in tree.type_ignores], [2, 5])

    def test_longargs(self):
        tree = self.parse(longargs)
        for t in tree.body:
            # The expected args are encoded in the function name
            todo = set(t.name[1:])
            self.assertEqual(len(t.args.args),
                             len(todo) - bool(t.args.vararg) - bool(t.args.kwarg))
            self.assertTrue(t.name.startswith('f'), t.name)
            for c in t.name[1:]:
                todo.remove(c)
                if c == 'v':
                    arg = t.args.vararg
                elif c == 'k':
                    arg = t.args.kwarg
                else:
                    assert 0 <= ord(c) - ord('a') < len(t.args.args)
                    arg = t.args.args[ord(c) - ord('a')]
                self.assertEqual(arg.arg, c)  # That's the argument name
                self.assertEqual(arg.type_comment, arg.arg.upper())
            assert not todo


if __name__ == '__main__':
    unittest.main()
