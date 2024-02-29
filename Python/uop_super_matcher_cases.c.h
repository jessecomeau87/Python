// This file is generated by Tools/cases_generator/tier2_super_matcher_generator.py
// from:
//   Python/bytecodes.c
// Do not edit!

#ifdef TIER_ONE
    #error "This file is for Tier 2 only"
#endif
#define TIER_TWO 2

        case _GUARD_BOTH_INT: {
            if (this_instr[1].opcode == _BINARY_OP_MULTIPLY_INT) {
                DPRINTF(2, "Inserting super _GUARD_BOTH_INT__BINARY_OP_MULTIPLY_INT\n");REPLACE_OP(this_instr, _GUARD_BOTH_INT__BINARY_OP_MULTIPLY_INT, this_instr[0].oparg, this_instr[0].operand);
                for (int i = 1; i < 2; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 2;
                break;
            }
            if (this_instr[1].opcode == _BINARY_OP_ADD_INT) {
                DPRINTF(2, "Inserting super _GUARD_BOTH_INT__BINARY_OP_ADD_INT\n");REPLACE_OP(this_instr, _GUARD_BOTH_INT__BINARY_OP_ADD_INT, this_instr[0].oparg, this_instr[0].operand);
                for (int i = 1; i < 2; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 2;
                break;
            }
            if (this_instr[1].opcode == _BINARY_OP_SUBTRACT_INT) {
                DPRINTF(2, "Inserting super _GUARD_BOTH_INT__BINARY_OP_SUBTRACT_INT\n");REPLACE_OP(this_instr, _GUARD_BOTH_INT__BINARY_OP_SUBTRACT_INT, this_instr[0].oparg, this_instr[0].operand);
                for (int i = 1; i < 2; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 2;
                break;
            }
            this_instr += 1;
            break;
        }

        case _GUARD_BOTH_FLOAT: {
            if (this_instr[1].opcode == _BINARY_OP_MULTIPLY_FLOAT) {
                DPRINTF(2, "Inserting super _GUARD_BOTH_FLOAT__BINARY_OP_MULTIPLY_FLOAT\n");REPLACE_OP(this_instr, _GUARD_BOTH_FLOAT__BINARY_OP_MULTIPLY_FLOAT, this_instr[0].oparg, this_instr[0].operand);
                for (int i = 1; i < 2; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 2;
                break;
            }
            if (this_instr[1].opcode == _BINARY_OP_ADD_FLOAT) {
                DPRINTF(2, "Inserting super _GUARD_BOTH_FLOAT__BINARY_OP_ADD_FLOAT\n");REPLACE_OP(this_instr, _GUARD_BOTH_FLOAT__BINARY_OP_ADD_FLOAT, this_instr[0].oparg, this_instr[0].operand);
                for (int i = 1; i < 2; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 2;
                break;
            }
            if (this_instr[1].opcode == _BINARY_OP_SUBTRACT_FLOAT) {
                DPRINTF(2, "Inserting super _GUARD_BOTH_FLOAT__BINARY_OP_SUBTRACT_FLOAT\n");REPLACE_OP(this_instr, _GUARD_BOTH_FLOAT__BINARY_OP_SUBTRACT_FLOAT, this_instr[0].oparg, this_instr[0].operand);
                for (int i = 1; i < 2; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 2;
                break;
            }
            this_instr += 1;
            break;
        }

        case _GUARD_BOTH_UNICODE: {
            if (this_instr[1].opcode == _BINARY_OP_ADD_UNICODE) {
                DPRINTF(2, "Inserting super _GUARD_BOTH_UNICODE__BINARY_OP_ADD_UNICODE\n");REPLACE_OP(this_instr, _GUARD_BOTH_UNICODE__BINARY_OP_ADD_UNICODE, this_instr[0].oparg, this_instr[0].operand);
                for (int i = 1; i < 2; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 2;
                break;
            }
            this_instr += 1;
            break;
        }

        case _CHECK_CALL_BOUND_METHOD_EXACT_ARGS: {
            switch (this_instr[1].opcode) {
                case _INIT_CALL_BOUND_METHOD_EXACT_ARGS: {
                    switch (this_instr[2].opcode) {
                        case _CHECK_FUNCTION_EXACT_ARGS: {
                            switch (this_instr[3].opcode) {
                                case _CHECK_STACK_SPACE: {
                                    if (this_instr[4].opcode == _INIT_CALL_PY_EXACT_ARGS) {
                                        DPRINTF(2, "Inserting super _CALL_BOUND_METHOD_EXACT_ARGS\n");REPLACE_OP(this_instr, _CALL_BOUND_METHOD_EXACT_ARGS, this_instr[4].oparg, this_instr[2].operand);
                                        for (int i = 1; i < 5; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 5;
                                        break;
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            this_instr += 1;
            break;
        }

        case _CHECK_FUNCTION_EXACT_ARGS: {
            switch (this_instr[1].opcode) {
                case _CHECK_STACK_SPACE: {
                    if (this_instr[2].opcode == _INIT_CALL_PY_EXACT_ARGS) {
                        DPRINTF(2, "Inserting super _CHECK_CALL_PY_EXACT_ARGS\n");REPLACE_OP(this_instr, _CHECK_CALL_PY_EXACT_ARGS, this_instr[2].oparg, this_instr[0].operand);
                        for (int i = 1; i < 3; i++) { REPLACE_OP((&this_instr[i]), _NOP, 0, 0); }this_instr += 3;
                        break;
                    }
                    break;
                }
            }
            this_instr += 1;
            break;
        }

#undef TIER_TWO
