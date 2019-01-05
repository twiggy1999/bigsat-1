#ifndef Minisat_List_h
#define Minisat_List_h

#include <assert.h>
#include <new>

#include "mtl/IntTypes.h"
#include "mtl/XAlloc.h"

namespace Minisat {

//=================================================================================================
// Automatically resizable arrays
//
// NOTE! Don't use this vector on datatypes that cannot be re-located in memory (with realloc)

template<class T>
class node {
public:
    T data;
    node *next;

    node(T da = NULL, node *p = NULL) {
        this->data = da;
        this->next = p;
    }
};

template<class T>
class list {
    node* head;
    node* tail;

public:
    // Constructors:
    list()                       : head(NULL) , tail(NULL)  { }
    ~list()                       { clear(); }

    // Pointer to first element:
    operator T*       (void)           { return head; }
;
    // Size operations:
    int      size     (void) const     {/*TODO: return sz;*/ }

    void     clear    (bool dealloc = false);

    // Stack interface:
    void     add  (T& data)              {/*TODO: add certain node with data: data at tail/head of the list*/}
    void     add  (node* Node)           {/*TODO: add certain node at tail/head of the list*/}
    void     delete   (T& data)        {/*TODO: delete certain node with data : data*/}
    void     delete   (node* Node)     {/*TODO: delete certain node*/}
    
};




template<class T>
void list<T>::clear(bool dealloc) {
//TODO clear list;
}

#endif