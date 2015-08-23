/********************************
 ***Basic _typeobject Instance***
 ********************************/
PyTypeObject PyType_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,					/* ob_size */
	"type",					/* tp_name */
	sizeof(PyHeapTypeObject),		/* tp_basicsize */
	sizeof(PyMemberDef),			/* tp_itemsize */
	(destructor)type_dealloc,		/* tp_dealloc */
	0,					/* tp_print */
	0,			 		/* tp_getattr */
	0,					/* tp_setattr */
	type_compare,				/* tp_compare */
	(reprfunc)type_repr,			/* tp_repr */
	0,					/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	(hashfunc)_Py_HashPointer,		/* tp_hash */
	(ternaryfunc)type_call,			/* tp_call */
	0,					/* tp_str */
	(getattrofunc)type_getattro,		/* tp_getattro */
	(setattrofunc)type_setattro,		/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |
		Py_TPFLAGS_BASETYPE,		/* tp_flags */
	type_doc,				/* tp_doc */
	(traverseproc)type_traverse,		/* tp_traverse */
	(inquiry)type_clear,			/* tp_clear */
	0,					/* tp_richcompare */
	offsetof(PyTypeObject, tp_weaklist),	/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	type_methods,				/* tp_methods */
	type_members,				/* tp_members */
	type_getsets,				/* tp_getset */
	0,					/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	offsetof(PyTypeObject, tp_dict),	/* tp_dictoffset */
	0,					/* tp_init */
	0,					/* tp_alloc */
	type_new,				/* tp_new */
	PyObject_GC_Del,        		/* tp_free */
	(inquiry)type_is_gc,			/* tp_is_gc */
};

PyDoc_STRVAR(type_doc,
"type(object) -> the object's type\n"
"type(name, bases, dict) -> a new type");

static PyMethodDef type_methods[] = {
	{"mro", (PyCFunction)mro_external, METH_NOARGS,
	 PyDoc_STR("mro() -> list\nreturn a type's method resolution order")},
	{"__subclasses__", (PyCFunction)type_subclasses, METH_NOARGS,
	 PyDoc_STR("__subclasses__() -> list of immediate subclasses")},
	{0}
};

static PyMemberDef type_members[] = {
	{"__basicsize__", T_INT, offsetof(PyTypeObject,tp_basicsize),READONLY},
	{"__itemsize__", T_INT, offsetof(PyTypeObject, tp_itemsize), READONLY},
	{"__flags__", T_LONG, offsetof(PyTypeObject, tp_flags), READONLY},
	{"__weakrefoffset__", T_LONG,
	 offsetof(PyTypeObject, tp_weaklistoffset), READONLY},
	{"__base__", T_OBJECT, offsetof(PyTypeObject, tp_base), READONLY},
	{"__dictoffset__", T_LONG,
	 offsetof(PyTypeObject, tp_dictoffset), READONLY},
	{"__mro__", T_OBJECT, offsetof(PyTypeObject, tp_mro), READONLY},
	{0}
};

static PyGetSetDef type_getsets[] = {
	{"__name__", (getter)type_name, (setter)type_set_name, NULL},
	{"__bases__", (getter)type_get_bases, (setter)type_set_bases, NULL},
	{"__module__", (getter)type_module, (setter)type_set_module, NULL},
	{"__dict__",  (getter)type_dict,  NULL, NULL},
	{"__doc__", (getter)type_get_doc, NULL, NULL},
	{0}
};

/********************************
 **Integer _typeobject Instance**
 ********************************/
PyTypeObject PyInt_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"int",
	sizeof(PyIntObject),
	0,
	(destructor)int_dealloc,		/* tp_dealloc */
	(printfunc)int_print,			/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	(cmpfunc)int_compare,			/* tp_compare */
	(reprfunc)int_repr,			/* tp_repr */
	&int_as_number,				/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	(hashfunc)int_hash,			/* tp_hash */
        0,					/* tp_call */
        (reprfunc)int_repr,			/* tp_str */
	PyObject_GenericGetAttr,		/* tp_getattro */
	0,					/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
		Py_TPFLAGS_BASETYPE,		/* tp_flags */
	int_doc,				/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	int_methods,				/* tp_methods */
	0,					/* tp_members */
	0,					/* tp_getset */
	0,					/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	0,					/* tp_init */
	0,					/* tp_alloc */
	int_new,				/* tp_new */
	(freefunc)int_free,           		/* tp_free */
};

static PyNumberMethods int_as_number = {
	(binaryfunc)int_add,	/*nb_add*/
	(binaryfunc)int_sub,	/*nb_subtract*/
	(binaryfunc)int_mul,	/*nb_multiply*/
	(binaryfunc)int_classic_div, /*nb_divide*/
	(binaryfunc)int_mod,	/*nb_remainder*/
	(binaryfunc)int_divmod,	/*nb_divmod*/
	(ternaryfunc)int_pow,	/*nb_power*/
	(unaryfunc)int_neg,	/*nb_negative*/
	(unaryfunc)int_pos,	/*nb_positive*/
	(unaryfunc)int_abs,	/*nb_absolute*/
	(inquiry)int_nonzero,	/*nb_nonzero*/
	(unaryfunc)int_invert,	/*nb_invert*/
	(binaryfunc)int_lshift,	/*nb_lshift*/
	(binaryfunc)int_rshift,	/*nb_rshift*/
	(binaryfunc)int_and,	/*nb_and*/
	(binaryfunc)int_xor,	/*nb_xor*/
	(binaryfunc)int_or,	/*nb_or*/
	int_coerce,		/*nb_coerce*/
	(unaryfunc)int_int,	/*nb_int*/
	(unaryfunc)int_long,	/*nb_long*/
	(unaryfunc)int_float,	/*nb_float*/
	(unaryfunc)int_oct,	/*nb_oct*/
	(unaryfunc)int_hex, 	/*nb_hex*/
	0,			/*nb_inplace_add*/
	0,			/*nb_inplace_subtract*/
	0,			/*nb_inplace_multiply*/
	0,			/*nb_inplace_divide*/
	0,			/*nb_inplace_remainder*/
	0,			/*nb_inplace_power*/
	0,			/*nb_inplace_lshift*/
	0,			/*nb_inplace_rshift*/
	0,			/*nb_inplace_and*/
	0,			/*nb_inplace_xor*/
	0,			/*nb_inplace_or*/
	(binaryfunc)int_div,	/* nb_floor_divide */
	int_true_divide,	/* nb_true_divide */
	0,			/* nb_inplace_floor_divide */
	0,			/* nb_inplace_true_divide */
	(unaryfunc)int_int,	/* nb_index */
};

PyDoc_STRVAR(int_doc,
"int(x[, base]) -> integer\n\
\n\
Convert a string or number to an integer, if possible.  A floating point\n\
argument will be truncated towards zero (this does not include a string\n\
representation of a floating point number!)  When converting a string, use\n\
the optional base.  It is an error to supply a base when converting a\n\
non-string. If the argument is outside the integer range a long object\n\
will be returned instead.");

static PyMethodDef int_methods[] = {
	{"__getnewargs__",	(PyCFunction)int_getnewargs,	METH_NOARGS},
	{NULL,		NULL}		/* sentinel */
};

/* Small Integer Object Pool */
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
static PyIntObject *small_ints[NSMALLNEGINTS + NSMALLPOSINTS];
#endif

/* Common Integer Pool for Big Integer Object */
static PyIntBlock *block_list = NULL;
static PyIntObject *free_list = NULL;

/*********************************
 ***String _typeobject Instance***
 *********************************/
PyTypeObject PyString_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"str",
	sizeof(PyStringObject),
	sizeof(char),
 	string_dealloc, 			/* tp_dealloc */
	(printfunc)string_print, 		/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	string_repr, 				/* tp_repr */
	&string_as_number,			/* tp_as_number */
	&string_as_sequence,			/* tp_as_sequence */
	&string_as_mapping,			/* tp_as_mapping */
	(hashfunc)string_hash, 			/* tp_hash */
	0,					/* tp_call */
	string_str,				/* tp_str */
	PyObject_GenericGetAttr,		/* tp_getattro */
	0,					/* tp_setattro */
	&string_as_buffer,			/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
		Py_TPFLAGS_BASETYPE,		/* tp_flags */
	string_doc,				/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	(richcmpfunc)string_richcompare,	/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	string_methods,				/* tp_methods */
	0,					/* tp_members */
	0,					/* tp_getset */
	&PyBaseString_Type,			/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	0,					/* tp_init */
	0,					/* tp_alloc */
	string_new,				/* tp_new */
	PyObject_Del,	                	/* tp_free */
};

static PyNumberMethods string_as_number = {
	0,			/*nb_add*/
	0,			/*nb_subtract*/
	0,			/*nb_multiply*/
	0, 			/*nb_divide*/
	string_mod,		/*nb_remainder*/
};

static PySequenceMethods string_as_sequence = {
	(lenfunc)string_length, /*sq_length*/
	(binaryfunc)string_concat, /*sq_concat*/
	(ssizeargfunc)string_repeat, /*sq_repeat*/
	(ssizeargfunc)string_item, /*sq_item*/
	(ssizessizeargfunc)string_slice, /*sq_slice*/
	0,		/*sq_ass_item*/
	0,		/*sq_ass_slice*/
	(objobjproc)string_contains /*sq_contains*/
};

static PyMappingMethods string_as_mapping = {
	(lenfunc)string_length,
	(binaryfunc)string_subscript,
	0,
};

static PyBufferProcs string_as_buffer = {
	(readbufferproc)string_buffer_getreadbuf,
	(writebufferproc)string_buffer_getwritebuf,
	(segcountproc)string_buffer_getsegcount,
	(charbufferproc)string_buffer_getcharbuf,
};

PyDoc_STRVAR(string_doc,
"str(object) -> string\n\
\n\
Return a nice string representation of the object.\n\
If the argument is a string, the return value is the same object.");

static PyMethodDef
string_methods[] = {
	/* Counterparts of the obsolete stropmodule functions; except
	   string.maketrans(). */
	{"join", (PyCFunction)string_join, METH_O, join__doc__},
	{"split", (PyCFunction)string_split, METH_VARARGS, split__doc__},
	{"rsplit", (PyCFunction)string_rsplit, METH_VARARGS, rsplit__doc__},
	{"lower", (PyCFunction)string_lower, METH_NOARGS, lower__doc__},
	{"upper", (PyCFunction)string_upper, METH_NOARGS, upper__doc__},
	{"islower", (PyCFunction)string_islower, METH_NOARGS, islower__doc__},
	{"isupper", (PyCFunction)string_isupper, METH_NOARGS, isupper__doc__},
	{"isspace", (PyCFunction)string_isspace, METH_NOARGS, isspace__doc__},
	{"isdigit", (PyCFunction)string_isdigit, METH_NOARGS, isdigit__doc__},
	{"istitle", (PyCFunction)string_istitle, METH_NOARGS, istitle__doc__},
	{"isalpha", (PyCFunction)string_isalpha, METH_NOARGS, isalpha__doc__},
	{"isalnum", (PyCFunction)string_isalnum, METH_NOARGS, isalnum__doc__},
	{"capitalize", (PyCFunction)string_capitalize, METH_NOARGS,
	 capitalize__doc__},
	{"count", (PyCFunction)string_count, METH_VARARGS, count__doc__},
	{"endswith", (PyCFunction)string_endswith, METH_VARARGS,
	 endswith__doc__},
	{"partition", (PyCFunction)string_partition, METH_O, partition__doc__},
	{"find", (PyCFunction)string_find, METH_VARARGS, find__doc__},
	{"index", (PyCFunction)string_index, METH_VARARGS, index__doc__},
	{"lstrip", (PyCFunction)string_lstrip, METH_VARARGS, lstrip__doc__},
	{"replace", (PyCFunction)string_replace, METH_VARARGS, replace__doc__},
	{"rfind", (PyCFunction)string_rfind, METH_VARARGS, rfind__doc__},
	{"rindex", (PyCFunction)string_rindex, METH_VARARGS, rindex__doc__},
	{"rstrip", (PyCFunction)string_rstrip, METH_VARARGS, rstrip__doc__},
	{"rpartition", (PyCFunction)string_rpartition, METH_O,
	 rpartition__doc__},
	{"startswith", (PyCFunction)string_startswith, METH_VARARGS,
	 startswith__doc__},
	{"strip", (PyCFunction)string_strip, METH_VARARGS, strip__doc__},
	{"swapcase", (PyCFunction)string_swapcase, METH_NOARGS,
	 swapcase__doc__},
	{"translate", (PyCFunction)string_translate, METH_VARARGS,
	 translate__doc__},
	{"title", (PyCFunction)string_title, METH_NOARGS, title__doc__},
	{"ljust", (PyCFunction)string_ljust, METH_VARARGS, ljust__doc__},
	{"rjust", (PyCFunction)string_rjust, METH_VARARGS, rjust__doc__},
	{"center", (PyCFunction)string_center, METH_VARARGS, center__doc__},
	{"zfill", (PyCFunction)string_zfill, METH_VARARGS, zfill__doc__},
	{"encode", (PyCFunction)string_encode, METH_VARARGS, encode__doc__},
	{"decode", (PyCFunction)string_decode, METH_VARARGS, decode__doc__},
	{"expandtabs", (PyCFunction)string_expandtabs, METH_VARARGS,
	 expandtabs__doc__},
	{"splitlines", (PyCFunction)string_splitlines, METH_VARARGS,
	 splitlines__doc__},
	{"__getnewargs__",	(PyCFunction)string_getnewargs,	METH_NOARGS},
	{NULL,     NULL}		     /* sentinel */
};

PyTypeObject PyBaseString_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"basestring",
	0,
	0,
 	0,			 		/* tp_dealloc */
	0,			 		/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	0,		 			/* tp_repr */
	0,					/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	0,		 			/* tp_hash */
	0,					/* tp_call */
	0,					/* tp_str */
	0,					/* tp_getattro */
	0,					/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	basestring_doc,				/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	0,					/* tp_methods */
	0,					/* tp_members */
	0,					/* tp_getset */
	&PyBaseObject_Type,			/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	0,					/* tp_init */
	0,					/* tp_alloc */
	basestring_new,				/* tp_new */
	0,		                	/* tp_free */
};

PyDoc_STRVAR(basestring_doc,
"Type basestring cannot be instantiated; it is the base for str and unicode.");

static PyStringObject *characters[UCHAR_MAX + 1];
