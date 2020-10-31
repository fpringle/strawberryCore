#ifndef __NTREE_H
#define __NTREE_H

#include <vector>
#include <iostream>

#include "ntree.h"
#include <queue>


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
    ~ntreeNode<T>();
};

template <class T>
ntreeNode<T>::~ntreeNode() {
    ntreeNode<T> * child, * next;
    child = firstChild;
    while (child != NULL) {
        next = child->sibling;
        delete child;
        child = next;
    }
}

template <class T>
void delete_child( ntreeNode<T> * parent, ntreeNode<T> * child ) {
    if (parent == NULL || child == NULL) return;
    ntreeNode<T> * p, * prev;
    p = parent->firstChild;
    if (p==child) {
        parent->firstChild = child->sibling;
    }
    else {
        bool found = false;
        while (p != NULL) {
            prev = p;
            p = p->sibling;
            if (p==child) {
                found = true;
                prev->sibling = p->sibling;
                break;
            }
        }
        if (!found) throw "ValueError: arg 2 is not a child of arg 1";
    }
    delete child;
}

template <class T>
ntreeNode<T> * choose_child( ntreeNode<T> * parent, ntreeNode<T> * child ) {
    if (parent == NULL || child == NULL) return parent;
    ntreeNode<T> * p = parent->firstChild, ret;
    bool found = false;
    while (p != NULL) {
        if (p == child) {
            found = true;
            break;
        }
        p = p->sibling;
    }
    if (!found) throw "ValueError: arg 2 is not a child of arg 1";
    p = parent->firstChild;
    ntreeNode<T> * next;
    while (p != NULL) {
        next = p->sibling;
        if (p != child) delete p;
        p = next;
    }
    return child;
}

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
int num_nodes( ntreeNode<T> * root ) {
    if (root==NULL) return 0;
    int ret = 1;
    ntreeNode<T> * child = root->firstChild;
    while (child != NULL) {
        ret += num_nodes(child);
        child = child->sibling;
    }
    return ret;
}

template <class T>
void num_nodes_per_level( ntreeNode<T> * root ) {
    int level = 0;
    std::queue<ntreeNode<T>*> Q;
    Q.push(root);
    int qlen,i;
    ntreeNode<T> * child, * v, * p;

    while (!Q.empty()) {
        qlen = Q.size();
        std::cout << qlen << " nodes at depth " << level << std::endl;
        for (i=0; i<qlen; i++) {
            v = Q.front();
            Q.pop();
            p = v->firstChild;
            while (p != NULL) {
                Q.push(p);
                p = p->sibling;
            }
        }
        level++;
    }
}

template <class T>
void print_tree( ntreeNode<T> * root, int max_depth=-1,
                 std::ostream& cout=std::cout, int depth=0 ) {
    for ( int i=0; i<depth; i++ ) cout << '-';
    cout << " " << root->data << std::endl;
    if (depth==max_depth) return;
    ntreeNode<T> * child = root->firstChild;
    while (child != NULL) {
        print_tree( child, max_depth, cout, depth+1 );
        child = child->sibling;
    }
}
/*
template <class T>
void delete_node( ntreeNode<T> ** node ) {
    ntreeNode<T> * current = (*node)->firstChild, * next;
    while (current != NULL) {
        next = current->sibling;
        delete_node(&current);
        current = next;
    }
   *node = NULL;
    delete *node;
}
*/
//template class ntreeNode<int32_t>;


#endif
