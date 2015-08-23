static void string_dealloc(PyObject *op) {
	switch (PyString_CHECK_INTERNED(op)) {
		case SSTATE_NOT_INTERNED:
			break;

		case SSTATE_INTERNED_MORTAL:
			/* revive dead object temporarily for DelItem */
			op->ob_refcnt = 3;
			if (PyDict_DelItem(interned, op) != 0)
				Py_FatalError(
					"deletion of interned string failed");
			break;

		case SSTATE_INTERNED_IMMORTAL:
			Py_FatalError("Immortal interned string died.");

		default:
			Py_FatalError("Inconsistent interned string state.");
	}
	op->ob_type->tp_free(op);
}

static int string_print(PyStringObject *op, FILE *fp, int flags) {
	Py_ssize_t i;
	char c;
	int quote;

	/* XXX Ought to check for interrupts when writing long strings */
	if (! PyString_CheckExact(op)) {
		int ret;
		/* A str subclass may have its own __str__ method. */
		op = (PyStringObject *) PyObject_Str((PyObject *)op);
		if (op == NULL)
			return -1;
		ret = string_print(op, fp, flags);
		Py_DECREF(op);
		return ret;
	}
	if (flags & Py_PRINT_RAW) {
#ifdef __VMS
                if (op->ob_size) fwrite(op->ob_sval, (int) op->ob_size, 1, fp);
#else
                fwrite(op->ob_sval, 1, (int) op->ob_size, fp);
#endif
		return 0;
	}

	/* figure out which quote to use; single is preferred */
	quote = '\'';
	if (memchr(op->ob_sval, '\'', op->ob_size) &&
	    !memchr(op->ob_sval, '"', op->ob_size))
		quote = '"';

	fputc(quote, fp);
	for (i = 0; i < op->ob_size; i++) {
		c = op->ob_sval[i];
		if (c == quote || c == '\\')
			fprintf(fp, "\\%c", c);
                else if (c == '\t')
                        fprintf(fp, "\\t");
                else if (c == '\n')
                        fprintf(fp, "\\n");
                else if (c == '\r')
                        fprintf(fp, "\\r");
		else if (c < ' ' || c >= 0x7f)
			fprintf(fp, "\\x%02x", c & 0xff);
		else
			fputc(c, fp);
	}
	fputc(quote, fp);
	return 0;
}

PyObject * PyString_Repr(PyObject *obj, int smartquotes) {
	register PyStringObject* op = (PyStringObject*) obj;
	size_t newsize = 2 + 4 * op->ob_size;
	PyObject *v;
	if (newsize > PY_SSIZE_T_MAX) {
		PyErr_SetString(PyExc_OverflowError,
			"string is too large to make repr");
	}
	v = PyString_FromStringAndSize((char *)NULL, newsize);
	if (v == NULL) {
		return NULL;
	}
	else {
		register Py_ssize_t i;
		register char c;
		register char *p;
		int quote;

		/* figure out which quote to use; single is preferred */
		quote = '\'';
		if (smartquotes &&
		    memchr(op->ob_sval, '\'', op->ob_size) &&
		    !memchr(op->ob_sval, '"', op->ob_size))
			quote = '"';

		p = PyString_AS_STRING(v);
		*p++ = quote;
		for (i = 0; i < op->ob_size; i++) {
			/* There's at least enough room for a hex escape
			   and a closing quote. */
			assert(newsize - (p - PyString_AS_STRING(v)) >= 5);
			c = op->ob_sval[i];
			if (c == quote || c == '\\')
				*p++ = '\\', *p++ = c;
			else if (c == '\t')
				*p++ = '\\', *p++ = 't';
			else if (c == '\n')
				*p++ = '\\', *p++ = 'n';
			else if (c == '\r')
				*p++ = '\\', *p++ = 'r';
			else if (c < ' ' || c >= 0x7f) {
				/* For performance, we don't want to call
				   PyOS_snprintf here (extra layers of
				   function call). */
				sprintf(p, "\\x%02x", c & 0xff);
                                p += 4;
			}
			else
				*p++ = c;
		}
		assert(newsize - (p - PyString_AS_STRING(v)) >= 1);
		*p++ = quote;
		*p = '\0';
		_PyString_Resize(
			&v, (p - PyString_AS_STRING(v)));
		return v;
	}
}

static PyObject * string_repr(PyObject *op) {
	return PyString_Repr(op, 1);
}

static long string_hash(PyStringObject *a) {
	register Py_ssize_t len;
	register unsigned char *p;
	register long x;

	if (a->ob_shash != -1)
		return a->ob_shash;
	len = a->ob_size;
	p = (unsigned char *) a->ob_sval;
	x = *p << 7;
	while (--len >= 0)
		x = (1000003*x) ^ *p++;
	x ^= a->ob_size;
	if (x == -1)
		x = -2;
	a->ob_shash = x;
	return x;
}

static PyObject * string_str(PyObject *s) {
	assert(PyString_Check(s));
	if (PyString_CheckExact(s)) {
		Py_INCREF(s);
		return s;
	}
	else {
		/* Subtype -- return genuine string with the same value. */
		PyStringObject *t = (PyStringObject *) s;
		return PyString_FromStringAndSize(t->ob_sval, t->ob_size);
	}
}

static PyObject* string_richcompare(PyStringObject *a, PyStringObject *b, int op) {
	int c;
	Py_ssize_t len_a, len_b;
	Py_ssize_t min_len;
	PyObject *result;

	/* Make sure both arguments are strings. */
	if (!(PyString_Check(a) && PyString_Check(b))) {
		result = Py_NotImplemented;
		goto out;
	}
	if (a == b) {
		switch (op) {
		case Py_EQ:case Py_LE:case Py_GE:
			result = Py_True;
			goto out;
		case Py_NE:case Py_LT:case Py_GT:
			result = Py_False;
			goto out;
		}
	}
	if (op == Py_EQ) {
		/* Supporting Py_NE here as well does not save
		   much time, since Py_NE is rarely used.  */
		if (a->ob_size == b->ob_size
		    && (a->ob_sval[0] == b->ob_sval[0]
			&& memcmp(a->ob_sval, b->ob_sval,
				  a->ob_size) == 0)) {
			result = Py_True;
		} else {
			result = Py_False;
		}
		goto out;
	}
	len_a = a->ob_size; len_b = b->ob_size;
	min_len = (len_a < len_b) ? len_a : len_b;
	if (min_len > 0) {
		c = Py_CHARMASK(*a->ob_sval) - Py_CHARMASK(*b->ob_sval);
		if (c==0)
			c = memcmp(a->ob_sval, b->ob_sval, min_len);
	}else
		c = 0;
	if (c == 0)
		c = (len_a < len_b) ? -1 : (len_a > len_b) ? 1 : 0;
	switch (op) {
	case Py_LT: c = c <  0; break;
	case Py_LE: c = c <= 0; break;
	case Py_EQ: assert(0);  break; /* unreachable */
	case Py_NE: c = c != 0; break;
	case Py_GT: c = c >  0; break;
	case Py_GE: c = c >= 0; break;
	default:
		result = Py_NotImplemented;
		goto out;
	}
	result = c ? Py_True : Py_False;
  out:
	Py_INCREF(result);
	return result;
}

static PyObject * string_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	PyObject *x = NULL;
	static char *kwlist[] = {"object", 0};

	if (type != &PyString_Type)
		return str_subtype_new(type, args, kwds);
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O:str", kwlist, &x))
		return NULL;
	if (x == NULL)
		return PyString_FromString("");
	return PyObject_Str(x);
}

static PyObject * string_concat(register PyStringObject *a, register PyObject *bb) {
	register Py_ssize_t size;
	register PyStringObject *op;
	if (!PyString_Check(bb)) {
#ifdef Py_USING_UNICODE
		if (PyUnicode_Check(bb))
		    return PyUnicode_Concat((PyObject *)a, bb);
#endif
		PyErr_Format(PyExc_TypeError,
			     "cannot concatenate 'str' and '%.200s' objects",
			     bb->ob_type->tp_name);
		return NULL;
	}
#define b ((PyStringObject *)bb)
	/* Optimize cases with empty left or right operand */
	if ((a->ob_size == 0 || b->ob_size == 0) &&
	    PyString_CheckExact(a) && PyString_CheckExact(b)) {
		if (a->ob_size == 0) {
			Py_INCREF(bb);
			return bb;
		}
		Py_INCREF(a);
		return (PyObject *)a;
	}
	size = a->ob_size + b->ob_size;
	if (size < 0) {
		PyErr_SetString(PyExc_OverflowError,
				"strings are too large to concat");
		return NULL;
	}
	  
	/* Inline PyObject_NewVar */
	op = (PyStringObject *)PyObject_MALLOC(sizeof(PyStringObject) + size);
	if (op == NULL)
		return PyErr_NoMemory();
	PyObject_INIT_VAR(op, &PyString_Type, size);
	op->ob_shash = -1;
	op->ob_sstate = SSTATE_NOT_INTERNED;
	Py_MEMCPY(op->ob_sval, a->ob_sval, a->ob_size);
	Py_MEMCPY(op->ob_sval + a->ob_size, b->ob_sval, b->ob_size);
	op->ob_sval[size] = '\0';
	return (PyObject *) op;
#undef b
}

static PyObject * string_join(PyStringObject *self, PyObject *orig) {
	char *sep = PyString_AS_STRING(self);
	const Py_ssize_t seplen = PyString_GET_SIZE(self);
	PyObject *res = NULL;
	char *p;
	Py_ssize_t seqlen = 0;
	size_t sz = 0;
	Py_ssize_t i;
	PyObject *seq, *item;

	seq = PySequence_Fast(orig, "");
	if (seq == NULL) {
		return NULL;
	}

	seqlen = PySequence_Size(seq);
	if (seqlen == 0) {
		Py_DECREF(seq);
		return PyString_FromString("");
	}
	if (seqlen == 1) {
		item = PySequence_Fast_GET_ITEM(seq, 0);
		if (PyString_CheckExact(item) || PyUnicode_CheckExact(item)) {
			Py_INCREF(item);
			Py_DECREF(seq);
			return item;
		}
	}

	/* There are at least two things to join, or else we have a subclass
	 * of the builtin types in the sequence.
	 * Do a pre-pass to figure out the total amount of space we'll
	 * need (sz), see whether any argument is absurd, and defer to
	 * the Unicode join if appropriate.
	 */
	for (i = 0; i < seqlen; i++) {
		const size_t old_sz = sz;
		item = PySequence_Fast_GET_ITEM(seq, i);
		if (!PyString_Check(item)){
#ifdef Py_USING_UNICODE
			if (PyUnicode_Check(item)) {
				/* Defer to Unicode join.
				 * CAUTION:  There's no gurantee that the
				 * original sequence can be iterated over
				 * again, so we must pass seq here.
				 */
				PyObject *result;
				result = PyUnicode_Join((PyObject *)self, seq);
				Py_DECREF(seq);
				return result;
			}
#endif
			PyErr_Format(PyExc_TypeError,
				     "sequence item %zd: expected string,"
				     " %.80s found",
				     i, item->ob_type->tp_name);
			Py_DECREF(seq);
			return NULL;
		}
		sz += PyString_GET_SIZE(item);
		if (i != 0)
			sz += seplen;
		if (sz < old_sz || sz > PY_SSIZE_T_MAX) {
			PyErr_SetString(PyExc_OverflowError,
				"join() result is too long for a Python string");
			Py_DECREF(seq);
			return NULL;
		}
	}

	/* Allocate result space. */
	res = PyString_FromStringAndSize((char*)NULL, sz);
	if (res == NULL) {
		Py_DECREF(seq);
		return NULL;
	}

	/* Catenate everything. */
	p = PyString_AS_STRING(res);
	for (i = 0; i < seqlen; ++i) {
		size_t n;
		item = PySequence_Fast_GET_ITEM(seq, i);
		n = PyString_GET_SIZE(item);
		Py_MEMCPY(p, PyString_AS_STRING(item), n);
		p += n;
		if (i < seqlen - 1) {
			Py_MEMCPY(p, sep, seplen);
			p += seplen;
		}
	}

	Py_DECREF(seq);
	return res;
}

/*********************************
 *****BaseStringType Function*****
 *********************************/
static PyObject * basestring_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	PyErr_SetString(PyExc_TypeError,
			"The basestring type cannot be instantiated");
	return NULL;
