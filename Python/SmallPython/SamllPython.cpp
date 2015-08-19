#include<string>
#include<map>
#include<cstdlib>
#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

#define PyObject_HEAD \
    int refCount;\
    struct tagPyTypeObject *type

#define PyObject_HEAD_INIT(typePtr)\
    0, typePtr

typedef struct tagPyObject {
    PyObject_HEAD;
}PyObject;

//definition of PyTypeObject
typedef void (*PrintFun)(PyObject* object);
typedef PyObject* (*AddFun)(PyObject* left, PyObject* right);
typedef long (*HashFun)(PyObject* object);

//Function Declare
void int_print(PyObject* object);
void string_print(PyObject* object);
long int_hash(PyObject* object);
PyObject* int_add(PyObject* left, PyObject* right);
long string_hash(PyObject* object);
PyObject* string_add(PyObject* left, PyObject* right);
PyObject* GetObjectBySymbol(string& symbol);
void ExcutePrint(string symbol);
void Excute();
void ExcuteAdd(string& target, string& source);
void ExcuteCommand(string& command);

typedef struct tagPyTypeObject {

    PyObject_HEAD;
    char* name;
    PrintFun print;
    AddFun add;
    HashFun hash;
}PyTypeObject;

PyTypeObject PyType_Type = {

	PyObject_HEAD_INIT(0),
	"type",
	0,
	0,
	0
};

PyTypeObject PyInt_Type = {

    PyObject_HEAD_INIT(&PyType_Type),
    "int",
    int_print,
    int_add,
    int_hash
};

typedef struct tagPyIntObject {

    PyObject_HEAD;
    int value;
}PyIntObject;

PyTypeObject PyString_Type = {

    PyObject_HEAD_INIT(&PyType_Type),
        "str",
        string_print,
        string_add,
        string_hash
};

PyObject* PyInt_Create(int value) {

    PyIntObject* object = new PyIntObject;
    object->refCount = 1;
    object->type = &PyInt_Type;
    object->value = value;
    return (PyObject*)object;
}

void int_print(PyObject* object) {

    PyIntObject* intObject = (PyIntObject*)object;
    printf("%d\n", intObject->value);
}

PyObject* int_add(PyObject* left, PyObject* right) {

    PyIntObject* leftInt = (PyIntObject*)left;
    PyIntObject* rightInt = (PyIntObject*)right;
    PyIntObject* result = (PyIntObject*)PyInt_Create(0);
    if(result == NULL) {
        printf("We have no enough memory!!");
    } else {
        result->value = leftInt->value + rightInt->value;
    }
    return (PyObject*)result;
}

long int_hash(PyObject* object) {
    return (long)((PyIntObject*)object)->value;
}

typedef struct tagPyStrObject {

    PyObject_HEAD;
    int length;
    long hashValue;
    char value[50];
}PyStringObject;

PyObject* PyStr_Create(const char* value) {

    PyStringObject* object = new PyStringObject;
    object->refCount = 1;
    object->type = &PyString_Type;
    object->length = (value == NULL) ? 0 : strlen(value);
    object->hashValue = -1;
    memset(object->value, 0, 50);
    if(value != NULL) {
        strcpy(object->value, value);
    }
    return (PyObject*)object;
}

void string_print(PyObject* object) {

    PyStringObject* strObject = (PyStringObject*)object;
    printf("%s\n", strObject->value);
}

long string_hash(PyObject* object) {

    PyStringObject* strObject = (PyStringObject*)object;
    register int len;
    register unsigned char *p;
    register long x;

    if (strObject->hashValue != -1)
        return strObject->hashValue;
    len = strObject->length;
    p = (unsigned char *)strObject->value;
    x = *p << 7;
    while (--len >= 0)
        x = (1000003*x) ^ *p++;
    x ^= strObject->length;
    if (x == -1)
        x = -2;
    strObject->hashValue = x;
    return x;
}

PyObject* string_add(PyObject* left, PyObject* right) {

    PyStringObject* leftStr = (PyStringObject*)left;
    PyStringObject* rightStr = (PyStringObject*)right;
    PyStringObject* result = (PyStringObject*)PyStr_Create(NULL);
    if(result == NULL) {
        printf("We have no enough memory!!");
    } else {
        strcpy(result->value, leftStr->value);
        strcat(result->value, rightStr->value);
    }
    return (PyObject*)result;
}

typedef struct tagPyDictObject {

    PyObject_HEAD;
    map<long, PyObject*> dict;
}PyDictObject;

void dict_print(PyObject* object);

PyTypeObject PyDict_Type = {

    PyObject_HEAD_INIT(&PyType_Type),
        "dict",
        dict_print,
        0,
        0
};

PyObject* PyDict_Create() {

    //create object
    PyDictObject* object = new PyDictObject;
    object->refCount = 1;
    object->type = &PyDict_Type;

    return (PyObject*)object;
}

PyObject* PyDict_GetItem(PyObject* target, PyObject* key) {

    long keyHashValue = (key->type)->hash(key);
    map<long, PyObject*>& dict = ((PyDictObject*)target)->dict;
    map<long, PyObject*>::iterator it = dict.find(keyHashValue);
    map<long, PyObject*>::iterator end = dict.end();
    if(it == end) {
        return NULL;
    }
    return it->second;
}

int PyDict_SetItem(PyObject* target, PyObject* key, PyObject* value){

    long keyHashValue = (key->type)->hash(key);
    PyDictObject* dictObject = (PyDictObject*)target;
    (dictObject->dict)[keyHashValue] = value;
    return 0;
}

//function for PyDict_Type
void dict_print(PyObject* object) {

    PyDictObject* dictObject = (PyDictObject*)object;
    printf("{");
    map<long, PyObject*>::iterator it = (dictObject->dict).begin();
    map<long, PyObject*>::iterator end = (dictObject->dict).end();
    for( ; it != end; ++it) {
	
        //print key
        printf("%d : ", it->first);
        //print value
        PyObject* value = it->second;
        (value->type)->print(value);
        printf(", ");
    }
    printf("}\n");
}

string mytrim(string &str) {

    char sch[50];
    int i=0,j=str.length()-1,k=0;
    while( str[i]==' ' && i<=j ) ++i;
    while( str[j]==' ' && i<=j ) --j;
    while( i<=j ) sch[k++] = str[i++];
    sch[k] = '\0';
    return sch;
}

void ExcuteCommand(string& command) {

    string::size_type pos = 0;
    if((pos = command.find("print")) != string::npos) {
        ExcutePrint(command.substr(5));
    } else if((pos = command.find("=")) != string::npos) {
	
        string target = command.substr(0, pos);
        string source = command.substr(pos+1);
        ExcuteAdd(target, source);
    }
}


bool IsSourceAllDigit(string& source1) {

    string source = mytrim( source1 );
	int i=0;
	while( source[i] ) {
	
		if( !( source[i]>='0' && source[i]<='9' ) )
			return false;
		++i;
	}
	return true;
 }

PyObject *m_LocalEnvironment = PyDict_Create();

void ExcuteAdd(string& target1, string& source1) {

    string target=mytrim(target1), source=mytrim(source1);
    string::size_type pos;
    if(IsSourceAllDigit(source)) {
	
        PyObject* intValue = PyInt_Create(atoi(source.c_str()));
        PyObject* key = PyStr_Create(target.c_str());
        PyDict_SetItem(m_LocalEnvironment, key, intValue);
    } else if(source.find("\"") != string::npos) {
	
        PyObject* strValue = PyStr_Create(source.substr(1, source.size()-2).c_str());
        PyObject* key = PyStr_Create(target.c_str());
        PyDict_SetItem(m_LocalEnvironment, key, strValue);
    } else if((pos = source.find("+")) != string::npos) {
	
        string tempstr;
        tempstr = source.substr(0, pos);
        PyObject* leftObject = GetObjectBySymbol(tempstr);
        tempstr = source.substr(pos+1);
        PyObject* rightObject = GetObjectBySymbol(tempstr);
        if(leftObject != NULL && right != NULL && leftObject->type == rightObject->type) {
		
            PyObject* resultValue = (leftObject->type)->add(leftObject, rightObject);
            PyObject* key = PyStr_Create(target.c_str());
            PyDict_SetItem(m_LocalEnvironment, key, resultValue);
        }
            //(m_LocalEnvironment->type)->print(m_LocalEnvironment);
    }
}

PyObject* GetObjectBySymbol(string& symbol2) {

    string symbol = mytrim( symbol2 );
    PyObject* key = PyStr_Create(symbol.c_str());
    PyObject* value = PyDict_GetItem(m_LocalEnvironment, key);
    if(value == NULL) {
	
        cout << "[Error] : " << symbol << " is not defined!!" << endl;
        return NULL;
    }
    return value;
}

void ExcutePrint(string symbol) {

    PyObject* object = GetObjectBySymbol(symbol);
    if(object != NULL) {
	
        PyTypeObject* type = object->type;
        type->print(object);
    }
}

char* info = "********** Python Research **********\n";
char* prompt = ">>> ";

void Excute() {

	string m_Command;
    cout << info;
    cout << prompt;
    while(getline(cin, m_Command)){
        if(m_Command.size() == 0){
            cout << prompt;
            continue;
        } else if(m_Command == "exit") {
            return;
        } else {
            ExcuteCommand(m_Command);
        }
        cout << prompt;
    }
}

int main() {

	Excute();
	return 0;
}
