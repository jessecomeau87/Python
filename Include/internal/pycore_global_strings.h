#ifndef Py_INTERNAL_GLOBAL_STRINGS_H
#define Py_INTERNAL_GLOBAL_STRINGS_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

// The data structure & init here are inspired by Tools/build/deepfreeze.py.

// All field names generated by ASCII_STR() have a common prefix,
// to help avoid collisions with keywords, macros, etc.

#define STRUCT_FOR_ASCII_STR(LITERAL) \
    struct { \
        PyASCIIObject _ascii; \
        uint8_t _data[sizeof(LITERAL)]; \
    }
#define STRUCT_FOR_STR(NAME, LITERAL) \
    STRUCT_FOR_ASCII_STR(LITERAL) _py_ ## NAME;
#define STRUCT_FOR_ID(NAME) \
    STRUCT_FOR_ASCII_STR(#NAME) _py_ ## NAME;

// XXX Order by frequency of use?

/* The following is auto-generated by Tools/build/generate_global_objects.py. */
struct _Py_global_strings {
    struct {
        STRUCT_FOR_STR(anon_dictcomp, "<dictcomp>")
        STRUCT_FOR_STR(anon_genexpr, "<genexpr>")
        STRUCT_FOR_STR(anon_lambda, "<lambda>")
        STRUCT_FOR_STR(anon_listcomp, "<listcomp>")
        STRUCT_FOR_STR(anon_module, "<module>")
        STRUCT_FOR_STR(anon_null, "<NULL>")
        STRUCT_FOR_STR(anon_setcomp, "<setcomp>")
        STRUCT_FOR_STR(anon_string, "<string>")
        STRUCT_FOR_STR(anon_unknown, "<unknown>")
        STRUCT_FOR_STR(close_br, "}")
        STRUCT_FOR_STR(dbl_close_br, "}}")
        STRUCT_FOR_STR(dbl_open_br, "{{")
        STRUCT_FOR_STR(dbl_percent, "%%")
        STRUCT_FOR_STR(defaults, ".defaults")
        STRUCT_FOR_STR(dot, ".")
        STRUCT_FOR_STR(dot_locals, ".<locals>")
        STRUCT_FOR_STR(empty, "")
        STRUCT_FOR_STR(generic_base, ".generic_base")
        STRUCT_FOR_STR(json_decoder, "json.decoder")
        STRUCT_FOR_STR(kwdefaults, ".kwdefaults")
        STRUCT_FOR_STR(list_err, "list index out of range")
        STRUCT_FOR_STR(newline, "\n")
        STRUCT_FOR_STR(open_br, "{")
        STRUCT_FOR_STR(percent, "%")
        STRUCT_FOR_STR(type_params, ".type_params")
        STRUCT_FOR_STR(utf_8, "utf-8")
    } literals;

    struct {
        STRUCT_FOR_ID(CANCELLED)
        STRUCT_FOR_ID(FINISHED)
        STRUCT_FOR_ID(False)
        STRUCT_FOR_ID(JSONDecodeError)
        STRUCT_FOR_ID(PENDING)
        STRUCT_FOR_ID(Py_Repr)
        STRUCT_FOR_ID(TextIOWrapper)
        STRUCT_FOR_ID(True)
        STRUCT_FOR_ID(WarningMessage)
        STRUCT_FOR_ID(_)
        STRUCT_FOR_ID(_WindowsConsoleIO)
        STRUCT_FOR_ID(__IOBase_closed)
        STRUCT_FOR_ID(__abc_tpflags__)
        STRUCT_FOR_ID(__abs__)
        STRUCT_FOR_ID(__abstractmethods__)
        STRUCT_FOR_ID(__add__)
        STRUCT_FOR_ID(__aenter__)
        STRUCT_FOR_ID(__aexit__)
        STRUCT_FOR_ID(__aiter__)
        STRUCT_FOR_ID(__all__)
        STRUCT_FOR_ID(__and__)
        STRUCT_FOR_ID(__anext__)
        STRUCT_FOR_ID(__annotations__)
        STRUCT_FOR_ID(__args__)
        STRUCT_FOR_ID(__asyncio_running_event_loop__)
        STRUCT_FOR_ID(__await__)
        STRUCT_FOR_ID(__bases__)
        STRUCT_FOR_ID(__bool__)
        STRUCT_FOR_ID(__buffer__)
        STRUCT_FOR_ID(__build_class__)
        STRUCT_FOR_ID(__builtins__)
        STRUCT_FOR_ID(__bytes__)
        STRUCT_FOR_ID(__call__)
        STRUCT_FOR_ID(__cantrace__)
        STRUCT_FOR_ID(__class__)
        STRUCT_FOR_ID(__class_getitem__)
        STRUCT_FOR_ID(__classcell__)
        STRUCT_FOR_ID(__classdict__)
        STRUCT_FOR_ID(__classdictcell__)
        STRUCT_FOR_ID(__complex__)
        STRUCT_FOR_ID(__contains__)
        STRUCT_FOR_ID(__copy__)
        STRUCT_FOR_ID(__ctypes_from_outparam__)
        STRUCT_FOR_ID(__del__)
        STRUCT_FOR_ID(__delattr__)
        STRUCT_FOR_ID(__delete__)
        STRUCT_FOR_ID(__delitem__)
        STRUCT_FOR_ID(__dict__)
        STRUCT_FOR_ID(__dictoffset__)
        STRUCT_FOR_ID(__dir__)
        STRUCT_FOR_ID(__divmod__)
        STRUCT_FOR_ID(__doc__)
        STRUCT_FOR_ID(__enter__)
        STRUCT_FOR_ID(__eq__)
        STRUCT_FOR_ID(__exit__)
        STRUCT_FOR_ID(__file__)
        STRUCT_FOR_ID(__float__)
        STRUCT_FOR_ID(__floordiv__)
        STRUCT_FOR_ID(__format__)
        STRUCT_FOR_ID(__fspath__)
        STRUCT_FOR_ID(__ge__)
        STRUCT_FOR_ID(__get__)
        STRUCT_FOR_ID(__getattr__)
        STRUCT_FOR_ID(__getattribute__)
        STRUCT_FOR_ID(__getinitargs__)
        STRUCT_FOR_ID(__getitem__)
        STRUCT_FOR_ID(__getnewargs__)
        STRUCT_FOR_ID(__getnewargs_ex__)
        STRUCT_FOR_ID(__getstate__)
        STRUCT_FOR_ID(__gt__)
        STRUCT_FOR_ID(__hash__)
        STRUCT_FOR_ID(__iadd__)
        STRUCT_FOR_ID(__iand__)
        STRUCT_FOR_ID(__ifloordiv__)
        STRUCT_FOR_ID(__ilshift__)
        STRUCT_FOR_ID(__imatmul__)
        STRUCT_FOR_ID(__imod__)
        STRUCT_FOR_ID(__import__)
        STRUCT_FOR_ID(__imul__)
        STRUCT_FOR_ID(__index__)
        STRUCT_FOR_ID(__init__)
        STRUCT_FOR_ID(__init_subclass__)
        STRUCT_FOR_ID(__instancecheck__)
        STRUCT_FOR_ID(__int__)
        STRUCT_FOR_ID(__invert__)
        STRUCT_FOR_ID(__ior__)
        STRUCT_FOR_ID(__ipow__)
        STRUCT_FOR_ID(__irshift__)
        STRUCT_FOR_ID(__isabstractmethod__)
        STRUCT_FOR_ID(__isub__)
        STRUCT_FOR_ID(__iter__)
        STRUCT_FOR_ID(__itruediv__)
        STRUCT_FOR_ID(__ixor__)
        STRUCT_FOR_ID(__le__)
        STRUCT_FOR_ID(__len__)
        STRUCT_FOR_ID(__length_hint__)
        STRUCT_FOR_ID(__lltrace__)
        STRUCT_FOR_ID(__loader__)
        STRUCT_FOR_ID(__lshift__)
        STRUCT_FOR_ID(__lt__)
        STRUCT_FOR_ID(__main__)
        STRUCT_FOR_ID(__match_args__)
        STRUCT_FOR_ID(__matmul__)
        STRUCT_FOR_ID(__missing__)
        STRUCT_FOR_ID(__mod__)
        STRUCT_FOR_ID(__module__)
        STRUCT_FOR_ID(__mro_entries__)
        STRUCT_FOR_ID(__mul__)
        STRUCT_FOR_ID(__name__)
        STRUCT_FOR_ID(__ne__)
        STRUCT_FOR_ID(__neg__)
        STRUCT_FOR_ID(__new__)
        STRUCT_FOR_ID(__newobj__)
        STRUCT_FOR_ID(__newobj_ex__)
        STRUCT_FOR_ID(__next__)
        STRUCT_FOR_ID(__notes__)
        STRUCT_FOR_ID(__or__)
        STRUCT_FOR_ID(__orig_class__)
        STRUCT_FOR_ID(__origin__)
        STRUCT_FOR_ID(__package__)
        STRUCT_FOR_ID(__parameters__)
        STRUCT_FOR_ID(__path__)
        STRUCT_FOR_ID(__pos__)
        STRUCT_FOR_ID(__pow__)
        STRUCT_FOR_ID(__prepare__)
        STRUCT_FOR_ID(__qualname__)
        STRUCT_FOR_ID(__radd__)
        STRUCT_FOR_ID(__rand__)
        STRUCT_FOR_ID(__rdivmod__)
        STRUCT_FOR_ID(__reduce__)
        STRUCT_FOR_ID(__reduce_ex__)
        STRUCT_FOR_ID(__release_buffer__)
        STRUCT_FOR_ID(__repr__)
        STRUCT_FOR_ID(__reversed__)
        STRUCT_FOR_ID(__rfloordiv__)
        STRUCT_FOR_ID(__rlshift__)
        STRUCT_FOR_ID(__rmatmul__)
        STRUCT_FOR_ID(__rmod__)
        STRUCT_FOR_ID(__rmul__)
        STRUCT_FOR_ID(__ror__)
        STRUCT_FOR_ID(__round__)
        STRUCT_FOR_ID(__rpow__)
        STRUCT_FOR_ID(__rrshift__)
        STRUCT_FOR_ID(__rshift__)
        STRUCT_FOR_ID(__rsub__)
        STRUCT_FOR_ID(__rtruediv__)
        STRUCT_FOR_ID(__rxor__)
        STRUCT_FOR_ID(__set__)
        STRUCT_FOR_ID(__set_name__)
        STRUCT_FOR_ID(__setattr__)
        STRUCT_FOR_ID(__setitem__)
        STRUCT_FOR_ID(__setstate__)
        STRUCT_FOR_ID(__sizeof__)
        STRUCT_FOR_ID(__slotnames__)
        STRUCT_FOR_ID(__slots__)
        STRUCT_FOR_ID(__spec__)
        STRUCT_FOR_ID(__static_attributes__)
        STRUCT_FOR_ID(__str__)
        STRUCT_FOR_ID(__sub__)
        STRUCT_FOR_ID(__subclasscheck__)
        STRUCT_FOR_ID(__subclasshook__)
        STRUCT_FOR_ID(__truediv__)
        STRUCT_FOR_ID(__trunc__)
        STRUCT_FOR_ID(__type_params__)
        STRUCT_FOR_ID(__typing_is_unpacked_typevartuple__)
        STRUCT_FOR_ID(__typing_prepare_subst__)
        STRUCT_FOR_ID(__typing_subst__)
        STRUCT_FOR_ID(__typing_unpacked_tuple_args__)
        STRUCT_FOR_ID(__warningregistry__)
        STRUCT_FOR_ID(__weaklistoffset__)
        STRUCT_FOR_ID(__weakref__)
        STRUCT_FOR_ID(__xor__)
        STRUCT_FOR_ID(_abc_impl)
        STRUCT_FOR_ID(_abstract_)
        STRUCT_FOR_ID(_active)
        STRUCT_FOR_ID(_align_)
        STRUCT_FOR_ID(_annotation)
        STRUCT_FOR_ID(_anonymous_)
        STRUCT_FOR_ID(_argtypes_)
        STRUCT_FOR_ID(_as_parameter_)
        STRUCT_FOR_ID(_asyncio_future_blocking)
        STRUCT_FOR_ID(_blksize)
        STRUCT_FOR_ID(_bootstrap)
        STRUCT_FOR_ID(_check_retval_)
        STRUCT_FOR_ID(_dealloc_warn)
        STRUCT_FOR_ID(_feature_version)
        STRUCT_FOR_ID(_field_types)
        STRUCT_FOR_ID(_fields_)
        STRUCT_FOR_ID(_finalizing)
        STRUCT_FOR_ID(_find_and_load)
        STRUCT_FOR_ID(_fix_up_module)
        STRUCT_FOR_ID(_flags_)
        STRUCT_FOR_ID(_get_sourcefile)
        STRUCT_FOR_ID(_handle_fromlist)
        STRUCT_FOR_ID(_initializing)
        STRUCT_FOR_ID(_io)
        STRUCT_FOR_ID(_is_text_encoding)
        STRUCT_FOR_ID(_length_)
        STRUCT_FOR_ID(_limbo)
        STRUCT_FOR_ID(_lock_unlock_module)
        STRUCT_FOR_ID(_loop)
        STRUCT_FOR_ID(_needs_com_addref_)
        STRUCT_FOR_ID(_pack_)
        STRUCT_FOR_ID(_restype_)
        STRUCT_FOR_ID(_showwarnmsg)
        STRUCT_FOR_ID(_shutdown)
        STRUCT_FOR_ID(_slotnames)
        STRUCT_FOR_ID(_strptime_datetime)
        STRUCT_FOR_ID(_swappedbytes_)
        STRUCT_FOR_ID(_type_)
        STRUCT_FOR_ID(_uninitialized_submodules)
        STRUCT_FOR_ID(_warn_unawaited_coroutine)
        STRUCT_FOR_ID(_xoptions)
        STRUCT_FOR_ID(a)
        STRUCT_FOR_ID(abs_tol)
        STRUCT_FOR_ID(access)
        STRUCT_FOR_ID(aclose)
        STRUCT_FOR_ID(add)
        STRUCT_FOR_ID(add_done_callback)
        STRUCT_FOR_ID(after_in_child)
        STRUCT_FOR_ID(after_in_parent)
        STRUCT_FOR_ID(aggregate_class)
        STRUCT_FOR_ID(alias)
        STRUCT_FOR_ID(allow_code)
        STRUCT_FOR_ID(append)
        STRUCT_FOR_ID(arg)
        STRUCT_FOR_ID(argdefs)
        STRUCT_FOR_ID(args)
        STRUCT_FOR_ID(arguments)
        STRUCT_FOR_ID(argv)
        STRUCT_FOR_ID(as_integer_ratio)
        STRUCT_FOR_ID(asend)
        STRUCT_FOR_ID(ast)
        STRUCT_FOR_ID(athrow)
        STRUCT_FOR_ID(attribute)
        STRUCT_FOR_ID(authorizer_callback)
        STRUCT_FOR_ID(autocommit)
        STRUCT_FOR_ID(b)
        STRUCT_FOR_ID(backtick)
        STRUCT_FOR_ID(base)
        STRUCT_FOR_ID(before)
        STRUCT_FOR_ID(big)
        STRUCT_FOR_ID(binary_form)
        STRUCT_FOR_ID(block)
        STRUCT_FOR_ID(bound)
        STRUCT_FOR_ID(buffer)
        STRUCT_FOR_ID(buffer_callback)
        STRUCT_FOR_ID(buffer_size)
        STRUCT_FOR_ID(buffering)
        STRUCT_FOR_ID(buffers)
        STRUCT_FOR_ID(bufsize)
        STRUCT_FOR_ID(builtins)
        STRUCT_FOR_ID(byteorder)
        STRUCT_FOR_ID(bytes)
        STRUCT_FOR_ID(bytes_per_sep)
        STRUCT_FOR_ID(c)
        STRUCT_FOR_ID(c_call)
        STRUCT_FOR_ID(c_exception)
        STRUCT_FOR_ID(c_return)
        STRUCT_FOR_ID(cached_statements)
        STRUCT_FOR_ID(cadata)
        STRUCT_FOR_ID(cafile)
        STRUCT_FOR_ID(call)
        STRUCT_FOR_ID(call_exception_handler)
        STRUCT_FOR_ID(call_soon)
        STRUCT_FOR_ID(callback)
        STRUCT_FOR_ID(cancel)
        STRUCT_FOR_ID(capath)
        STRUCT_FOR_ID(category)
        STRUCT_FOR_ID(cb_type)
        STRUCT_FOR_ID(certfile)
        STRUCT_FOR_ID(check_same_thread)
        STRUCT_FOR_ID(clear)
        STRUCT_FOR_ID(close)
        STRUCT_FOR_ID(closed)
        STRUCT_FOR_ID(closefd)
        STRUCT_FOR_ID(closure)
        STRUCT_FOR_ID(co_argcount)
        STRUCT_FOR_ID(co_cellvars)
        STRUCT_FOR_ID(co_code)
        STRUCT_FOR_ID(co_consts)
        STRUCT_FOR_ID(co_exceptiontable)
        STRUCT_FOR_ID(co_filename)
        STRUCT_FOR_ID(co_firstlineno)
        STRUCT_FOR_ID(co_flags)
        STRUCT_FOR_ID(co_freevars)
        STRUCT_FOR_ID(co_kwonlyargcount)
        STRUCT_FOR_ID(co_linetable)
        STRUCT_FOR_ID(co_name)
        STRUCT_FOR_ID(co_names)
        STRUCT_FOR_ID(co_nlocals)
        STRUCT_FOR_ID(co_posonlyargcount)
        STRUCT_FOR_ID(co_qualname)
        STRUCT_FOR_ID(co_stacksize)
        STRUCT_FOR_ID(co_varnames)
        STRUCT_FOR_ID(code)
        STRUCT_FOR_ID(col_offset)
        STRUCT_FOR_ID(command)
        STRUCT_FOR_ID(comment_factory)
        STRUCT_FOR_ID(compile_mode)
        STRUCT_FOR_ID(consts)
        STRUCT_FOR_ID(context)
        STRUCT_FOR_ID(contravariant)
        STRUCT_FOR_ID(cookie)
        STRUCT_FOR_ID(copy)
        STRUCT_FOR_ID(copyreg)
        STRUCT_FOR_ID(coro)
        STRUCT_FOR_ID(count)
        STRUCT_FOR_ID(covariant)
        STRUCT_FOR_ID(cwd)
        STRUCT_FOR_ID(d)
        STRUCT_FOR_ID(data)
        STRUCT_FOR_ID(database)
        STRUCT_FOR_ID(day)
        STRUCT_FOR_ID(decode)
        STRUCT_FOR_ID(decoder)
        STRUCT_FOR_ID(default)
        STRUCT_FOR_ID(defaultaction)
        STRUCT_FOR_ID(delete)
        STRUCT_FOR_ID(depth)
        STRUCT_FOR_ID(desired_access)
        STRUCT_FOR_ID(detect_types)
        STRUCT_FOR_ID(deterministic)
        STRUCT_FOR_ID(device)
        STRUCT_FOR_ID(dict)
        STRUCT_FOR_ID(dictcomp)
        STRUCT_FOR_ID(difference_update)
        STRUCT_FOR_ID(digest)
        STRUCT_FOR_ID(digest_size)
        STRUCT_FOR_ID(digestmod)
        STRUCT_FOR_ID(dir_fd)
        STRUCT_FOR_ID(discard)
        STRUCT_FOR_ID(dispatch_table)
        STRUCT_FOR_ID(displayhook)
        STRUCT_FOR_ID(dklen)
        STRUCT_FOR_ID(doc)
        STRUCT_FOR_ID(dont_inherit)
        STRUCT_FOR_ID(dst)
        STRUCT_FOR_ID(dst_dir_fd)
        STRUCT_FOR_ID(e)
        STRUCT_FOR_ID(eager_start)
        STRUCT_FOR_ID(effective_ids)
        STRUCT_FOR_ID(element_factory)
        STRUCT_FOR_ID(encode)
        STRUCT_FOR_ID(encoding)
        STRUCT_FOR_ID(end)
        STRUCT_FOR_ID(end_col_offset)
        STRUCT_FOR_ID(end_lineno)
        STRUCT_FOR_ID(end_offset)
        STRUCT_FOR_ID(endpos)
        STRUCT_FOR_ID(entrypoint)
        STRUCT_FOR_ID(env)
        STRUCT_FOR_ID(errors)
        STRUCT_FOR_ID(event)
        STRUCT_FOR_ID(eventmask)
        STRUCT_FOR_ID(exc_type)
        STRUCT_FOR_ID(exc_value)
        STRUCT_FOR_ID(excepthook)
        STRUCT_FOR_ID(exception)
        STRUCT_FOR_ID(existing_file_name)
        STRUCT_FOR_ID(exp)
        STRUCT_FOR_ID(extend)
        STRUCT_FOR_ID(extra_tokens)
        STRUCT_FOR_ID(f)
        STRUCT_FOR_ID(facility)
        STRUCT_FOR_ID(factory)
        STRUCT_FOR_ID(false)
        STRUCT_FOR_ID(family)
        STRUCT_FOR_ID(fanout)
        STRUCT_FOR_ID(fd)
        STRUCT_FOR_ID(fd2)
        STRUCT_FOR_ID(fdel)
        STRUCT_FOR_ID(fget)
        STRUCT_FOR_ID(file)
        STRUCT_FOR_ID(file_actions)
        STRUCT_FOR_ID(filename)
        STRUCT_FOR_ID(fileno)
        STRUCT_FOR_ID(filepath)
        STRUCT_FOR_ID(fillvalue)
        STRUCT_FOR_ID(filter)
        STRUCT_FOR_ID(filters)
        STRUCT_FOR_ID(final)
        STRUCT_FOR_ID(find_class)
        STRUCT_FOR_ID(fix_imports)
        STRUCT_FOR_ID(flags)
        STRUCT_FOR_ID(flush)
        STRUCT_FOR_ID(fold)
        STRUCT_FOR_ID(follow_symlinks)
        STRUCT_FOR_ID(format)
        STRUCT_FOR_ID(from_param)
        STRUCT_FOR_ID(fromlist)
        STRUCT_FOR_ID(fromtimestamp)
        STRUCT_FOR_ID(fromutc)
        STRUCT_FOR_ID(fset)
        STRUCT_FOR_ID(func)
        STRUCT_FOR_ID(future)
        STRUCT_FOR_ID(g)
        STRUCT_FOR_ID(generation)
        STRUCT_FOR_ID(genexpr)
        STRUCT_FOR_ID(get)
        STRUCT_FOR_ID(get_debug)
        STRUCT_FOR_ID(get_event_loop)
        STRUCT_FOR_ID(get_loop)
        STRUCT_FOR_ID(get_source)
        STRUCT_FOR_ID(getattr)
        STRUCT_FOR_ID(getstate)
        STRUCT_FOR_ID(gid)
        STRUCT_FOR_ID(globals)
        STRUCT_FOR_ID(groupindex)
        STRUCT_FOR_ID(groups)
        STRUCT_FOR_ID(h)
        STRUCT_FOR_ID(handle)
        STRUCT_FOR_ID(handle_seq)
        STRUCT_FOR_ID(hash_name)
        STRUCT_FOR_ID(header)
        STRUCT_FOR_ID(headers)
        STRUCT_FOR_ID(hi)
        STRUCT_FOR_ID(hook)
        STRUCT_FOR_ID(hour)
        STRUCT_FOR_ID(ident)
        STRUCT_FOR_ID(identity_hint)
        STRUCT_FOR_ID(ignore)
        STRUCT_FOR_ID(imag)
        STRUCT_FOR_ID(importlib)
        STRUCT_FOR_ID(in_fd)
        STRUCT_FOR_ID(incoming)
        STRUCT_FOR_ID(indexgroup)
        STRUCT_FOR_ID(inf)
        STRUCT_FOR_ID(infer_variance)
        STRUCT_FOR_ID(inherit_handle)
        STRUCT_FOR_ID(inheritable)
        STRUCT_FOR_ID(initial)
        STRUCT_FOR_ID(initial_bytes)
        STRUCT_FOR_ID(initial_owner)
        STRUCT_FOR_ID(initial_state)
        STRUCT_FOR_ID(initial_value)
        STRUCT_FOR_ID(initval)
        STRUCT_FOR_ID(inner_size)
        STRUCT_FOR_ID(input)
        STRUCT_FOR_ID(insert_comments)
        STRUCT_FOR_ID(insert_pis)
        STRUCT_FOR_ID(instructions)
        STRUCT_FOR_ID(intern)
        STRUCT_FOR_ID(intersection)
        STRUCT_FOR_ID(interval)
        STRUCT_FOR_ID(is_running)
        STRUCT_FOR_ID(isatty)
        STRUCT_FOR_ID(isinstance)
        STRUCT_FOR_ID(isoformat)
        STRUCT_FOR_ID(isolation_level)
        STRUCT_FOR_ID(istext)
        STRUCT_FOR_ID(item)
        STRUCT_FOR_ID(items)
        STRUCT_FOR_ID(iter)
        STRUCT_FOR_ID(iterable)
        STRUCT_FOR_ID(iterations)
        STRUCT_FOR_ID(join)
        STRUCT_FOR_ID(jump)
        STRUCT_FOR_ID(keepends)
        STRUCT_FOR_ID(key)
        STRUCT_FOR_ID(keyfile)
        STRUCT_FOR_ID(keys)
        STRUCT_FOR_ID(kind)
        STRUCT_FOR_ID(kw)
        STRUCT_FOR_ID(kw1)
        STRUCT_FOR_ID(kw2)
        STRUCT_FOR_ID(kwdefaults)
        STRUCT_FOR_ID(label)
        STRUCT_FOR_ID(lambda)
        STRUCT_FOR_ID(last)
        STRUCT_FOR_ID(last_exc)
        STRUCT_FOR_ID(last_node)
        STRUCT_FOR_ID(last_traceback)
        STRUCT_FOR_ID(last_type)
        STRUCT_FOR_ID(last_value)
        STRUCT_FOR_ID(latin1)
        STRUCT_FOR_ID(leaf_size)
        STRUCT_FOR_ID(len)
        STRUCT_FOR_ID(length)
        STRUCT_FOR_ID(level)
        STRUCT_FOR_ID(limit)
        STRUCT_FOR_ID(line)
        STRUCT_FOR_ID(line_buffering)
        STRUCT_FOR_ID(lineno)
        STRUCT_FOR_ID(listcomp)
        STRUCT_FOR_ID(little)
        STRUCT_FOR_ID(lo)
        STRUCT_FOR_ID(locale)
        STRUCT_FOR_ID(locals)
        STRUCT_FOR_ID(logoption)
        STRUCT_FOR_ID(loop)
        STRUCT_FOR_ID(manual_reset)
        STRUCT_FOR_ID(mapping)
        STRUCT_FOR_ID(match)
        STRUCT_FOR_ID(max_length)
        STRUCT_FOR_ID(maxdigits)
        STRUCT_FOR_ID(maxevents)
        STRUCT_FOR_ID(maxlen)
        STRUCT_FOR_ID(maxmem)
        STRUCT_FOR_ID(maxsplit)
        STRUCT_FOR_ID(maxvalue)
        STRUCT_FOR_ID(memLevel)
        STRUCT_FOR_ID(memlimit)
        STRUCT_FOR_ID(message)
        STRUCT_FOR_ID(metaclass)
        STRUCT_FOR_ID(metadata)
        STRUCT_FOR_ID(method)
        STRUCT_FOR_ID(microsecond)
        STRUCT_FOR_ID(milliseconds)
        STRUCT_FOR_ID(minute)
        STRUCT_FOR_ID(mod)
        STRUCT_FOR_ID(mode)
        STRUCT_FOR_ID(module)
        STRUCT_FOR_ID(module_globals)
        STRUCT_FOR_ID(modules)
        STRUCT_FOR_ID(month)
        STRUCT_FOR_ID(mro)
        STRUCT_FOR_ID(msg)
        STRUCT_FOR_ID(mutex)
        STRUCT_FOR_ID(mycmp)
        STRUCT_FOR_ID(n)
        STRUCT_FOR_ID(n_arg)
        STRUCT_FOR_ID(n_fields)
        STRUCT_FOR_ID(n_sequence_fields)
        STRUCT_FOR_ID(n_unnamed_fields)
        STRUCT_FOR_ID(name)
        STRUCT_FOR_ID(name_from)
        STRUCT_FOR_ID(namespace_separator)
        STRUCT_FOR_ID(namespaces)
        STRUCT_FOR_ID(narg)
        STRUCT_FOR_ID(ndigits)
        STRUCT_FOR_ID(nested)
        STRUCT_FOR_ID(new_file_name)
        STRUCT_FOR_ID(new_limit)
        STRUCT_FOR_ID(newline)
        STRUCT_FOR_ID(newlines)
        STRUCT_FOR_ID(next)
        STRUCT_FOR_ID(nlocals)
        STRUCT_FOR_ID(node_depth)
        STRUCT_FOR_ID(node_offset)
        STRUCT_FOR_ID(ns)
        STRUCT_FOR_ID(nstype)
        STRUCT_FOR_ID(nt)
        STRUCT_FOR_ID(null)
        STRUCT_FOR_ID(number)
        STRUCT_FOR_ID(obj)
        STRUCT_FOR_ID(object)
        STRUCT_FOR_ID(offset)
        STRUCT_FOR_ID(offset_dst)
        STRUCT_FOR_ID(offset_src)
        STRUCT_FOR_ID(on_type_read)
        STRUCT_FOR_ID(onceregistry)
        STRUCT_FOR_ID(only_keys)
        STRUCT_FOR_ID(oparg)
        STRUCT_FOR_ID(opcode)
        STRUCT_FOR_ID(open)
        STRUCT_FOR_ID(opener)
        STRUCT_FOR_ID(operation)
        STRUCT_FOR_ID(optimize)
        STRUCT_FOR_ID(options)
        STRUCT_FOR_ID(order)
        STRUCT_FOR_ID(origin)
        STRUCT_FOR_ID(out_fd)
        STRUCT_FOR_ID(outgoing)
        STRUCT_FOR_ID(overlapped)
        STRUCT_FOR_ID(owner)
        STRUCT_FOR_ID(p)
        STRUCT_FOR_ID(pages)
        STRUCT_FOR_ID(parent)
        STRUCT_FOR_ID(password)
        STRUCT_FOR_ID(path)
        STRUCT_FOR_ID(pattern)
        STRUCT_FOR_ID(peek)
        STRUCT_FOR_ID(persistent_id)
        STRUCT_FOR_ID(persistent_load)
        STRUCT_FOR_ID(person)
        STRUCT_FOR_ID(pi_factory)
        STRUCT_FOR_ID(pid)
        STRUCT_FOR_ID(policy)
        STRUCT_FOR_ID(pos)
        STRUCT_FOR_ID(pos1)
        STRUCT_FOR_ID(pos2)
        STRUCT_FOR_ID(posix)
        STRUCT_FOR_ID(print_file_and_line)
        STRUCT_FOR_ID(priority)
        STRUCT_FOR_ID(progress)
        STRUCT_FOR_ID(progress_handler)
        STRUCT_FOR_ID(progress_routine)
        STRUCT_FOR_ID(proto)
        STRUCT_FOR_ID(protocol)
        STRUCT_FOR_ID(ps1)
        STRUCT_FOR_ID(ps2)
        STRUCT_FOR_ID(query)
        STRUCT_FOR_ID(quotetabs)
        STRUCT_FOR_ID(r)
        STRUCT_FOR_ID(raw)
        STRUCT_FOR_ID(read)
        STRUCT_FOR_ID(read1)
        STRUCT_FOR_ID(readable)
        STRUCT_FOR_ID(readall)
        STRUCT_FOR_ID(readinto)
        STRUCT_FOR_ID(readinto1)
        STRUCT_FOR_ID(readline)
        STRUCT_FOR_ID(readonly)
        STRUCT_FOR_ID(real)
        STRUCT_FOR_ID(reducer_override)
        STRUCT_FOR_ID(registry)
        STRUCT_FOR_ID(rel_tol)
        STRUCT_FOR_ID(release)
        STRUCT_FOR_ID(reload)
        STRUCT_FOR_ID(repl)
        STRUCT_FOR_ID(replace)
        STRUCT_FOR_ID(reserved)
        STRUCT_FOR_ID(reset)
        STRUCT_FOR_ID(resetids)
        STRUCT_FOR_ID(return)
        STRUCT_FOR_ID(reverse)
        STRUCT_FOR_ID(reversed)
        STRUCT_FOR_ID(s)
        STRUCT_FOR_ID(salt)
        STRUCT_FOR_ID(sched_priority)
        STRUCT_FOR_ID(scheduler)
        STRUCT_FOR_ID(second)
        STRUCT_FOR_ID(security_attributes)
        STRUCT_FOR_ID(seek)
        STRUCT_FOR_ID(seekable)
        STRUCT_FOR_ID(selectors)
        STRUCT_FOR_ID(self)
        STRUCT_FOR_ID(send)
        STRUCT_FOR_ID(sep)
        STRUCT_FOR_ID(sequence)
        STRUCT_FOR_ID(server_hostname)
        STRUCT_FOR_ID(server_side)
        STRUCT_FOR_ID(session)
        STRUCT_FOR_ID(setcomp)
        STRUCT_FOR_ID(setpgroup)
        STRUCT_FOR_ID(setsid)
        STRUCT_FOR_ID(setsigdef)
        STRUCT_FOR_ID(setsigmask)
        STRUCT_FOR_ID(setstate)
        STRUCT_FOR_ID(shape)
        STRUCT_FOR_ID(show_cmd)
        STRUCT_FOR_ID(signed)
        STRUCT_FOR_ID(size)
        STRUCT_FOR_ID(sizehint)
        STRUCT_FOR_ID(skip_file_prefixes)
        STRUCT_FOR_ID(sleep)
        STRUCT_FOR_ID(sock)
        STRUCT_FOR_ID(sort)
        STRUCT_FOR_ID(source)
        STRUCT_FOR_ID(source_traceback)
        STRUCT_FOR_ID(src)
        STRUCT_FOR_ID(src_dir_fd)
        STRUCT_FOR_ID(stacklevel)
        STRUCT_FOR_ID(start)
        STRUCT_FOR_ID(statement)
        STRUCT_FOR_ID(status)
        STRUCT_FOR_ID(stderr)
        STRUCT_FOR_ID(stdin)
        STRUCT_FOR_ID(stdout)
        STRUCT_FOR_ID(step)
        STRUCT_FOR_ID(steps)
        STRUCT_FOR_ID(store_name)
        STRUCT_FOR_ID(strategy)
        STRUCT_FOR_ID(strftime)
        STRUCT_FOR_ID(strict)
        STRUCT_FOR_ID(strict_mode)
        STRUCT_FOR_ID(string)
        STRUCT_FOR_ID(sub_key)
        STRUCT_FOR_ID(symmetric_difference_update)
        STRUCT_FOR_ID(tabsize)
        STRUCT_FOR_ID(tag)
        STRUCT_FOR_ID(target)
        STRUCT_FOR_ID(target_is_directory)
        STRUCT_FOR_ID(task)
        STRUCT_FOR_ID(tb_frame)
        STRUCT_FOR_ID(tb_lasti)
        STRUCT_FOR_ID(tb_lineno)
        STRUCT_FOR_ID(tb_next)
        STRUCT_FOR_ID(tell)
        STRUCT_FOR_ID(template)
        STRUCT_FOR_ID(term)
        STRUCT_FOR_ID(text)
        STRUCT_FOR_ID(threading)
        STRUCT_FOR_ID(throw)
        STRUCT_FOR_ID(timeout)
        STRUCT_FOR_ID(times)
        STRUCT_FOR_ID(timetuple)
        STRUCT_FOR_ID(top)
        STRUCT_FOR_ID(trace_callback)
        STRUCT_FOR_ID(traceback)
        STRUCT_FOR_ID(trailers)
        STRUCT_FOR_ID(translate)
        STRUCT_FOR_ID(true)
        STRUCT_FOR_ID(truncate)
        STRUCT_FOR_ID(twice)
        STRUCT_FOR_ID(txt)
        STRUCT_FOR_ID(type)
        STRUCT_FOR_ID(type_params)
        STRUCT_FOR_ID(tz)
        STRUCT_FOR_ID(tzinfo)
        STRUCT_FOR_ID(tzname)
        STRUCT_FOR_ID(uid)
        STRUCT_FOR_ID(unlink)
        STRUCT_FOR_ID(unraisablehook)
        STRUCT_FOR_ID(uri)
        STRUCT_FOR_ID(usedforsecurity)
        STRUCT_FOR_ID(value)
        STRUCT_FOR_ID(values)
        STRUCT_FOR_ID(version)
        STRUCT_FOR_ID(volume)
        STRUCT_FOR_ID(wait_all)
        STRUCT_FOR_ID(warn_on_full_buffer)
        STRUCT_FOR_ID(warnings)
        STRUCT_FOR_ID(warnoptions)
        STRUCT_FOR_ID(wbits)
        STRUCT_FOR_ID(week)
        STRUCT_FOR_ID(weekday)
        STRUCT_FOR_ID(which)
        STRUCT_FOR_ID(who)
        STRUCT_FOR_ID(withdata)
        STRUCT_FOR_ID(writable)
        STRUCT_FOR_ID(write)
        STRUCT_FOR_ID(write_through)
        STRUCT_FOR_ID(x)
        STRUCT_FOR_ID(year)
        STRUCT_FOR_ID(zdict)
    } identifiers;
    struct {
        PyASCIIObject _ascii;
        uint8_t _data[2];
    } ascii[128];
    struct {
        PyCompactUnicodeObject _latin1;
        uint8_t _data[2];
    } latin1[128];
};
/* End auto-generated code */

#undef ID
#undef STR


#define _Py_ID(NAME) \
     (_Py_SINGLETON(strings.identifiers._py_ ## NAME._ascii.ob_base))
#define _Py_STR(NAME) \
     (_Py_SINGLETON(strings.literals._py_ ## NAME._ascii.ob_base))

/* _Py_DECLARE_STR() should precede all uses of _Py_STR() in a function.

   This is true even if the same string has already been declared
   elsewhere, even in the same file.  Mismatched duplicates are detected
   by Tools/scripts/generate-global-objects.py.

   Pairing _Py_DECLARE_STR() with every use of _Py_STR() makes sure the
   string keeps working even if the declaration is removed somewhere
   else.  It also makes it clear what the actual string is at every
   place it is being used. */
#define _Py_DECLARE_STR(name, str)

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_GLOBAL_STRINGS_H */
