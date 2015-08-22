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