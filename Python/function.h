/********************************
 ********Common Functions********
 ********************************/
typedef PyObject * (*binaryfunc)(PyObject *, PyObject *);
typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
typedef PyObject * (*unaryfunc)(PyObject *);
typedef int (*inquiry)(PyObject *);
typedef int (*coercion)(PyObject **, PyObject **);

/*******************************
 *****_typeobject Functions*****
 *******************************/
typedef void (*freefunc)(void *);
typedef void (*destructor)(PyObject *);
typedef int (*printfunc)(PyObject *, FILE *, int);
typedef PyObject *(*getattrfunc)(PyObject *, char *);
typedef PyObject *(*getattrofunc)(PyObject *, PyObject *);
typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
typedef int (*cmpfunc)(PyObject *, PyObject *);
typedef PyObject *(*reprfunc)(PyObject *);
typedef long (*hashfunc)(PyObject *);
typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
typedef PyObject *(*getiterfunc) (PyObject *);
typedef PyObject *(*iternextfunc) (PyObject *);
typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*newfunc)(struct _typeobject *, PyObject *, PyObject *);
typedef PyObject *(*allocfunc)(struct _typeobject *, Py_ssize_t);

/********************************
 *******PyObject Functions*******
 ********************************/
PyObject * PyObject_Init(PyObject *op, PyTypeObject *tp) {
	if (op == NULL)
		return PyErr_NoMemory();
	/* Any changes should be reflected in PyObject_INIT (objimpl.h) */
	op->ob_type = tp;
	_Py_NewReference(op);
	return op;
}

PyObject * PyObject_GenericGetAttr(PyObject *obj, PyObject *name) {
	PyTypeObject *tp = obj->ob_type;
	PyObject *descr = NULL;
	PyObject *res = NULL;
	descrgetfunc f;
	Py_ssize_t dictoffset;
	PyObject **dictptr;

	if (!PyString_Check(name)){
#ifdef Py_USING_UNICODE
		/* The Unicode to string conversion is done here because the
		   existing tp_setattro slots expect a string object as name
		   and we wouldn't want to break those. */
		if (PyUnicode_Check(name)) {
			name = PyUnicode_AsEncodedString(name, NULL, NULL);
			if (name == NULL)
				return NULL;
		}
		else
#endif
		{
			PyErr_Format(PyExc_TypeError,
				     "attribute name must be string, not '%.200s'",
				     name->ob_type->tp_name);
			return NULL;
		}
	}
	else
		Py_INCREF(name);

	if (tp->tp_dict == NULL) {
		if (PyType_Ready(tp) < 0)
			goto done;
	}

	/* Inline _PyType_Lookup */
	{
		Py_ssize_t i, n;
		PyObject *mro, *base, *dict;

		/* Look in tp_dict of types in MRO */
		mro = tp->tp_mro;
		assert(mro != NULL);
		assert(PyTuple_Check(mro));
		n = PyTuple_GET_SIZE(mro);
		for (i = 0; i < n; i++) {
			base = PyTuple_GET_ITEM(mro, i);
			if (PyClass_Check(base))
				dict = ((PyClassObject *)base)->cl_dict;
			else {
				assert(PyType_Check(base));
				dict = ((PyTypeObject *)base)->tp_dict;
			}
			assert(dict && PyDict_Check(dict));
			descr = PyDict_GetItem(dict, name);
			if (descr != NULL)
				break;
		}
	}

	Py_XINCREF(descr);

	f = NULL;
	if (descr != NULL &&
	    PyType_HasFeature(descr->ob_type, Py_TPFLAGS_HAVE_CLASS)) {
		f = descr->ob_type->tp_descr_get;
		if (f != NULL && PyDescr_IsData(descr)) {
			res = f(descr, obj, (PyObject *)obj->ob_type);
			Py_DECREF(descr);
			goto done;
		}
	}

	/* Inline _PyObject_GetDictPtr */
	dictoffset = tp->tp_dictoffset;
	if (dictoffset != 0) {
		PyObject *dict;
		if (dictoffset < 0) {
			Py_ssize_t tsize;
			size_t size;

			tsize = ((PyVarObject *)obj)->ob_size;
			if (tsize < 0)
				tsize = -tsize;
			size = _PyObject_VAR_SIZE(tp, tsize);

			dictoffset += (long)size;
			assert(dictoffset > 0);
			assert(dictoffset % SIZEOF_VOID_P == 0);
		}
		dictptr = (PyObject **) ((char *)obj + dictoffset);
		dict = *dictptr;
		if (dict != NULL) {
			res = PyDict_GetItem(dict, name);
			if (res != NULL) {
				Py_INCREF(res);
				Py_XDECREF(descr);
				goto done;
			}
		}
	}

	if (f != NULL) {
		res = f(descr, obj, (PyObject *)obj->ob_type);
		Py_DECREF(descr);
		goto done;
	}

	if (descr != NULL) {
		res = descr;
		/* descr was already increfed above */
		goto done;
	}

	PyErr_Format(PyExc_AttributeError,
		     "'%.50s' object has no attribute '%.400s'",
		     tp->tp_name, PyString_AS_STRING(name));
  done:
	Py_DECREF(name);
	return res;
}

long PyObject_Hash(PyObject *v) {
	PyTypeObject *tp = v->ob_type;
	if (tp->tp_hash != NULL)
		return (*tp->tp_hash)(v);
	if (tp->tp_compare == NULL && RICHCOMPARE(tp) == NULL) {
		return _Py_HashPointer(v); /* Use address as hash value */
	}
	/* If there's a cmp but no hash defined, the object can't be hashed */
	PyErr_Format(PyExc_TypeError, "unhashable type: '%.200s'",
		     v->ob_type->tp_name);
	return -1;
}

static int internal_print(PyObject *op, FILE *fp, int flags, int nesting) {
	int ret = 0;
	if (nesting > 10) {
		PyErr_SetString(PyExc_RuntimeError, "print recursion");
		return -1;
	}
	if (PyErr_CheckSignals())
		return -1;
#ifdef USE_STACKCHECK
	if (PyOS_CheckStack()) {
		PyErr_SetString(PyExc_MemoryError, "stack overflow");
		return -1;
	}
#endif
	clearerr(fp); /* Clear any previous error condition */
	if (op == NULL) {
		fprintf(fp, "<nil>");
	}
	else {
		if (op->ob_refcnt <= 0)
			/* XXX(twouters) cast refcount to long until %zd is
			   universally available */
			fprintf(fp, "<refcnt %ld at %p>",
				(long)op->ob_refcnt, op);
		else if (op->ob_type->tp_print == NULL) {
			PyObject *s;
			if (flags & Py_PRINT_RAW)
				s = PyObject_Str(op);
			else
				s = PyObject_Repr(op);
			if (s == NULL)
				ret = -1;
			else {
				ret = internal_print(s, fp, Py_PRINT_RAW,
						     nesting+1);
			}
			Py_XDECREF(s);
		}
		else
			ret = (*op->ob_type->tp_print)(op, fp, flags);
	}
	if (ret == 0) {
		if (ferror(fp)) {
			PyErr_SetFromErrno(PyExc_IOError);
			clearerr(fp);
			ret = -1;
		}
	}
	return ret;
}

int PyObject_Print(PyObject *op, FILE *fp, int flags) {
	return internal_print(op, fp, flags, 0);
}

/*********************************
 ******PyIntObject Functions******
 *********************************/
PyObject * PyInt_FromLong(long ival) {
	register PyIntObject *v;
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
	if (-NSMALLNEGINTS <= ival && ival < NSMALLPOSINTS) {
		v = small_ints[ival + NSMALLNEGINTS];
		Py_INCREF(v);
#ifdef COUNT_ALLOCS
		if (ival >= 0)
			quick_int_allocs++;
		else
			quick_neg_int_allocs++;
#endif
		return (PyObject *) v;
	}
#endif
	if (free_list == NULL) {
		if ((free_list = fill_free_list()) == NULL)
			return NULL;
	}
	/* Inline PyObject_New */
	v = free_list;
	free_list = (PyIntObject *)v->ob_type;
	PyObject_INIT(v, &PyInt_Type);
	v->ob_ival = ival;
	return (PyObject *) v;
}

PyObject * PyInt_FromString(char *s, char **pend, int base) {
	char *end;
	long x;
	Py_ssize_t slen;
	PyObject *sobj, *srepr;

	if ((base != 0 && base < 2) || base > 36) {
		PyErr_SetString(PyExc_ValueError,
				"int() base must be >= 2 and <= 36");
		return NULL;
	}

	while (*s && isspace(Py_CHARMASK(*s)))
		s++;
	errno = 0;
	if (base == 0 && s[0] == '0') {
		x = (long) PyOS_strtoul(s, &end, base);
		if (x < 0)
			return PyLong_FromString(s, pend, base);
	}
	else
		x = PyOS_strtol(s, &end, base);
	if (end == s || !isalnum(Py_CHARMASK(end[-1])))
		goto bad;
	while (*end && isspace(Py_CHARMASK(*end)))
		end++;
	if (*end != '\0') {
  bad:
		slen = strlen(s) < 200 ? strlen(s) : 200;
		sobj = PyString_FromStringAndSize(s, slen);
		if (sobj == NULL)
			return NULL;
		srepr = PyObject_Repr(sobj);
		Py_DECREF(sobj);
		if (srepr == NULL)
			return NULL;
		PyErr_Format(PyExc_ValueError,
			     "invalid literal for int() with base %d: %s",
			     base, PyString_AS_STRING(srepr));
		Py_DECREF(srepr);
		return NULL;
	}
	else if (errno != 0)
		return PyLong_FromString(s, pend, base);
	if (pend)
		*pend = end;
	return PyInt_FromLong(x);
}

#ifdef Py_USING_UNICODE
PyObject * PyInt_FromUnicode(Py_UNICODE *s, Py_ssize_t length, int base) {
	PyObject *result;
	char *buffer = (char *)PyMem_MALLOC(length+1);

	if (buffer == NULL)
		return NULL;

	if (PyUnicode_EncodeDecimal(s, length, buffer, NULL)) {
		PyMem_FREE(buffer);
		return NULL;
	}
	result = PyInt_FromString(buffer, NULL, base);
	PyMem_FREE(buffer);
	return result;
}
#endif

static PyIntObject * fill_free_list(void) {
	PyIntObject *p, *q;
	/* Python's object allocator isn't appropriate for large blocks. */
	p = (PyIntObject *) PyMem_MALLOC(sizeof(PyIntBlock));
	if (p == NULL)
		return (PyIntObject *) PyErr_NoMemory();
	((PyIntBlock *)p)->next = block_list;
	block_list = (PyIntBlock *)p;
	/* Link the int objects together, from rear to front, then return
	   the address of the last int object in the block. */
	p = &((PyIntBlock *)p)->objects[0];
	q = p + N_INTOBJECTS;
	while (--q > p)
		q->ob_type = (struct _typeobject *)(q-1);
	q->ob_type = NULL;
	return p + N_INTOBJECTS - 1;
}

int _PyInt_Init(void) {
	PyIntObject *v;
	int ival;
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
	for (ival = -NSMALLNEGINTS; ival < NSMALLPOSINTS; ival++) {
              if (!free_list && (free_list = fill_free_list()) == NULL)
			return 0;
		/* PyObject_New is inlined */
		v = free_list;
		free_list = (PyIntObject *)v->ob_type;
		PyObject_INIT(v, &PyInt_Type);
		v->ob_ival = ival;
		small_ints[ival + NSMALLNEGINTS] = v;
	}
#endif
	return 1;
}

/********************************
 ****PyStringObject Functions****
 ********************************/
PyObject * PyString_FromString(const char *str) {
	register size_t size;
	register PyStringObject *op;

	assert(str != NULL);
	size = strlen(str);
	if (size > PY_SSIZE_T_MAX) {
		PyErr_SetString(PyExc_OverflowError,
			"string is too long for a Python string");
		return NULL;
	}
	if (size == 0 && (op = nullstring) != NULL) {
#ifdef COUNT_ALLOCS
		null_strings++;
#endif
		Py_INCREF(op);
		return (PyObject *)op;
	}
	if (size == 1 && (op = characters[*str & UCHAR_MAX]) != NULL) {
#ifdef COUNT_ALLOCS
		one_strings++;
#endif
		Py_INCREF(op);
		return (PyObject *)op;
	}

	/* Inline PyObject_NewVar */
	op = (PyStringObject *)PyObject_MALLOC(sizeof(PyStringObject) + size);
	if (op == NULL)
		return PyErr_NoMemory();
	PyObject_INIT_VAR(op, &PyString_Type, size);
	op->ob_shash = -1;
	op->ob_sstate = SSTATE_NOT_INTERNED;
	Py_MEMCPY(op->ob_sval, str, size+1);
	/* share short strings */
	if (size == 0) {
		PyObject *t = (PyObject *)op;
		PyString_InternInPlace(&t);
		op = (PyStringObject *)t;
		nullstring = op;
		Py_INCREF(op);
	} else if (size == 1) {
		PyObject *t = (PyObject *)op;
		PyString_InternInPlace(&t);
		op = (PyStringObject *)t;
		characters[*str & UCHAR_MAX] = op;
		Py_INCREF(op);
	}
	return (PyObject *) op;
}

PyObject *
PyString_FromStringAndSize(const char *str, Py_ssize_t size)
{
	register PyStringObject *op;
	assert(size >= 0);
	if (size == 0 && (op = nullstring) != NULL) {
#ifdef COUNT_ALLOCS
		null_strings++;
#endif
		Py_INCREF(op);
		return (PyObject *)op;
	}
	if (size == 1 && str != NULL &&
	    (op = characters[*str & UCHAR_MAX]) != NULL)
	{
#ifdef COUNT_ALLOCS
		one_strings++;
#endif
		Py_INCREF(op);
		return (PyObject *)op;
	}

	/* Inline PyObject_NewVar */
	op = (PyStringObject *)PyObject_MALLOC(sizeof(PyStringObject) + size);
	if (op == NULL)
		return PyErr_NoMemory();
	PyObject_INIT_VAR(op, &PyString_Type, size);
	op->ob_shash = -1;
	op->ob_sstate = SSTATE_NOT_INTERNED;
	if (str != NULL)
		Py_MEMCPY(op->ob_sval, str, size);
	op->ob_sval[size] = '\0';
	/* share short strings */
	if (size == 0) {
		PyObject *t = (PyObject *)op;
		PyString_InternInPlace(&t);
		op = (PyStringObject *)t;
		nullstring = op;
		Py_INCREF(op);
	} else if (size == 1 && str != NULL) {
		PyObject *t = (PyObject *)op;
		PyString_InternInPlace(&t);
		op = (PyStringObject *)t;
		characters[*str & UCHAR_MAX] = op;
		Py_INCREF(op);
	}
	return (PyObject *) op;
}

void PyString_InternInPlace(PyObject **p) {
	register PyStringObject *s = (PyStringObject *)(*p);
	PyObject *t;
	if (s == NULL || !PyString_Check(s))
		Py_FatalError("PyString_InternInPlace: strings only please!");
	/* If it's a string subclass, we don't really know what putting
	   it in the interned dict might do. */
	if (!PyString_CheckExact(s))
		return;
	if (PyString_CHECK_INTERNED(s))
		return;
	if (interned == NULL) {
		interned = PyDict_New();
		if (interned == NULL) {
			PyErr_Clear(); /* Don't leave an exception */
			return;
		}
	}
	t = PyDict_GetItem(interned, (PyObject *)s);
	if (t) {
		Py_INCREF(t);
		Py_DECREF(*p);
		*p = t;
		return;
	}

	if (PyDict_SetItem(interned, (PyObject *)s, (PyObject *)s) < 0) {
		PyErr_Clear();
		return;
	}
	/* The two references in interned are not counted by refcnt.
	   The string deallocator will take care of this */
	s->ob_refcnt -= 2;
	PyString_CHECK_INTERNED(s) = SSTATE_INTERNED_MORTAL;
}

PyObject * PyString_InternFromString(const char *cp) {
	PyObject *s = PyString_FromString(cp);
	if (s == NULL)
		return NULL;
	PyString_InternInPlace(&s);
	return s;
}

void PyString_InternImmortal(PyObject **p) {
	PyString_InternInPlace(p);
	if (PyString_CHECK_INTERNED(*p) != SSTATE_INTERNED_IMMORTAL) {
		PyString_CHECK_INTERNED(*p) = SSTATE_INTERNED_IMMORTAL;
		Py_INCREF(*p);
	}
}