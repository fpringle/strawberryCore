/*
 * File:   doublylinkedlist.h
 * Author: pi
 *
 * Created on June 13, 2020, 6:10 PM
 */

#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

template <class T>
struct binode {
    T data;
    struct binode <T> * next;
    struct binode <T> * prev;
};

template <class T>
struct binode <T> * newBiNode( );


template <class T>
class doublylinkedlist {
    struct binode <T> * head;
    struct binode <T>* tail;

public:
    // constructors
    doublylinkedlist( );
    // adding values
    void append( T );
    void prepend( T );
    void insertRight( int, T );
    void insertLeft( int, T );
    // deleting values
    void deleteIndex( int );
    // get info
    int linear_search( T );
    int len( );
    T min( );
    T max( );
    T get_item( int );
    bool is_empty( );
    // do stuff
    void set_item( int, T );
    void swap( int, int );
    void bubbleSort( );
    void selectionSort( );
    void insertionSort( );
    // utility
    void print( );
    void headtail();
};


#endif /* DOUBLYLINKEDLIST_H */

