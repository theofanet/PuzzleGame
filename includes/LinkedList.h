//
// Created by Theo Fanet on 25/10/2016.
//

#ifndef TESTPROG_LINKEDLIST_H
#define TESTPROG_LINKEDLIST_H

#include <iostream>

class LinkedListElement{
public:
    LinkedListElement(){
        next     = NULL;
        previous = NULL;
    }

    LinkedListElement *next, *previous;
};


class LinkedList {
public:
    LinkedList(){
        _first = NULL;
        _size  = 0;
    }

    void push(LinkedListElement *value) {
        if(_first)
            _first->previous = value;
        value->next = _first;
        _first = value;
        _size++;
    }

    void clearSize(){
        _size = 0;
        _first = NULL;
    }

    void remove(LinkedListElement *value){
        if(_size){
            if(value->previous)
                value->previous->next = value->next;
            else
                _first = value->next;
            //delete_obj(value);
            _size--;
        }
    }

    LinkedListElement* getFirst() { return _first; }

    int size(){ return _size; }

private:
    LinkedListElement* _first;
    int _size;
};


#endif //TESTPROG_LINKEDLIST_H
