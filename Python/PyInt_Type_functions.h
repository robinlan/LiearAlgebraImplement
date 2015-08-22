static void int_dealloc(PyIntObject *v) {
	if (PyInt_CheckExact(v)) {
		v->ob_type = (struct _typeobject *)free_list;
		free_list = v;
	}
	else
		v->ob_type->tp_free((PyObject *)v);
}

static int int_print(PyIntObject *v, FILE *fp, int flags) {
	fprintf(fp, "%ld", v->ob_ival);
	return 0;
}

static int int_compare(PyIntObject *v, PyIntObject *w) {
	register long i = v->ob_ival;
	register long j = w->ob_ival;
	return (i < j) ? -1 : (i > j) ? 1 : 0;
}

static PyObject * int_repr(PyIntObject *v) {
	char buf[64];
	PyOS_snprintf(buf, sizeof(buf), "%ld", v->ob_ival);
	return PyString_FromString(buf);
}

static PyObject * int_add(PyIntObject *v, PyIntObject *w) {
	register long a, b, x;
	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	x = a + b;
	if ((x^a) >= 0 || (x^b) >= 0)
		return PyInt_FromLong(x);
	return PyLong_Type.tp_as_number->nb_add((PyObject *)v, (PyObject *)w);
}

static PyObject * int_sub(PyIntObject *v, PyIntObject *w) {
	register long a, b, x;
	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	x = a - b;
	if ((x^a) >= 0 || (x^~b) >= 0)
		return PyInt_FromLong(x);
	return PyLong_Type.tp_as_number->nb_subtract((PyObject *)v,
						     (PyObject *)w);
}

static PyObject * int_mul(PyObject *v, PyObject *w) {
	long a, b;
	long longprod;			/* a*b in native long arithmetic */
	double doubled_longprod;	/* (double)longprod */
	double doubleprod;		/* (double)a * (double)b */

	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	longprod = a * b;
	doubleprod = (double)a * (double)b;
	doubled_longprod = (double)longprod;

	/* Fast path for normal case:  small multiplicands, and no info
	   is lost in either method. */
	if (doubled_longprod == doubleprod)
		return PyInt_FromLong(longprod);

	/* Somebody somewhere lost info.  Close enough, or way off?  Note
	   that a != 0 and b != 0 (else doubled_longprod == doubleprod == 0).
	   The difference either is or isn't significant compared to the
	   true value (of which doubleprod is a good approximation).
	*/
	{
		const double diff = doubled_longprod - doubleprod;
		const double absdiff = diff >= 0.0 ? diff : -diff;
		const double absprod = doubleprod >= 0.0 ? doubleprod :
							  -doubleprod;
		/* absdiff/absprod <= 1/32 iff
		   32 * absdiff <= absprod -- 5 good bits is "close enough" */
		if (32.0 * absdiff <= absprod)
			return PyInt_FromLong(longprod);
		else
			return PyLong_Type.tp_as_number->nb_multiply(v, w);
	}
}

static PyObject * int_classic_div(PyIntObject *x, PyIntObject *y) {
	long xi, yi;
	long d, m;
	CONVERT_TO_LONG(x, xi);
	CONVERT_TO_LONG(y, yi);
	if (Py_DivisionWarningFlag &&
	    PyErr_Warn(PyExc_DeprecationWarning, "classic int division") < 0)
		return NULL;
	switch (i_divmod(xi, yi, &d, &m)) {
	case DIVMOD_OK:
		return PyInt_FromLong(d);
	case DIVMOD_OVERFLOW:
		return PyLong_Type.tp_as_number->nb_divide((PyObject *)x,
							   (PyObject *)y);
	default:
		return NULL;
	}
}

static PyObject * int_mod(PyIntObject *x, PyIntObject *y) {
	long xi, yi;
	long d, m;
	CONVERT_TO_LONG(x, xi);
	CONVERT_TO_LONG(y, yi);
	switch (i_divmod(xi, yi, &d, &m)) {
	case DIVMOD_OK:
		return PyInt_FromLong(m);
	case DIVMOD_OVERFLOW:
		return PyLong_Type.tp_as_number->nb_remainder((PyObject *)x,
							      (PyObject *)y);
	default:
		return NULL;
	}
}

static PyObject * int_divmod(PyIntObject *x, PyIntObject *y) {
	long xi, yi;
	long d, m;
	CONVERT_TO_LONG(x, xi);
	CONVERT_TO_LONG(y, yi);
	switch (i_divmod(xi, yi, &d, &m)) {
	case DIVMOD_OK:
		return Py_BuildValue("(ll)", d, m);
	case DIVMOD_OVERFLOW:
		return PyLong_Type.tp_as_number->nb_divmod((PyObject *)x,
							   (PyObject *)y);
	default:
		return NULL;
	}
}

static PyObject * int_pow(PyIntObject *v, PyIntObject *w, PyIntObject *z) {
	register long iv, iw, iz=0, ix, temp, prev;
	CONVERT_TO_LONG(v, iv);
	CONVERT_TO_LONG(w, iw);
	if (iw < 0) {
		if ((PyObject *)z != Py_None) {
			PyErr_SetString(PyExc_TypeError, "pow() 2nd argument "
			     "cannot be negative when 3rd argument specified");
			return NULL;
		}
		/* Return a float.  This works because we know that
		   this calls float_pow() which converts its
		   arguments to double. */
		return PyFloat_Type.tp_as_number->nb_power(
			(PyObject *)v, (PyObject *)w, (PyObject *)z);
	}
 	if ((PyObject *)z != Py_None) {
		CONVERT_TO_LONG(z, iz);
		if (iz == 0) {
			PyErr_SetString(PyExc_ValueError,
					"pow() 3rd argument cannot be 0");
			return NULL;
		}
	}
	/*
	 * XXX: The original exponentiation code stopped looping
	 * when temp hit zero; this code will continue onwards
	 * unnecessarily, but at least it won't cause any errors.
	 * Hopefully the speed improvement from the fast exponentiation
	 * will compensate for the slight inefficiency.
	 * XXX: Better handling of overflows is desperately needed.
	 */
 	temp = iv;
	ix = 1;
	while (iw > 0) {
	 	prev = ix;	/* Save value for overflow check */
	 	if (iw & 1) {
		 	ix = ix*temp;
			if (temp == 0)
				break; /* Avoid ix / 0 */
			if (ix / temp != prev) {
				return PyLong_Type.tp_as_number->nb_power(
					(PyObject *)v,
					(PyObject *)w,
					(PyObject *)z);
			}
		}
	 	iw >>= 1;	/* Shift exponent down by 1 bit */
	        if (iw==0) break;
	 	prev = temp;
	 	temp *= temp;	/* Square the value of temp */
	 	if (prev != 0 && temp / prev != prev) {
			return PyLong_Type.tp_as_number->nb_power(
				(PyObject *)v, (PyObject *)w, (PyObject *)z);
		}
	 	if (iz) {
			/* If we did a multiplication, perform a modulo */
		 	ix = ix % iz;
		 	temp = temp % iz;
		}
	}
	if (iz) {
	 	long div, mod;
		switch (i_divmod(ix, iz, &div, &mod)) {
		case DIVMOD_OK:
			ix = mod;
			break;
		case DIVMOD_OVERFLOW:
			return PyLong_Type.tp_as_number->nb_power(
				(PyObject *)v, (PyObject *)w, (PyObject *)z);
		default:
			return NULL;
		}
	}
	return PyInt_FromLong(ix);
}

static PyObject * int_neg(PyIntObject *v) {
	register long a, x;
	a = v->ob_ival;
	x = -a;
	if (a < 0 && x < 0) {
		PyObject *o = PyLong_FromLong(a);
		if (o != NULL) {
			PyObject *result = PyNumber_Negative(o);
			Py_DECREF(o);
			return result;
		}
		return NULL;
	}
	return PyInt_FromLong(x);
}

static PyObject * int_pos(PyIntObject *v) {
	if (PyInt_CheckExact(v)) {
		Py_INCREF(v);
		return (PyObject *)v;
	}
	else
		return PyInt_FromLong(v->ob_ival);
}

static PyObject * int_abs(PyIntObject *v) {
	if (v->ob_ival >= 0)
		return int_pos(v);
	else
		return int_neg(v);
}

static int int_nonzero(PyIntObject *v) {
	return v->ob_ival != 0;
}

static PyObject * int_invert(PyIntObject *v) {
	return PyInt_FromLong(~v->ob_ival);
}

static PyObject * int_lshift(PyIntObject *v, PyIntObject *w) {
	long a, b, c;
	PyObject *vv, *ww, *result;

	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	if (b < 0) {
		PyErr_SetString(PyExc_ValueError, "negative shift count");
		return NULL;
	}
	if (a == 0 || b == 0)
		return int_pos(v);
	if (b >= LONG_BIT) {
		vv = PyLong_FromLong(PyInt_AS_LONG(v));
		if (vv == NULL)
			return NULL;
		ww = PyLong_FromLong(PyInt_AS_LONG(w));
		if (ww == NULL) {
			Py_DECREF(vv);
			return NULL;
		}
		result = PyNumber_Lshift(vv, ww);
		Py_DECREF(vv);
		Py_DECREF(ww);
		return result;
	}
	c = a << b;
	if (a != Py_ARITHMETIC_RIGHT_SHIFT(long, c, b)) {
		vv = PyLong_FromLong(PyInt_AS_LONG(v));
		if (vv == NULL)
			return NULL;
		ww = PyLong_FromLong(PyInt_AS_LONG(w));
		if (ww == NULL) {
			Py_DECREF(vv);
			return NULL;
		}
		result = PyNumber_Lshift(vv, ww);
		Py_DECREF(vv);
		Py_DECREF(ww);
		return result;
	}
	return PyInt_FromLong(c);
}

static PyObject * int_rshift(PyIntObject *v, PyIntObject *w) {
	register long a, b;
	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	if (b < 0) {
		PyErr_SetString(PyExc_ValueError, "negative shift count");
		return NULL;
	}
	if (a == 0 || b == 0)
		return int_pos(v);
	if (b >= LONG_BIT) {
		if (a < 0)
			a = -1;
		else
			a = 0;
	}
	else {
		a = Py_ARITHMETIC_RIGHT_SHIFT(long, a, b);
	}
	return PyInt_FromLong(a);
}

static PyObject * int_and(PyIntObject *v, PyIntObject *w) {
	register long a, b;
	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	return PyInt_FromLong(a & b);
}

static PyObject *int_xor(PyIntObject *v, PyIntObject *w) {
	register long a, b;
	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	return PyInt_FromLong(a ^ b);
}

static PyObject * int_or(PyIntObject *v, PyIntObject *w) {
	register long a, b;
	CONVERT_TO_LONG(v, a);
	CONVERT_TO_LONG(w, b);
	return PyInt_FromLong(a | b);
}

static int int_coerce(PyObject **pv, PyObject **pw) {
	if (PyInt_Check(*pw)) {
		Py_INCREF(*pv);
		Py_INCREF(*pw);
		return 0;
	}
	return 1; /* Can't do it */
}

static PyObject * int_int(PyIntObject *v) {
	if (PyInt_CheckExact(v))
		Py_INCREF(v);
	else
		v = (PyIntObject *)PyInt_FromLong(v->ob_ival);
	return (PyObject *)v;
}

static PyObject * int_long(PyIntObject *v) {
	return PyLong_FromLong((v -> ob_ival));
}

static PyObject * int_float(PyIntObject *v) {
	return PyFloat_FromDouble((double)(v -> ob_ival));
}

static PyObject * int_oct(PyIntObject *v) {
	char buf[100];
	long x = v -> ob_ival;
	if (x < 0)
		PyOS_snprintf(buf, sizeof(buf), "-0%lo", -x);
	else if (x == 0)
		strcpy(buf, "0");
	else
		PyOS_snprintf(buf, sizeof(buf), "0%lo", x);
	return PyString_FromString(buf);
}

static PyObject * int_hex(PyIntObject *v) {
	char buf[100];
	long x = v -> ob_ival;
	if (x < 0)
		PyOS_snprintf(buf, sizeof(buf), "-0x%lx", -x);
	else
		PyOS_snprintf(buf, sizeof(buf), "0x%lx", x);
	return PyString_FromString(buf);
}

static PyObject * int_div(PyIntObject *x, PyIntObject *y) {
	long xi, yi;
	long d, m;
	CONVERT_TO_LONG(x, xi);
	CONVERT_TO_LONG(y, yi);
	switch (i_divmod(xi, yi, &d, &m)) {
	case DIVMOD_OK:
		return PyInt_FromLong(d);
	case DIVMOD_OVERFLOW:
		return PyLong_Type.tp_as_number->nb_divide((PyObject *)x,
							   (PyObject *)y);
	default:
		return NULL;
	}
}

static PyObject * int_true_divide(PyObject *v, PyObject *w) {
	if (PyInt_Check(v) && PyInt_Check(w))
		return PyFloat_Type.tp_as_number->nb_true_divide(v, w);
	Py_INCREF(Py_NotImplemented);
	return Py_NotImplemented;
}

static PyObject * int_int(PyIntObject *v) {
	if (PyInt_CheckExact(v))
		Py_INCREF(v);
	else
		v = (PyIntObject *)PyInt_FromLong(v->ob_ival);
	return (PyObject *)v;
}

static long int_hash(PyIntObject *v) {
	/* XXX If this is changed, you also need to change the way
	   Python's long, float and complex types are hashed. */
	long x = v -> ob_ival;
	if (x == -1)
		x = -2;
	return x;
}

static PyObject * int_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	PyObject *x = NULL;
	int base = -909;
	static char *kwlist[] = {"x", "base", 0};

	if (type != &PyInt_Type)
		return int_subtype_new(type, args, kwds); /* Wimp out */
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi:int", kwlist,
					 &x, &base))
		return NULL;
	if (x == NULL)
		return PyInt_FromLong(0L);
	if (base == -909)
		return PyNumber_Int(x);
	if (PyString_Check(x))
		return PyInt_FromString(PyString_AS_STRING(x), NULL, base);
#ifdef Py_USING_UNICODE
	if (PyUnicode_Check(x))
		return PyInt_FromUnicode(PyUnicode_AS_UNICODE(x),
					 PyUnicode_GET_SIZE(x),
					 base);
#endif
	PyErr_SetString(PyExc_TypeError,
			"int() can't convert non-string with explicit base");
	return NULL;
}

static void int_free(PyIntObject *v) {
	v->ob_type = (struct _typeobject *)free_list;
	free_list = v;
}