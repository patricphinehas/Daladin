#include "adaptermodule.h"

GMainLoop	    *gMain_loop = NULL;
char             adpName[30];

int
Adapter_init(Adapter *self, PyObject *args, PyObject *kwds)
{
    puts("Adapter_init");
    errorCode_t	ec;
    char           **adpArray = NULL;
    int              adpLen;
    char             adpName[30];
    neardal_adapter	*adapter;
    static int	     power = 1;

    /* Look for available adapter */
    ec = neardal_get_adapters(&adpArray, &adpLen);
    if (ec == NEARDAL_SUCCESS)
    {
        printf(".. Adapter found at '%s'\n", adpArray[0]);
        memcpy(adpName, adpArray[0], sizeof(adpName));
        neardal_free_array(&adpArray);
    } else
    {
        printf("No adapter found (%s)\n", neardal_error_get_text(ec));
        return 1;
    }

    /* Power on first adapter found */
    ec = neardal_get_adapter_properties(adpName,&adapter);
    if (ec == NEARDAL_SUCCESS)
    {
        power=adapter->powered;
        neardal_free_adapter(adapter);
        if (!power)
        {
            power = 1;
            ec = neardal_set_adapter_property(adpName, NEARD_ADP_PROP_POWERED, GINT_TO_POINTER(power));
            if (ec != NEARDAL_SUCCESS) {
                printf("Error setting adapter properties\n");
                return 1;
            }
        }
    } else
    {
        printf("Error getting adapter properties\n");
        return 1;
    }

    neardal_set_cb_adapter_added(call_adapter_added, NULL);
    neardal_set_cb_adapter_removed(call_adapter_removed, NULL);
    neardal_set_cb_adapter_property_changed(call_adapter_property_changed, NULL);
    ec = neardal_set_cb_tag_found(call_tag_found, NULL);
    if (ec != NEARDAL_SUCCESS)
    {
        printf("Error registering Tag found callback\n");
        return 1;
    }
    neardal_set_cb_tag_lost(call_tag_lost, NULL);
    neardal_set_cb_record_found(call_record_found, NULL);

    return 0;
}

PyObject* launch(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    errorCode_t	ec;
    /* Start Discovery Loop*/
	ec = neardal_start_poll(adpName);
	if (ec != NEARDAL_SUCCESS && ec != NEARDAL_ERROR_POLLING_ALREADY_ACTIVE)
	{
		printf("Error starting discovery loop\n");
		//return 1;
        return NULL;
	}

	gMain_loop = g_main_loop_new(NULL, FALSE);
	if (gMain_loop) {
		g_main_loop_run(gMain_loop);
		g_main_loop_unref(gMain_loop);
	} else
        return NULL;
		//return 1;

    Py_INCREF(Py_None);
    result = Py_None;

    return result;
}

static PyObject*
say_hello(PyObject* self, PyObject* args)
{
    const char* name;

    if (!PyArg_ParseTuple(args, "s", &name))
    return NULL;

    printf("Hello %s!\n", name);

    Py_RETURN_NONE;
}

/* neardal_set_cb_adapter_added(cb_adapter_added, NULL);
 * neardal_set_cb_adapter_removed(cb_adapter_removed, NULL);
 * neardal_set_cb_adapter_property_changed(cb_adapter_prop_changed, NULL);
 * neardal_set_cb_tag_found(cb_tag_found, NULL);
 * neardal_set_cb_tag_lost(cb_tag_lost, NULL);
 * neardal_set_cb_record_found(cb_record_found, NULL);
 */

// signals for action
static PyObject* callback_adapter_added = NULL;
PyObject* add_callback_adapter_added(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_adapter_added);  /* Dispose of previous callback */
        callback_adapter_added = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

void call_adapter_added(const char* tagName, void* data) {
    puts("call_adapter_added");
}

// signals for action
static PyObject* callback_adapter_removed = NULL;
PyObject* add_callback_adapter_removed(Adapter* self, PyObject* args) {
    PyObject* result = NULL;
    PyObject* temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_adapter_removed);  /* Dispose of previous callback */
        callback_adapter_removed = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

void call_adapter_removed(const char* tagName, void* data) {
    puts("call_adapter_removed");
}

// signals for action
static PyObject* callback_adapter_property_changed = NULL;
PyObject* add_callback_adapter_property_changed(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_adapter_property_changed);  /* Dispose of previous callback */
        callback_adapter_property_changed = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

void call_adapter_property_changed(char *adpName, char *propName, void *value, void *user_data) {
    puts("call_adapter_property_changed");
}

// signals for action
static PyObject* callback_tag_found = NULL;
PyObject* add_callback_tag_found(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_tag_found);  /* Dispose of previous callback */
        callback_tag_found = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

void call_tag_found(const char* tagName, void* data) {
    puts("call_tag_found");
}

// signals for action
static PyObject* callback_tag_lost = NULL;
PyObject* add_callback_tag_lost(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_tag_lost);  /* Dispose of previous callback */
        callback_tag_lost = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

void call_tag_lost(const char* tagName, void* data) {
    puts("call_tag_lost");
}

// signals for action
static PyObject* callback_record_found = NULL;
PyObject* add_callback_record_found(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_record_found);  /* Dispose of previous callback */
        callback_record_found = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

void call_record_found(const char* tagName, void* data) {
    puts("call_record_found");
}

/**********************
 * Python definitions *
 **********************/

PyMethodDef AdapterMethods[] =
{
    {"say_hello", (PyCFunction)say_hello, METH_VARARGS, "Greet somebody."},
    {"launch", (PyCFunction)launch, METH_VARARGS, "launch adapter interaction"},
    {"add_callback_adapter_added", (PyCFunction)add_callback_adapter_added, METH_VARARGS, "add callback for action"},
    {"add_callback_adapter_removed", (PyCFunction)add_callback_adapter_removed, METH_VARARGS, "add callback for action"},
    {"add_callback_adapter_property_changed", (PyCFunction)add_callback_adapter_property_changed, METH_VARARGS, "add callback for action"},
    {"add_callback_tag_found", (PyCFunction)add_callback_tag_found, METH_VARARGS, "add callback for action"},
    {"add_callback_tag_lost", (PyCFunction)add_callback_tag_lost, METH_VARARGS, "add callback for action"},
    {"add_callback_record_found", (PyCFunction)add_callback_record_found, METH_VARARGS, "add callback for action"},
    {NULL, NULL, 0, NULL}
};

PyTypeObject AdapterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "neardal.Adapter",             /* tp_name */
    sizeof(Adapter), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "adapter object",          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    AdapterMethods,            /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Adapter_init,      /* tp_init */
};