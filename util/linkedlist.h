/*
 * File:   linkedlist.h
 * Author: pi
 *
 * Created on June 13, 2020, 6:09 PM
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template <class T>
struct node {
    T data;
    struct node <T> * next;
};

template <class T>
struct node <T> * newNode( );


template <class T>
class linkedlist {
    struct node <T> * head;
    struct node <T> * get_tail( );

public:
    // constructors
    linkedlist( );
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
};

#endif /* LINKEDLIST_H */

