/*
 * File:   tree.h
 * Author: pi
 *
 * Created on June 13, 2020, 6:13 PM
 */

#ifndef TREE_H
#define TREE_H

#include "linkedlist.h"

template <class T>
struct treenode {
    T data;
    struct treenode <T> *left, *right;
};

template <class T>
struct treenode <T> * newTreeNode( );

template <class T>
bool is_leaf_node( struct treenode <T> * );

template <class T>
void insert_after_leaf_left( treenode <T> *, T );

template <class T>
void insert_after_leaf_right( treenode <T> *, T );

template <class T>
void append( treenode <T> **, T );

template <class T>
void get_leaves( treenode <T> *, linkedlist <T> * );


template <class T>
class tree {
public:
    struct treenode <T> * root;

    //public:
    tree( );
    tree( T );

    void insert( T );

    void print_leaves( );
};

#endif /* TREE_H */

