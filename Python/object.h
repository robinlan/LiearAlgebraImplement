/* Long Types */
typedef long Py_ssize_t;
typedef Py_ssize_t Py_hash_t;
	
/******************************
 *******Built-in Objects*******
 ******************************/
typedef struct _object {
	PyObject_HEAD
} PyObject;

/******************************
 *******Built-in Objects*******
 ******************************/
typedef struct {
	PyObject_VAR_HEAD
} PyVarObject;

/******************************
 *******Built-in Objects*******
 ******************************/
typedef struct {
    PyObject_HEAD
    long ob_ival;
} PyIntObject;

/******************************
 *********Type Objects*********
 ******************************/
typedef struct _typeobject {
	PyObject_VAR_HEAD
	const char *tp_name; 
	Py_ssize_t tp_basicsize, tp_itemsize; 

	/* Methods to implement standard operations */

	destructor tp_dealloc;
	printfunc tp_print;
	getattrfunc tp_getattr;
	setattrfunc tp_setattr;
	cmpfunc tp_compare;
	reprfunc tp_repr;

	/* Method suites for standard classes */

	PyNumberMethods *tp_as_number;
	PySequenceMethods *tp_as_sequence;
	PyMappingMethods *tp_as_mapping;

	/* More standard operations (here for binary compatibility) */

	hashfunc tp_hash;
	ternaryfunc tp_call;
	reprfunc tp_str;
	getattrofunc tp_getattro;
	setattrofunc tp_setattro;

	/* Functions to access object as input/output buffer */
	PyBufferProcs *tp_as_buffer;

	/* Flags to define presence of optional/expanded features */
	long tp_flags;

	const char *tp_doc; /* Documentation string */

	/* Assigned meaning in release 2.0 */
	/* call function for all accessible objects */
	traverseproc tp_traverse;

	/* delete references to contained objects */
	inquiry tp_clear;

	/* Assigned meaning in release 2.1 */
	/* rich comparisons */
	richcmpfunc tp_richcompare;

	/* weak reference enabler */
	Py_ssize_t tp_weaklistoffset;

	/* Added in release 2.2 */
	/* Iterators */
	getiterfunc tp_iter;
	iternextfunc tp_iternext;

	/* Attribute descriptor and subclassing stuff */
	struct PyMethodDef *tp_methods;
	struct PyMemberDef *tp_members;
	struct PyGetSetDef *tp_getset;
	struct _typeobject *tp_base;
	PyObject *tp_dict;
	descrgetfunc tp_descr_get;
	descrsetfunc tp_descr_set;
	Py_ssize_t tp_dictoffset;
	initproc tp_init; /* Called by tp_new, construct a new PyObject object */
	allocfunc tp_alloc;
	newfunc tp_new;	  /* Called when trying to construct a new PyObject, if NULL, go find tp_bases */
	freefunc tp_free; /* Low-level free-memory routine */
	inquiry tp_is_gc; /* For PyObject_IS_GC */
	PyObject *tp_bases;
	PyObject *tp_mro; /* method resolution order */
	PyObject *tp_cache;
	PyObject *tp_subclasses;
	PyObject *tp_weaklist;
	destructor tp_del;

#ifdef COUNT_ALLOCS
	/* these must be last and never explicitly initialized */
	Py_ssize_t tp_allocs;
	Py_ssize_t tp_frees;
	Py_ssize_t tp_maxalloc;
	struct _typeobject *tp_prev;
	struct _typeobject *tp_next;
#endif
} PyTypeObject;


/******************************
 *******Functions Objects*******
 ******************************/
typedef struct {
	
	binaryfunc nb_add;
	binaryfunc nb_subtract;
	binaryfunc nb_multiply;
	binaryfunc nb_divide;
	binaryfunc nb_remainder;
	binaryfunc nb_divmod;
	ternar
	yfunc nb_power;
	unaryfunc nb_negative;
	unaryfunc nb_positive;
	unaryfunc nb_absolute;
	inquiry nb_nonzero;
	unaryfunc nb_invert;
	binaryfunc nb_lshift;
	binaryfunc nb_rshift;
	binaryfunc nb_and;
	binaryfunc nb_xor;
	binaryfunc nb_or;
	coercion nb_coerce;
	unaryfunc nb_int;
	unaryfunc nb_long;
	unaryfunc nb_float;
	unaryfunc nb_oct;
	unaryfunc nb_hex;
	/* Added in release 2.0 */
	binaryfunc nb_inplace_add;
	binaryfunc nb_inplace_subtract;
	binaryfunc nb_inplace_multiply;
	binaryfunc nb_inplace_divide;
	binaryfunc nb_inplace_remainder;
	ternaryfunc nb_inplace_power;
	binaryfunc nb_inplace_lshift;
	binaryfunc nb_inplace_rshift;
	binaryfunc nb_inplace_and;
	binaryfunc nb_inplace_xor;
	binaryfunc nb_inplace_or;

	/* Added in release 2.2 */
	/* The following require the Py_TPFLAGS_HAVE_CLASS flag */
	binaryfunc nb_floor_divide;
	binaryfunc nb_true_divide;
	binaryfunc nb_inplace_floor_divide;
	binaryfunc nb_inplace_true_divide;

	/* Added in release 2.5 */
	unaryfunc nb_index;
} PyNumberMethods;


/******************************
 *******Functions Objects*******
 ******************************/
typedef struct {
	lenfunc sq_length;
	binaryfunc sq_concat;
	ssizeargfunc sq_repeat;
	ssizeargfunc sq_item;
	ssizessizeargfunc sq_slice;
	ssizeobjargproc sq_ass_item;
	ssizessizeobjargproc sq_ass_slice;
	objobjproc sq_contains;
	/* Added in release 2.0 */
	binaryfunc sq_inplace_concat;
	ssizeargfunc sq_inplace_repeat;
} PySequenceMethods;


/******************************
 *******Functions Objects*******
 ******************************/
typedef struct {
	lenfunc mp_length;
	binaryfunc mp_subscript;
	objobjargproc mp_ass_subscript;
} PyMappingMethods;


/******************************
 *******Functions Objects*******
 ******************************/
typedef struct {
	readbufferproc bf_getreadbuffer;
	writebufferproc bf_getwritebuffer;
	segcountproc bf_getsegcount;
	charbufferproc bf_getcharbuffer;
} PyBufferProcs;

typedef struct _heaptypeobject {
	
	PyTypeObject ht_type;
	PyNumberMethods as_number;
	PyMappingMethods as_mapping;
	PySequenceMethods as_sequence; 
	
	PyBufferProcs as_buffer;
	PyObject *ht_name, *ht_slots;
	
} PyHeapTypeObject;

typedef struct PyMemberDef {
	char *name;
	int type;
	Py_ssize_t offset;
	int flags;
	char *doc;
} PyMemberDef;

typedef struct PyGetSetDef {
    char *name;
    getter get;
    setter set;
    char *doc;
    void *closure;
} PyGetSetDef;

struct _intblock { /* Common PyIntObject Pool for Big Integers */
	struct _intblock *next;
	PyIntObject objects[N_INTOBJECTS];
};
typedef struct _intblock PyIntBlock;