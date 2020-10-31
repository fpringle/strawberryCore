/*
 * File:   queue.h
 * Author: pi
 *
 * Created on June 13, 2020, 6:11 PM
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "doublylinkedlist.h"

template <class T>
class queue {
    doublylinkedlist <T> lst;

public:
    queue( );
    void enqueue( T );
    // pop to target
    void dequeue( T* );
    // pop and return
    T dequeue( );
    // return head without popping
    T peek( );
    // check if empty
    bool is_empty( );
    // print
    void print( );
    // length
    int len( );
};

#endif /* QUEUE_H */

