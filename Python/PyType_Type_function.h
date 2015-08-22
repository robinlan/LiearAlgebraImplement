static void type_dealloc(PyTypeObject *type) {
	PyHeapTypeObject *et;

	/* Assert this is a heap-allocated type object */
	assert(type->tp_flags & Py_TPFLAGS_HEAPTYPE);
	_PyObject_GC_UNTRACK(type);
	PyObject_ClearWeakRefs((PyObject *)type);
	et = (PyHeapTypeObject *)type;
	Py_XDECREF(type->tp_base);
	Py_XDECREF(type->tp_dict);
	Py_XDECREF(type->tp_bases);
	Py_XDECREF(type->tp_mro);
	Py_XDECREF(type->tp_cache);
	Py_XDECREF(type->tp_subclasses);
        /* A type's tp_doc is heap allocated, unlike the tp_doc slots
         * of most other objects.  It's okay to cast it to char *.
         */
	PyObject_Free((char *)type->tp_doc);
	Py_XDECREF(et->ht_name);
	Py_XDECREF(et->ht_slots);
	type->ob_type->tp_free((PyObject *)type);
}

static int type_compare(PyObject *v, PyObject *w) {
	/* This is called with type objects only. So we
	   can just compare the addresses. */
	Py_uintptr_t vv = (Py_uintptr_t)v;
	Py_uintptr_t ww = (Py_uintptr_t)w;
	return (vv < ww) ? -1 : (vv > ww) ? 1 : 0;
}

static PyObject * type_repr(PyTypeObject *type) {
	PyObject *mod, *name, *rtn;
	char *kind;

	mod = type_module(type, NULL);
	if (mod == NULL)
		PyErr_Clear();
	else if (!PyString_Check(mod)) {
		Py_DECREF(mod);
		mod = NULL;
	}
	name = type_name(type, NULL);
	if (name == NULL)
		return NULL;

	if (type->tp_flags & Py_TPFLAGS_HEAPTYPE)
		kind = "class";
	else
		kind = "type";

	if (mod != NULL && strcmp(PyString_AS_STRING(mod), "__builtin__")) {
		rtn = PyString_FromFormat("<%s '%s.%s'>",
					  kind,
					  PyString_AS_STRING(mod),
					  PyString_AS_STRING(name));
	}
	else
		rtn = PyString_FromFormat("<%s '%s'>", kind, type->tp_name);

	Py_XDECREF(mod);
	Py_DECREF(name);
	return rtn;
}

Py_hash_t _Py_HashPointer(void *p) {
    Py_hash_t x;
    size_t y = (size_t)p;
    /* bottom 3 or 4 bits are likely to be 0; rotate y by 4 to avoid
       excessive hash collisions for dicts and sets */
    y = (y >> 4) | (y << (8 * SIZEOF_VOID_P - 4));
    x = (Py_hash_t)y;
    if (x == -1)
        x = -2;
    return x;
}

static PyObject * type_call(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	PyObject *obj;

	if (type->tp_new == NULL) {
		PyErr_Format(PyExc_TypeError,
			     "cannot create '%.100s' instances",
			     type->tp_name);
		return NULL;
	}

	obj = type->tp_new(type, args, kwds);
	if (obj != NULL) {
		/* Ugly exception: when the call was type(something),
		   don't call tp_init on the result. */
		if (type == &PyType_Type &&
		    PyTuple_Check(args) && PyTuple_GET_SIZE(args) == 1 &&
		    (kwds == NULL ||
		     (PyDict_Check(kwds) && PyDict_Size(kwds) == 0)))
			return obj;
		/* If the returned object is not an instance of type,
		   it won't be initialized. */
		if (!PyType_IsSubtype(obj->ob_type, type))
			return obj;
		type = obj->ob_type;
		if (PyType_HasFeature(type, Py_TPFLAGS_HAVE_CLASS) &&
		    type->tp_init != NULL &&
		    type->tp_init(obj, args, kwds) < 0) {
			Py_DECREF(obj);
			obj = NULL;
		}
	}
	return obj;
}

static PyObject * type_getattro(PyTypeObject *type, PyObject *name) {
	PyTypeObject *metatype = type->ob_type;
	PyObject *meta_attribute, *attribute;
	descrgetfunc meta_get;

	/* Initialize this type (we'll assume the metatype is initialized) */
	if (type->tp_dict == NULL) {
		if (PyType_Ready(type) < 0)
			return NULL;
	}

	/* No readable descriptor found yet */
	meta_get = NULL;

	/* Look for the attribute in the metatype */
	meta_attribute = _PyType_Lookup(metatype, name);

	if (meta_attribute != NULL) {
		meta_get = meta_attribute->ob_type->tp_descr_get;

		if (meta_get != NULL && PyDescr_IsData(meta_attribute)) {
			/* Data descriptors implement tp_descr_set to intercept
			 * writes. Assume the attribute is not overridden in
			 * type's tp_dict (and bases): call the descriptor now.
			 */
			return meta_get(meta_attribute, (PyObject *)type,
					(PyObject *)metatype);
		}
		Py_INCREF(meta_attribute);
	}

	/* No data descriptor found on metatype. Look in tp_dict of this
	 * type and its bases */
	attribute = _PyType_Lookup(type, name);
	if (attribute != NULL) {
		/* Implement descriptor functionality, if any */
		descrgetfunc local_get = attribute->ob_type->tp_descr_get;

		Py_XDECREF(meta_attribute);

		if (local_get != NULL) {
			/* NULL 2nd argument indicates the descriptor was
			 * found on the target object itself (or a base)  */
			return local_get(attribute, (PyObject *)NULL,
					 (PyObject *)type);
		}

		Py_INCREF(attribute);
		return attribute;
	}

	/* No attribute found in local __dict__ (or bases): use the
	 * descriptor from the metatype, if any */
	if (meta_get != NULL) {
		PyObject *res;
		res = meta_get(meta_attribute, (PyObject *)type,
			       (PyObject *)metatype);
		Py_DECREF(meta_attribute);
		return res;
	}

	/* If an ordinary attribute was found on the metatype, return it now */
	if (meta_attribute != NULL) {
		return meta_attribute;
	}

	/* Give up */
	PyErr_Format(PyExc_AttributeError,
			 "type object '%.50s' has no attribute '%.400s'",
			 type->tp_name, PyString_AS_STRING(name));
	return NULL;
}

static int type_setattro(PyTypeObject *type, PyObject *name, PyObject *value) {
	if (!(type->tp_flags & Py_TPFLAGS_HEAPTYPE)) {
		PyErr_Format(
			PyExc_TypeError,
			"can't set attributes of built-in/extension type '%s'",
			type->tp_name);
		return -1;
	}
	/* XXX Example of how I expect this to be used...
	if (update_subclasses(type, name, invalidate_cache, NULL) < 0)
		return -1;
	*/
	if (PyObject_GenericSetAttr((PyObject *)type, name, value) < 0)
		return -1;
	return update_slot(type, name);
}

static int type_traverse(PyTypeObject *type, visitproc visit, void *arg) {
	/* Because of type_is_gc(), the collector only calls this
	   for heaptypes. */
	assert(type->tp_flags & Py_TPFLAGS_HEAPTYPE);

	Py_VISIT(type->tp_dict);
	Py_VISIT(type->tp_cache);
	Py_VISIT(type->tp_mro);
	Py_VISIT(type->tp_bases);
	Py_VISIT(type->tp_base);

	/* There's no need to visit type->tp_subclasses or
	   ((PyHeapTypeObject *)type)->ht_slots, because they can't be involved
	   in cycles; tp_subclasses is a list of weak references,
	   and slots is a tuple of strings. */

	return 0;
}

static int type_clear(PyTypeObject *type) {
	/* Because of type_is_gc(), the collector only calls this
	   for heaptypes. */
	assert(type->tp_flags & Py_TPFLAGS_HEAPTYPE);

	Py_CLEAR(type->tp_mro);

	return 0;
}

static PyObject *
type_new(PyTypeObject *metatype, PyObject *args, PyObject *kwds)
{
	PyObject *name, *bases, *dict;
	static char *kwlist[] = {"name", "bases", "dict", 0};
	PyObject *slots, *tmp, *newslots;
	PyTypeObject *type, *base, *tmptype, *winner;
	PyHeapTypeObject *et;
	PyMemberDef *mp;
	Py_ssize_t i, nbases, nslots, slotoffset, add_dict, add_weak;
	int j, may_add_dict, may_add_weak;

	assert(args != NULL && PyTuple_Check(args));
	assert(kwds == NULL || PyDict_Check(kwds));

	/* Special case: type(x) should return x->ob_type */
	{
		const Py_ssize_t nargs = PyTuple_GET_SIZE(args);
		const Py_ssize_t nkwds = kwds == NULL ? 0 : PyDict_Size(kwds);

		if (PyType_CheckExact(metatype) && nargs == 1 && nkwds == 0) {
			PyObject *x = PyTuple_GET_ITEM(args, 0);
			Py_INCREF(x->ob_type);
			return (PyObject *) x->ob_type;
		}

		/* SF bug 475327 -- if that didn't trigger, we need 3
		   arguments. but PyArg_ParseTupleAndKeywords below may give
		   a msg saying type() needs exactly 3. */
		if (nargs + nkwds != 3) {
			PyErr_SetString(PyExc_TypeError,
					"type() takes 1 or 3 arguments");
			return NULL;
		}
	}

	/* Check arguments: (name, bases, dict) */
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "SO!O!:type", kwlist,
					 &name,
					 &PyTuple_Type, &bases,
					 &PyDict_Type, &dict))
		return NULL;

	/* Determine the proper metatype to deal with this,
	   and check for metatype conflicts while we're at it.
	   Note that if some other metatype wins to contract,
	   it's possible that its instances are not types. */
	nbases = PyTuple_GET_SIZE(bases);
	winner = metatype;
	for (i = 0; i < nbases; i++) {
		tmp = PyTuple_GET_ITEM(bases, i);
		tmptype = tmp->ob_type;
		if (tmptype == &PyClass_Type)
			continue; /* Special case classic classes */
		if (PyType_IsSubtype(winner, tmptype))
			continue;
		if (PyType_IsSubtype(tmptype, winner)) {
			winner = tmptype;
			continue;
		}
		PyErr_SetString(PyExc_TypeError,
				"metaclass conflict: "
				"the metaclass of a derived class "
				"must be a (non-strict) subclass "
				"of the metaclasses of all its bases");
		return NULL;
	}
	if (winner != metatype) {
		if (winner->tp_new != type_new) /* Pass it to the winner */
			return winner->tp_new(winner, args, kwds);
		metatype = winner;
	}

	/* Adjust for empty tuple bases */
	if (nbases == 0) {
		bases = PyTuple_Pack(1, &PyBaseObject_Type);
		if (bases == NULL)
			return NULL;
		nbases = 1;
	}
	else
		Py_INCREF(bases);

	/* XXX From here until type is allocated, "return NULL" leaks bases! */

	/* Calculate best base, and check that all bases are type objects */
	base = best_base(bases);
	if (base == NULL) {
		Py_DECREF(bases);
		return NULL;
	}
	if (!PyType_HasFeature(base, Py_TPFLAGS_BASETYPE)) {
		PyErr_Format(PyExc_TypeError,
			     "type '%.100s' is not an acceptable base type",
			     base->tp_name);
		Py_DECREF(bases);
		return NULL;
	}

	/* Check for a __slots__ sequence variable in dict, and count it */
	slots = PyDict_GetItemString(dict, "__slots__");
	nslots = 0;
	add_dict = 0;
	add_weak = 0;
	may_add_dict = base->tp_dictoffset == 0;
	may_add_weak = base->tp_weaklistoffset == 0 && base->tp_itemsize == 0;
	if (slots == NULL) {
		if (may_add_dict) {
			add_dict++;
		}
		if (may_add_weak) {
			add_weak++;
		}
	}
	else {
		/* Have slots */

		/* Make it into a tuple */
		if (PyString_Check(slots))
			slots = PyTuple_Pack(1, slots);
		else
			slots = PySequence_Tuple(slots);
		if (slots == NULL) {
			Py_DECREF(bases);
			return NULL;
		}
		assert(PyTuple_Check(slots));

		/* Are slots allowed? */
		nslots = PyTuple_GET_SIZE(slots);
		if (nslots > 0 && base->tp_itemsize != 0) {
			PyErr_Format(PyExc_TypeError,
				     "nonempty __slots__ "
				     "not supported for subtype of '%s'",
				     base->tp_name);
		  bad_slots:
			Py_DECREF(bases);
			Py_DECREF(slots);
			return NULL;
		}

#ifdef Py_USING_UNICODE
		tmp = _unicode_to_string(slots, nslots);
		if (tmp != slots) {
			Py_DECREF(slots);
			slots = tmp;
		}
		if (!tmp)
			return NULL;
#endif
		/* Check for valid slot names and two special cases */
		for (i = 0; i < nslots; i++) {
			PyObject *tmp = PyTuple_GET_ITEM(slots, i);
			char *s;
			if (!valid_identifier(tmp))
				goto bad_slots;
			assert(PyString_Check(tmp));
			s = PyString_AS_STRING(tmp);
			if (strcmp(s, "__dict__") == 0) {
				if (!may_add_dict || add_dict) {
					PyErr_SetString(PyExc_TypeError,
						"__dict__ slot disallowed: "
						"we already got one");
					goto bad_slots;
				}
				add_dict++;
			}
			if (strcmp(s, "__weakref__") == 0) {
				if (!may_add_weak || add_weak) {
					PyErr_SetString(PyExc_TypeError,
						"__weakref__ slot disallowed: "
						"either we already got one, "
						"or __itemsize__ != 0");
					goto bad_slots;
				}
				add_weak++;
			}
		}

		/* Copy slots into yet another tuple, demangling names */
		newslots = PyTuple_New(nslots - add_dict - add_weak);
		if (newslots == NULL)
			goto bad_slots;
		for (i = j = 0; i < nslots; i++) {
			char *s;
			tmp = PyTuple_GET_ITEM(slots, i);
			s = PyString_AS_STRING(tmp);
			if ((add_dict && strcmp(s, "__dict__") == 0) ||
			    (add_weak && strcmp(s, "__weakref__") == 0))
				continue;
			tmp =_Py_Mangle(name, tmp);
                        if (!tmp)
                            goto bad_slots;
			PyTuple_SET_ITEM(newslots, j, tmp);
			j++;
		}
		assert(j == nslots - add_dict - add_weak);
		nslots = j;
		Py_DECREF(slots);
		slots = newslots;

		/* Secondary bases may provide weakrefs or dict */
		if (nbases > 1 &&
		    ((may_add_dict && !add_dict) ||
		     (may_add_weak && !add_weak))) {
			for (i = 0; i < nbases; i++) {
				tmp = PyTuple_GET_ITEM(bases, i);
				if (tmp == (PyObject *)base)
					continue; /* Skip primary base */
				if (PyClass_Check(tmp)) {
					/* Classic base class provides both */
					if (may_add_dict && !add_dict)
						add_dict++;
					if (may_add_weak && !add_weak)
						add_weak++;
					break;
				}
				assert(PyType_Check(tmp));
				tmptype = (PyTypeObject *)tmp;
				if (may_add_dict && !add_dict &&
				    tmptype->tp_dictoffset != 0)
					add_dict++;
				if (may_add_weak && !add_weak &&
				    tmptype->tp_weaklistoffset != 0)
					add_weak++;
				if (may_add_dict && !add_dict)
					continue;
				if (may_add_weak && !add_weak)
					continue;
				/* Nothing more to check */
				break;
			}
		}
	}

	/* XXX From here until type is safely allocated,
	   "return NULL" may leak slots! */

	/* Allocate the type object */
	type = (PyTypeObject *)metatype->tp_alloc(metatype, nslots);
	if (type == NULL) {
		Py_XDECREF(slots);
		Py_DECREF(bases);
		return NULL;
	}

	/* Keep name and slots alive in the extended type object */
	et = (PyHeapTypeObject *)type;
	Py_INCREF(name);
	et->ht_name = name;
	et->ht_slots = slots;

	/* Initialize tp_flags */
	type->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE |
		Py_TPFLAGS_BASETYPE;
	if (base->tp_flags & Py_TPFLAGS_HAVE_GC)
		type->tp_flags |= Py_TPFLAGS_HAVE_GC;

	/* It's a new-style number unless it specifically inherits any
	   old-style numeric behavior */
	if ((base->tp_flags & Py_TPFLAGS_CHECKTYPES) ||
	    (base->tp_as_number == NULL))
		type->tp_flags |= Py_TPFLAGS_CHECKTYPES;

	/* Initialize essential fields */
	type->tp_as_number = &et->as_number;
	type->tp_as_sequence = &et->as_sequence;
	type->tp_as_mapping = &et->as_mapping;
	type->tp_as_buffer = &et->as_buffer;
	type->tp_name = PyString_AS_STRING(name);

	/* Set tp_base and tp_bases */
	type->tp_bases = bases;
	Py_INCREF(base);
	type->tp_base = base;

	/* Initialize tp_dict from passed-in dict */
	type->tp_dict = dict = PyDict_Copy(dict);
	if (dict == NULL) {
		Py_DECREF(type);
		return NULL;
	}

	/* Set __module__ in the dict */
	if (PyDict_GetItemString(dict, "__module__") == NULL) {
		tmp = PyEval_GetGlobals();
		if (tmp != NULL) {
			tmp = PyDict_GetItemString(tmp, "__name__");
			if (tmp != NULL) {
				if (PyDict_SetItemString(dict, "__module__",
							 tmp) < 0)
					return NULL;
			}
		}
	}

	/* Set tp_doc to a copy of dict['__doc__'], if the latter is there
	   and is a string.  The __doc__ accessor will first look for tp_doc;
	   if that fails, it will still look into __dict__.
	*/
	{
		PyObject *doc = PyDict_GetItemString(dict, "__doc__");
		if (doc != NULL && PyString_Check(doc)) {
			const size_t n = (size_t)PyString_GET_SIZE(doc);
                        char *tp_doc = (char *)PyObject_MALLOC(n+1);
			if (tp_doc == NULL) {
				Py_DECREF(type);
				return NULL;
			}
			memcpy(tp_doc, PyString_AS_STRING(doc), n+1);
                        type->tp_doc = tp_doc;
		}
	}

	/* Special-case __new__: if it's a plain function,
	   make it a static function */
	tmp = PyDict_GetItemString(dict, "__new__");
	if (tmp != NULL && PyFunction_Check(tmp)) {
		tmp = PyStaticMethod_New(tmp);
		if (tmp == NULL) {
			Py_DECREF(type);
			return NULL;
		}
		PyDict_SetItemString(dict, "__new__", tmp);
		Py_DECREF(tmp);
	}

	/* Add descriptors for custom slots from __slots__, or for __dict__ */
	mp = PyHeapType_GET_MEMBERS(et);
	slotoffset = base->tp_basicsize;
	if (slots != NULL) {
		for (i = 0; i < nslots; i++, mp++) {
			mp->name = PyString_AS_STRING(
				PyTuple_GET_ITEM(slots, i));
			mp->type = T_OBJECT_EX;
			mp->offset = slotoffset;
			if (base->tp_weaklistoffset == 0 &&
			    strcmp(mp->name, "__weakref__") == 0) {
				add_weak++;
				mp->type = T_OBJECT;
				mp->flags = READONLY;
				type->tp_weaklistoffset = slotoffset;
			}
			slotoffset += sizeof(PyObject *);
		}
	}
	if (add_dict) {
		if (base->tp_itemsize)
			type->tp_dictoffset = -(long)sizeof(PyObject *);
		else
			type->tp_dictoffset = slotoffset;
		slotoffset += sizeof(PyObject *);
	}
	if (add_weak) {
		assert(!base->tp_itemsize);
		type->tp_weaklistoffset = slotoffset;
		slotoffset += sizeof(PyObject *);
	}
	type->tp_basicsize = slotoffset;
	type->tp_itemsize = base->tp_itemsize;
	type->tp_members = PyHeapType_GET_MEMBERS(et);

	if (type->tp_weaklistoffset && type->tp_dictoffset)
		type->tp_getset = subtype_getsets_full;
	else if (type->tp_weaklistoffset && !type->tp_dictoffset)
		type->tp_getset = subtype_getsets_weakref_only;
	else if (!type->tp_weaklistoffset && type->tp_dictoffset)
		type->tp_getset = subtype_getsets_dict_only;
	else
		type->tp_getset = NULL;

	/* Special case some slots */
	if (type->tp_dictoffset != 0 || nslots > 0) {
		if (base->tp_getattr == NULL && base->tp_getattro == NULL)
			type->tp_getattro = PyObject_GenericGetAttr;
		if (base->tp_setattr == NULL && base->tp_setattro == NULL)
			type->tp_setattro = PyObject_GenericSetAttr;
	}
	type->tp_dealloc = subtype_dealloc;

	/* Enable GC unless there are really no instance variables possible */
	if (!(type->tp_basicsize == sizeof(PyObject) &&
	      type->tp_itemsize == 0))
		type->tp_flags |= Py_TPFLAGS_HAVE_GC;

	/* Always override allocation strategy to use regular heap */
	type->tp_alloc = PyType_GenericAlloc;
	if (type->tp_flags & Py_TPFLAGS_HAVE_GC) {
		type->tp_free = PyObject_GC_Del;
		type->tp_traverse = subtype_traverse;
		type->tp_clear = subtype_clear;
	}
	else
		type->tp_free = PyObject_Del;

	/* Initialize the rest */
	if (PyType_Ready(type) < 0) {
		Py_DECREF(type);
		return NULL;
	}

	/* Put the proper slots in place */
	fixup_slot_dispatchers(type);

	return (PyObject *)type;
}

void PyObject_GC_Del(void *op) {
    PyGC_Head *g = AS_GC(op);
    if (IS_TRACKED(op))
        gc_list_remove(g);
    if (generations[0].count > 0) {
        generations[0].count--;
    }
    PyObject_FREE(g);
}

static int type_is_gc(PyTypeObject *type) {
	return type->tp_flags & Py_TPFLAGS_HEAPTYPE;
}