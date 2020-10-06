#ifndef __NTREE_H
#define __NTREE_H

#include <vector>
#include <iostream>

#include "ntree.h"



template <class T>
struct ntreeNode {
    T data;
    ntreeNode<T> * sibling, * firstChild;
    ntreeNode<T>() {};
    ntreeNode<T>(T newData) {
        data = newData;
        sibling = NULL;
        firstChild = NULL;
    }
};


template <class T>
void add_child( ntreeNode<T> * parent, ntreeNode<T> * child ) {
    if ( parent->firstChild == NULL) {
        parent->firstChild = child;
    }
    else {
        ntreeNode<T> * p = parent->firstChild;
        while ( p->sibling != NULL ) p = p->sibling;
        p->sibling = child;
    }
}

template <class T>
void add_child( ntreeNode<T> * parent, T data ) {
    ntreeNode<T> * child = new ntreeNode<T>(data);
    add_child(parent,child);
}

template <class T>
void add_sibling( ntreeNode<T> * node, ntreeNode<T> * sib ) {
    ntreeNode<T> * p = node;
    while ( p->sibling != NULL ) p = p->sibling;
    p->sibling = sib;
}

template <class T>
void add_sibling( ntreeNode<T> * node, T data ) {
    ntreeNode<T> * sib = new ntreeNode<T>(data);
    add_sibling(node,sib);
}

template <class T>
void get_leaves( ntreeNode<T> * root, T ** arr ) {
    if ( root == NULL ) return;
    else if ( root->firstChild == NULL ) {
        **arr = root->data;
        (*arr)++;
    }
    else {
        ntreeNode<T> * child = root->firstChild;
        while ( child != NULL ) {
            get_leaves(child,arr);
            child = child->sibling;
        }
    }
}

template <class T>
void get_leaves( ntreeNode<T> * root, std::vector<T> * vec ) {
    if ( root == NULL ) return;
    else if ( root->firstChild == NULL ) {
        (*vec).push_back(root->data);
    }
    else {
        ntreeNode<T> * child = root->firstChild;
        while ( child != NULL ) {
            get_leaves(child,vec);
            child = child->sibling;
        }
    }
}

template <class T>
void print_tree( ntreeNode<T> * root, std::ostream& cout=std::cout, int depth=0 ) {
    for ( int i=0; i<depth; i++ ) cout << '-';
    cout << " " << root->data << std::endl;
    ntreeNode<T> * child = root->firstChild;
    while (child != NULL) {
        print_tree( child, cout, depth+1 );
        child = child->sibling;
    }
}

template class ntreeNode<int32_t>;


#endif
