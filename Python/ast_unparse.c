#include <stdbool.h>
#include "Python.h"
#include "Python-ast.h"

/* Forward declaration for recursion via helper functions. */
static int
append_ast_expr(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens,
    bool omit_string_brackets);

static int
append_charp(_PyUnicodeWriter *writer, const char *charp)
{
        return _PyUnicodeWriter_WriteASCIIString(writer, charp, -1);
}

static int
append_repr(_PyUnicodeWriter *writer, PyObject *obj)
{
    int ret;
    PyObject *repr;
    repr = PyObject_Repr(obj);
    if (!repr) {
        return -1;
    }
    ret = _PyUnicodeWriter_WriteStr(writer, repr);
    Py_DECREF(repr);
    return ret;
}

static int
append_ast_boolop(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    Py_ssize_t i, value_count;
    asdl_seq *values;

    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    values = e->v.BoolOp.values;
    value_count = asdl_seq_LEN(values) - 1;
    assert(value_count >= 0);

    if (-1 == append_ast_expr(writer,
                              (expr_ty)asdl_seq_GET(values, 0),
                              false,
                              false)) {
        return -1;
    }

    const char *op = (e->v.BoolOp.op == And) ? " and " : " or ";
    for (i = 1; i <= value_count; ++i) {
        if (-1 == append_charp(writer, op)) {
            return -1;
        }

        if (-1 == append_ast_expr(writer,
                                  (expr_ty)asdl_seq_GET(values, i),
                                  false,
                                  false)) {
            return -1;
        }
    }

    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_binop(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    const char *op;

    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.BinOp.left, false, false)) {
        return -1;
    }

    switch(e->v.BinOp.op) {
    case Add: op = " + "; break;
    case Sub: op = " - "; break;
    case Mult: op = " * "; break;
    case MatMult: op = " @ "; break;
    case Div: op = " / "; break;
    case Mod: op = " % "; break;
    case LShift: op = " << "; break;
    case RShift: op = " >> "; break;
    case BitOr: op = " | "; break;
    case BitXor: op = " ^ "; break;
    case BitAnd: op = " & "; break;
    case FloorDiv: op = " // "; break;
    case Pow: op = " ** "; break;
    }

    if (-1 == append_charp(writer, op)) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.BinOp.right, false, false)) {
        return -1;
    }

    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_unaryop(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    const char *op;

    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    switch(e->v.UnaryOp.op) {
    case Invert: op = "~"; break;
    case Not: op = "not "; break;
    case UAdd: op = "+"; break;
    case USub: op = "-"; break;
    }

    if (-1 == append_charp(writer, op)) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.UnaryOp.operand, false, false)) {
        return -1;
    }

    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_arg(_PyUnicodeWriter *writer, arg_ty arg)
{
    if (-1 == _PyUnicodeWriter_WriteStr(writer, arg->arg)) {
        return -1;
    }
    if (arg->annotation) {
        if (-1 == append_charp(writer, ": ")) {
            return -1;
        }
        if (-1 == append_ast_expr(writer, arg->annotation, true, false)) {
            return -1;
        }
    }
    return 0;
}

static int
append_ast_args(_PyUnicodeWriter *writer, arguments_ty args)
{
    bool first;
    Py_ssize_t i, di, arg_count, default_count;
    arg_ty arg;
    expr_ty default_;

    first = true;

    /* positional arguments with defaults */
    arg_count = asdl_seq_LEN(args->args);
    default_count = asdl_seq_LEN(args->defaults);
    for (i = 0; i < arg_count; i++) {
        if (first) {
            first = false;
        }
        else if (-1 == append_charp(writer, ", ")) {
            return -1;
        }

        arg = (arg_ty)asdl_seq_GET(args->args, i);
        if (-1 == append_ast_arg(writer, arg)) {
            return -1;
        }

        di = i - arg_count + default_count;
        if (di >= 0) {
            if (-1 == append_charp(writer, "=")) {
                return -1;
            }
            default_ = (expr_ty)asdl_seq_GET(args->defaults, di);
            if (-1 == append_ast_expr(writer, default_, false, false)) {
                return -1;
            }
        }
    }

    /* vararg, or bare '*' if no varargs but keyword-only arguments present */
    if (args->vararg || args->kwonlyargs) {
        if (first) {
            first = false;
        }
        else if (-1 == append_charp(writer, ", ")) {
            return -1;
        }

        if (-1 == append_charp(writer, "*")) {
            return -1;
        }

        if (args->vararg) {
            if (-1 == append_ast_arg(writer, args->vararg)) {
                return -1;
            }
        }
    }

    /* keyword-only arguments */
    arg_count = asdl_seq_LEN(args->kwonlyargs);
    default_count = asdl_seq_LEN(args->kw_defaults);
    for (i = 0; i < arg_count; i++) {
        if (first) {
            first = false;
        }
        else if (-1 == append_charp(writer, ", ")) {
            return -1;
        }

        arg = (arg_ty)asdl_seq_GET(args->kwonlyargs, i);
        if (-1 == append_ast_arg(writer, arg)) {
            return -1;
        }

        di = i - arg_count + default_count;
        if (di >= 0) {
            if (-1 == append_charp(writer, "=")) {
                return -1;
            }
            default_ = (expr_ty)asdl_seq_GET(args->kw_defaults, di);
            if (-1 == append_ast_expr(writer, default_, false, false)) {
                return -1;
            }
        }
    }

    /* **kwargs */
    if (args->kwarg) {
        if (first) {
            first = false;
        }
        else if (-1 == append_charp(writer, ", ")) {
            return -1;
        }

        if (-1 == append_charp(writer, "**")) {
            return -1;
        }

        if (-1 == append_ast_arg(writer, args->kwarg)) {
            return -1;
        }
    }

    return 0;
}

static int
append_ast_lambda(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    if (-1 == append_charp(writer, "lambda ")) {
        return -1;
    }

    if (-1 == append_ast_args(writer, e->v.Lambda.args)) {
        return -1;
    }

    if (-1 == append_charp(writer, ": ")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.Lambda.body, true, false)) {
        return -1;
    }

    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_ifexp(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.IfExp.body, false, false)) {
        return -1;
    }

    if (-1 == append_charp(writer, " if ")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.IfExp.test, false, false)) {
        return -1;
    }

    if (-1 == append_charp(writer, " else ")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.IfExp.orelse, false, false)) {
        return -1;
    }

    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_dict(_PyUnicodeWriter *writer, expr_ty e)
{
    Py_ssize_t i, value_count;
    expr_ty key_node, value_node;

    if (-1 == append_charp(writer, "{")) {
        return -1;
    }

    value_count = asdl_seq_LEN(e->v.Dict.values);

    for (i = 0; i < value_count; i++) {
        if (i > 0 && -1 == append_charp(writer, ", ")) {
            return -1;
        }
        key_node = (expr_ty)asdl_seq_GET(e->v.Dict.keys, i);
        if (key_node != NULL) {
            if (-1 == append_ast_expr(writer, key_node, false, false)) {
                return -1;
            }

            if (-1 == append_charp(writer, ": ")) {
                return -1;
            }
        }
        else if (-1 == append_charp(writer, "**")) {
            return -1;
        }

        value_node = (expr_ty)asdl_seq_GET(e->v.Dict.values, i);
        if (-1 == append_ast_expr(writer, value_node, false, false)) {
            return -1;
        }
    }

    return append_charp(writer, "}");
}

static int
append_ast_set(_PyUnicodeWriter *writer, expr_ty e)
{
    Py_ssize_t i, elem_count;
    expr_ty elem_node;

    if (-1 == append_charp(writer, "{")) {
        return -1;
    }

    elem_count = asdl_seq_LEN(e->v.Set.elts);
    for (i = 0; i < elem_count; i++) {
        if (i > 0 && -1 == append_charp(writer, ", ")) {
            return -1;
        }

        elem_node = (expr_ty)asdl_seq_GET(e->v.Set.elts, i);
        if (-1 == append_ast_expr(writer, elem_node, false, false)) {
            return -1;
        }
    }

    return append_charp(writer, "}");
}

static int
append_ast_list(_PyUnicodeWriter *writer, expr_ty e)
{
    Py_ssize_t i, elem_count;
    expr_ty elem_node;

    if (-1 == append_charp(writer, "[")) {
        return -1;
    }

    elem_count = asdl_seq_LEN(e->v.List.elts);
    for (i = 0; i < elem_count; i++) {
        if (i > 0 && -1 == append_charp(writer, ", ")) {
            return -1;
        }
        elem_node = (expr_ty)asdl_seq_GET(e->v.List.elts, i);
        if (-1 == append_ast_expr(writer, elem_node, false, false)) {
            return -1;
        }
    }

    return append_charp(writer, "]");
}

static int
append_ast_tuple(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    Py_ssize_t i, elem_count;
    expr_ty elem_node;

    elem_count = asdl_seq_LEN(e->v.Tuple.elts);

    if (!omit_parens || elem_count < 2) {
        if (-1 == append_charp(writer, "(")) {
            return -1;
        }
    }

    for (i = 0; i < elem_count; i++) {
        if ((i > 0 || elem_count == 1) && -1 == append_charp(writer, ", ")) {
            return -1;
        }
        elem_node = (expr_ty)asdl_seq_GET(e->v.Tuple.elts, i);
        if (-1 == append_ast_expr(writer, elem_node, false, false)) {
            return -1;
        }
    }

    if (!omit_parens || elem_count < 2) {
        return append_charp(writer, ")");
    }

    return 0;
}

static int
append_ast_comprehension(_PyUnicodeWriter *writer, comprehension_ty gen)
{
    Py_ssize_t i, if_count;

    if (-1 == append_charp(writer, gen->is_async ? " async for " : " for ")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, gen->target, true, false)) {
        return -1;
    }

    if (-1 == append_charp(writer, " in ")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, gen->iter, false, false)) {
        return -1;
    }

    if_count = asdl_seq_LEN(gen->ifs);
    for (i = 0; i < if_count; i++) {
        if (-1 == append_charp(writer, " if ")) {
            return -1;
        }

        if (-1 == append_ast_expr(writer,
                                  (expr_ty)asdl_seq_GET(gen->ifs, i),
                                  false,
                                  false)) {
            return -1;
        }
    }
    return 0;
}

static int
append_ast_comprehensions(_PyUnicodeWriter *writer, asdl_seq *comprehensions)
{
    Py_ssize_t i, gen_count;
    comprehension_ty comp_node;
    gen_count = asdl_seq_LEN(comprehensions);

    for (i = 0; i < gen_count; i++) {
        comp_node = (comprehension_ty)asdl_seq_GET(comprehensions, i);
        if (-1 == append_ast_comprehension(writer, comp_node)) {
            return -1;
        }
    }

    return 0;
}

static int
append_ast_genexp(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.GeneratorExp.elt, false, false)) {
        return -1;
    }

    if (-1 == append_ast_comprehensions(writer, e->v.GeneratorExp.generators)) {
        return -1;
    }

    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_listcomp(_PyUnicodeWriter *writer, expr_ty e)
{
    if (-1 == append_charp(writer, "[")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.ListComp.elt, false, false)) {
        return -1;
    }

    if (-1 == append_ast_comprehensions(writer, e->v.ListComp.generators)) {
        return -1;
    }

    return append_charp(writer, "]");
}

static int
append_ast_setcomp(_PyUnicodeWriter *writer, expr_ty e)
{
    if (-1 == append_charp(writer, "{")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.SetComp.elt, false, false)) {
        return -1;
    }

    if (-1 == append_ast_comprehensions(writer, e->v.SetComp.generators)) {
        return -1;
    }

    return append_charp(writer, "}");
}

static int
append_ast_dictcomp(_PyUnicodeWriter *writer, expr_ty e)
{
    if (-1 == append_charp(writer, "{")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.DictComp.key, false, false)) {
        return -1;
    }

    if (-1 == append_charp(writer, ": ")) {
        return -1;
    }

    if (-1 == append_ast_expr(writer, e->v.DictComp.value, false, false)) {
        return -1;
    }

    if (-1 == append_ast_comprehensions(writer, e->v.DictComp.generators)) {
        return -1;
    }

    return append_charp(writer, "}");
}

static int
append_ast_compare(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    const char *op;
    Py_ssize_t i, comparator_count;
    asdl_seq *comparators;
    asdl_int_seq *ops;

    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    comparators = e->v.Compare.comparators;
    ops = e->v.Compare.ops;
    comparator_count = asdl_seq_LEN(comparators);
    assert(comparator_count > 0);
    assert(comparator_count == asdl_seq_LEN(ops));

    if (-1 == append_ast_expr(writer, e->v.Compare.left, false, false)) {
        return -1;
    }

    for (i = 0; i < comparator_count; i++) {
        switch ((cmpop_ty)asdl_seq_GET(ops, i)) {
        case Eq:
            op = " == ";
            break;
        case NotEq:
            op = " != ";
            break;
        case Lt:
            op = " < ";
            break;
        case LtE:
            op = " <= ";
            break;
        case Gt:
            op = " > ";
            break;
        case GtE:
            op = " >= ";
            break;
        case Is:
            op = " is ";
            break;
        case IsNot:
            op = " is not ";
            break;
        case In:
            op = " in ";
            break;
        case NotIn:
            op = " not in ";
            break;
        default:
            PyErr_SetString(PyExc_SystemError,
                            "unexpected comparison kind");
            return -1;
        }

        if (-1 == append_charp(writer, op)) {
            return -1;
        }

        if (-1 == append_ast_expr(writer,
                                  (expr_ty)asdl_seq_GET(comparators, i),
                                  false,
                                  false)) {
            return -1;
        }
    }

    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_keyword(_PyUnicodeWriter *writer, keyword_ty kw)
{
    if (kw->arg == NULL) {
        if (-1 == append_charp(writer, "**")) {
            return -1;
        }
    }
    else {
        if (-1 == _PyUnicodeWriter_WriteStr(writer, kw->arg)) {
            return -1;
        }

        if (-1 == append_charp(writer, "=")) {
            return -1;
        }
    }

    return append_ast_expr(writer, kw->value, false, false);
}

static int
append_ast_call(_PyUnicodeWriter *writer, expr_ty e)
{
    bool first;
    Py_ssize_t i, arg_count, kw_count;
    expr_ty expr;
    keyword_ty kw;

    if (-1 == append_ast_expr(writer, e->v.Call.func, false, false)) {
        return -1;
    }

    if (-1 == append_charp(writer, "(")) {
        return -1;
    }

    first = true;
    arg_count = asdl_seq_LEN(e->v.Call.args);
    for (i = 0; i < arg_count; i++) {
        if (first) {
            first = false;
        }
        else if (-1 == append_charp(writer, ", ")) {
            return -1;
        }

        expr = (expr_ty)asdl_seq_GET(e->v.Call.args, i);
        if (-1 == append_ast_expr(writer, expr, false, false)) {
            return -1;
        }
    }

    kw_count = asdl_seq_LEN(e->v.Call.keywords);
    for (i = 0; i < kw_count; i++) {
        if (first) {
            first = false;
        }
        else if (-1 == append_charp(writer, ", ")) {
            return -1;
        }

        kw = (keyword_ty)asdl_seq_GET(e->v.Call.keywords, i);
        if (-1 == append_ast_keyword(writer, kw)) {
            return -1;
        }
    }

    return append_charp(writer, ")");
}

static int
append_ast_attribute(_PyUnicodeWriter *writer, expr_ty e)
{
    const char *period;
    if (-1 == append_ast_expr(writer, e->v.Attribute.value, false, false)) {
        return -1;
    }

    /* Special case: integers require a space for attribute access to be
       unambiguous.  Floats and complex numbers don't but work with it, too. */
    if (e->v.Attribute.value->kind == Num_kind ||
        e->v.Attribute.value->kind == Constant_kind)
    {
        period = " .";
    }
    else {
        period = ".";
    }
    if (-1 == append_charp(writer, period)) {
        return -1;
    }

    return _PyUnicodeWriter_WriteStr(writer, e->v.Attribute.attr);
}

static int
append_ast_simple_slice(_PyUnicodeWriter *writer, slice_ty slice)
{
    if (slice->v.Slice.lower) {
        if (-1 == append_ast_expr(writer, slice->v.Slice.lower, false, false)) {
            return -1;
        }
    }

    if (-1 == append_charp(writer, ":")) {
        return -1;
    }

    if (slice->v.Slice.upper) {
        if (-1 == append_ast_expr(writer, slice->v.Slice.upper, false, false)) {
            return -1;
        }
    }

    if (slice->v.Slice.step) {
        if (-1 == append_charp(writer, ":")) {
            return -1;
        }
        if (-1 == append_ast_expr(writer, slice->v.Slice.step, false, false)) {
            return -1;
        }
    }
    return 0;
}

static int
append_ast_ext_slice(_PyUnicodeWriter *writer, slice_ty slice)
{
    Py_ssize_t i, dims_count;
    dims_count = asdl_seq_LEN(slice->v.ExtSlice.dims);
    for (i = 0; i < dims_count; i++) {
        if (i > 0 && -1 == append_charp(writer, ", ")) {
            return -1;
        }
        if (-1 == append_ast_expr(writer,
                                  (expr_ty)asdl_seq_GET(slice->v.ExtSlice.dims, i),
                                  false,
                                  false)) {
            return -1;
        }
    }
    return 0;
}

static int
append_ast_slice(_PyUnicodeWriter *writer, slice_ty slice, bool omit_parens)
{
    switch(slice->kind) {
    case Slice_kind:
        return append_ast_simple_slice(writer, slice);
    case ExtSlice_kind:
        return append_ast_ext_slice(writer, slice);
    case Index_kind:
        return append_ast_expr(writer, slice->v.Index.value, omit_parens, false);
    default:
        PyErr_SetString(PyExc_SystemError,
                        "unexpected slice kind");
        return -1;
    }
}

static int
append_ast_subscript(_PyUnicodeWriter *writer, expr_ty e)
{
    if (-1 == append_ast_expr(writer, e->v.Subscript.value, false, false)) {
        return -1;
    }

    if (-1 == append_charp(writer, "[")) {
        return -1;
    }

    if (-1 == append_ast_slice(writer, e->v.Subscript.slice, true)) {
        return -1;
    }

    return append_charp(writer, "]");
}

static int
append_ast_starred(_PyUnicodeWriter *writer, expr_ty e)
{
    if (-1 == append_charp(writer, "*")) {
        return -1;
    }

    return append_ast_expr(writer, e->v.Starred.value, false, false);
}

static int
append_ast_yield(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    if (-1 == append_charp(writer, e->v.Yield.value ? "yield " : "yield")) {
        return -1;
    }

    if (e->v.Yield.value) {
        if (-1 == append_ast_expr(writer, e->v.Yield.value, false, false)) {
            return -1;
        }
    }
    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_yield_from(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    if (-1 == append_charp(writer,
                           e->v.YieldFrom.value ? "yield from " : "yield from")) {
        return -1;
    }

    if (e->v.YieldFrom.value) {
        if (-1 == append_ast_expr(writer, e->v.YieldFrom.value, false, false)) {
            return -1;
        }
    }
    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_await(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens)
{
    if (!omit_parens && -1 == append_charp(writer, "(")) {
        return -1;
    }

    if (-1 == append_charp(writer, e->v.Await.value ? "await " : "await")) {
        return -1;
    }

    if (e->v.Await.value) {
        if (-1 == append_ast_expr(writer, e->v.Await.value, false, false)) {
            return -1;
        }
    }
    return omit_parens ? 0 : append_charp(writer, ")");
}

static int
append_ast_str(_PyUnicodeWriter *writer, expr_ty e, bool omit_string_brackets)
{
    PyObject *s =  e->v.Str.s;
    if (omit_string_brackets) {
        return _PyUnicodeWriter_WriteStr(writer, s);
    }

    return append_repr(writer, s);
}

static int
append_ast_expr(_PyUnicodeWriter *writer, expr_ty e, bool omit_parens,
    bool omit_string_brackets)
{
    switch (e->kind) {
    case BoolOp_kind:
        return append_ast_boolop(writer, e, omit_parens);
    case BinOp_kind:
        return append_ast_binop(writer, e, omit_parens);
    case UnaryOp_kind:
        return append_ast_unaryop(writer, e, omit_parens);
    case Lambda_kind:
        return append_ast_lambda(writer, e, omit_parens);
    case IfExp_kind:
        return append_ast_ifexp(writer, e, omit_parens);
    case Dict_kind:
        return append_ast_dict(writer, e);
    case Set_kind:
        return append_ast_set(writer, e);
    case GeneratorExp_kind:
        return append_ast_genexp(writer, e, omit_parens);
    case ListComp_kind:
        return append_ast_listcomp(writer, e);
    case SetComp_kind:
        return append_ast_setcomp(writer, e);
    case DictComp_kind:
        return append_ast_dictcomp(writer, e);
    case Yield_kind:
        return append_ast_yield(writer, e, omit_parens);
    case YieldFrom_kind:
        return append_ast_yield_from(writer, e, omit_parens);
    case Await_kind:
        return append_ast_await(writer, e, omit_parens);
    case Compare_kind:
        return append_ast_compare(writer, e, omit_parens);
    case Call_kind:
        return append_ast_call(writer, e);
    case Constant_kind:
        return append_repr(writer, e->v.Constant.value);
    case Num_kind:
        return append_repr(writer, e->v.Num.n);
    case Str_kind:
        return append_ast_str(writer, e, omit_string_brackets);
    case JoinedStr_kind:
        PyErr_SetString(PyExc_SystemError,
                        "f-string support in annotations not implemented yet");
        return -1;
    case FormattedValue_kind:
        PyErr_SetString(PyExc_SystemError,
                        "f-string support in annotations not implemented yet");
        return -1;
    case Bytes_kind:
        return append_repr(writer, e->v.Bytes.s);
    case Ellipsis_kind:
        return append_charp(writer, "...");
    case NameConstant_kind:
        return append_repr(writer, e->v.NameConstant.value);
    /* The following exprs can be assignment targets. */
    case Attribute_kind:
        return append_ast_attribute(writer, e);
    case Subscript_kind:
        return append_ast_subscript(writer, e);
    case Starred_kind:
        return append_ast_starred(writer, e);
    case Name_kind:
        return _PyUnicodeWriter_WriteStr(writer, e->v.Name.id);
    /* child nodes of List and Tuple will have expr_context set */
    case List_kind:
        return append_ast_list(writer, e);
    case Tuple_kind:
        return append_ast_tuple(writer, e, omit_parens);
    default:
        PyErr_SetString(PyExc_SystemError,
                        "unknown expression kind");
        return -1;
    }
}

PyObject *
_PyAST_ExprAsUnicode(expr_ty e, bool omit_parens, bool omit_string_brackets)
{
    _PyUnicodeWriter writer;
    _PyUnicodeWriter_Init(&writer);
    writer.min_length = 256;
    writer.overallocate = 1;
    if (-1 == append_ast_expr(&writer, e, omit_parens, omit_string_brackets)) {
        _PyUnicodeWriter_Dealloc(&writer);
        return NULL;
    }
    return _PyUnicodeWriter_Finish(&writer);
}
