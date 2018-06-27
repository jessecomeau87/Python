#! /usr/bin/env python3

"""Non-terminal symbols of Python grammar (from "graminit.h")."""

#  This file is automatically generated; please don't muck it up!
#
#  To update the symbols in this file, 'cd' to the top directory of
#  the python source tree after building the interpreter and run:
#
#    ./python Lib/symbol.py

#--start constants--
single_input = 256
file_input = 257
eval_input = 258
decorator = 259
decorators = 260
decorated = 261
async_funcdef = 262
funcdef = 263
parameters = 264
typedargslist = 265
tfpdef = 266
varargslist = 267
vfpdef = 268
stmt = 269
simple_stmt = 270
small_stmt = 271
expr_stmt = 272
annassign = 273
testlist_star_expr = 274
augassign = 275
del_stmt = 276
pass_stmt = 277
flow_stmt = 278
break_stmt = 279
continue_stmt = 280
return_stmt = 281
yield_stmt = 282
raise_stmt = 283
import_stmt = 284
import_name = 285
import_from = 286
import_as_name = 287
dotted_as_name = 288
import_as_names = 289
dotted_as_names = 290
dotted_name = 291
global_stmt = 292
nonlocal_stmt = 293
assert_stmt = 294
compound_stmt = 295
async_stmt = 296
if_stmt = 297
while_stmt = 298
for_stmt = 299
try_stmt = 300
with_stmt = 301
with_item = 302
except_clause = 303
suite = 304
test = 305
test_nocond = 306
lambdef = 307
lambdef_nocond = 308
or_test = 309
and_test = 310
not_test = 311
comparison = 312
comp_op = 313
star_expr = 314
expr = 315
xor_expr = 316
and_expr = 317
shift_expr = 318
arith_expr = 319
term = 320
factor = 321
power = 322
atom_expr = 323
atom = 324
testlist_comp = 325
trailer = 326
subscriptlist = 327
subscript = 328
sliceop = 329
exprlist = 330
testlist = 331
dictorsetmaker = 332
classdef = 333
arglist = 334
argument = 335
comp_iter = 336
sync_comp_for = 337
comp_for = 338
comp_if = 339
encoding_decl = 340
yield_expr = 341
yield_arg = 342
#--end constants--

sym_name = {}
for _name, _value in list(globals().items()):
    if type(_value) is type(0):
        sym_name[_value] = _name


def _main():
    import sys
    import token
    if len(sys.argv) == 1:
        sys.argv = sys.argv + ["Include/graminit.h", "Lib/symbol.py"]
    token._main()

if __name__ == "__main__":
    _main()
