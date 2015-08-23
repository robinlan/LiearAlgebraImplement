/****************************************
 ******PyObject-related Definitions******
 ****************************************/
#define _PyObject_HEAD_EXTRA		\
	struct _object *_ob_next;		\
	struct _object *_ob_prev;

#define PyObject_HEAD				\
	_PyObject_HEAD_EXTRA			\
	Py_ssize_t ob_refcnt;			\
	struct _typeobject *ob_type;
	
#define _PyObject_EXTRA_INIT 0, 0,
	
#define PyObject_HEAD_INIT(type)	\
	_PyObject_EXTRA_INIT			\
	1, type,

#define PyObject_VAR_HEAD			\
	PyObject_HEAD					\
	Py_ssize_t ob_size;
	
/*******************************
 ******tp_flag Definitions******
 *******************************/
#define Py_TPFLAGS_HAVE_GETCHARBUFFER  (1L<<0)
#define Py_TPFLAGS_HAVE_SEQUENCE_IN (1L<<1)
#define Py_TPFLAGS_HAVE_INPLACEOPS (1L<<3)
#define Py_TPFLAGS_HAVE_RICHCOMPARE (1L<<5)
#define Py_TPFLAGS_HAVE_WEAKREFS (1L<<6)
#define Py_TPFLAGS_HAVE_ITER (1L<<7)
#define Py_TPFLAGS_HAVE_CLASS (1L<<8)

#ifdef STACKLESS
#define Py_TPFLAGS_HAVE_STACKLESS_EXTENSION (3L<<15)
#else
#define Py_TPFLAGS_HAVE_STACKLESS_EXTENSION 0
#endif

#define Py_TPFLAGS_HAVE_INDEX (1L<<17)
#define Py_TPFLAGS_DEFAULT  ( 								 		\
                             Py_TPFLAGS_HAVE_GETCHARBUFFER | 		\
                             Py_TPFLAGS_HAVE_SEQUENCE_IN | 			\
                             Py_TPFLAGS_HAVE_INPLACEOPS | 			\
                             Py_TPFLAGS_HAVE_RICHCOMPARE | 			\
                             Py_TPFLAGS_HAVE_WEAKREFS | 			\
                             Py_TPFLAGS_HAVE_ITER | 				\
                             Py_TPFLAGS_HAVE_CLASS | 				\
                             Py_TPFLAGS_HAVE_STACKLESS_EXTENSION | 	\
                             Py_TPFLAGS_HAVE_INDEX | 				\
                             0)

#define Py_TPFLAGS_HAVE_GC (1UL << 14)
#define Py_TPFLAGS_BASETYPE (1L<<10)

/********************************
 *******tp_doc Definitions*******
 ********************************/
#define PyDoc_VAR(name) static char name[]
#define PyDoc_STRVAR(name,str) PyDoc_VAR(name) = PyDoc_STR(str)

#ifdef WITH_DOC_STRINGS
#define PyDoc_STR(str) str
#else
#define PyDoc_STR(str) ""
#endif

/***************************************
 ******Reference Count Definitions******
 ***************************************/
#define _Py_NewReference(op) (				\
	_Py_INC_TPALLOCS(op) _Py_COUNT_ALLOCS_COMMA	\
	_Py_INC_REFTOTAL  _Py_REF_DEBUG_COMMA		\
	(op)->ob_refcnt = 1)

#define _Py_ForgetReference(op) _Py_INC_TPFREES(op)

#define _Py_Dealloc(op) (				\
	_Py_INC_TPFREES(op) _Py_COUNT_ALLOCS_COMMA	\
	(*(op)->ob_type->tp_dealloc)((PyObject *)(op)))
#endif 

#define Py_INCREF(op) (				\
	_Py_INC_REFTOTAL  _Py_REF_DEBUG_COMMA	\
	(op)->ob_refcnt++)

#define Py_DECREF(op)					\
	if (_Py_DEC_REFTOTAL  _Py_REF_DEBUG_COMMA	\
	    --(op)->ob_refcnt != 0)			\
		_Py_CHECK_REFCNT(op)			\
	else						\
		_Py_Dealloc((PyObject *)(op))

#define Py_XINCREF(op) if ((op) == NULL) ; else Py_INCREF(op)
#define Py_XDECREF(op) if ((op) == NULL) ; else Py_DECREF(op)

/***************************************
 ****PyIntObject-related Definitions****
 ***************************************/
#define PyInt_AS_LONG(op) (((PyIntObject *)(op))->ob_ival)
#define CONVERT_TO_LONG(obj, lng)		\
	if (PyInt_Check(obj)) {			\
		lng = PyInt_AS_LONG(obj);	\
	}					\
	else {					\
		Py_INCREF(Py_NotImplemented);	\
		return Py_NotImplemented;	\
	}

/* Small Integer Object Pool */
#ifndef NSMALLPOSINTS
#define NSMALLPOSINTS		257
#endif
#ifndef NSMALLNEGINTS
#define NSMALLNEGINTS		5
#endif

/* Common Integer Object Pool */
#define BLOCK_SIZE	1000	
#define BHEAD_SIZE	8	
#define N_INTOBJECTS	((BLOCK_SIZE - BHEAD_SIZE) / sizeof(PyIntObject))

/***************************************
 ****PyIntObject-related Definitions****
 ***************************************/
 
 /* This value depends on platform, this one is from Win32 */
 #define UCHAR_MAX   0xff