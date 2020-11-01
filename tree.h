#ifndef __NTREE_H
#define __NTREE_H

#include <vector>
#include <iostream>

#include <queue>


template <class T>
struct treeNode {
    T data;
    treeNode<T> * sibling, * firstChild;
    treeNode<T>() {};
    treeNode<T>(T newData) {
        data = newData;
        sibling = NULL;
        firstChild = NULL;
    }
    ~treeNode<T>();
};

template <class T>
treeNode<T>::~treeNode() {
    treeNode<T> * child, * next;
    child = firstChild;
    while (child != NULL) {
        next = child->sibling;
        delete child;
        child = next;
    }
}

template <class T>
void delete_child( treeNode<T> * parent, treeNode<T> * child ) {
    if (parent == NULL || child == NULL) return;
    treeNode<T> * p, * prev;
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
treeNode<T> * choose_child( treeNode<T> * parent, treeNode<T> * child ) {
    if (parent == NULL || child == NULL) return parent;
    treeNode<T> * p = parent->firstChild, ret;
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
    treeNode<T> * next;
    while (p != NULL) {
        next = p->sibling;
        if (p != child) delete p;
        p = next;
    }
    return child;
}

template <class T>
void add_child( treeNode<T> * parent, treeNode<T> * child ) {
    if ( parent->firstChild == NULL) {
        parent->firstChild = child;
    }
    else {
        treeNode<T> * p = parent->firstChild;
        while ( p->sibling != NULL ) p = p->sibling;
        p->sibling = child;
    }
}

template <class T>
void add_child( treeNode<T> * parent, T data ) {
    treeNode<T> * child = new treeNode<T>(data);
    add_child(parent,child);
}

template <class T>
void add_sibling( treeNode<T> * node, treeNode<T> * sib ) {
    treeNode<T> * p = node;
    while ( p->sibling != NULL ) p = p->sibling;
    p->sibling = sib;
}

template <class T>
void add_sibling( treeNode<T> * node, T data ) {
    treeNode<T> * sib = new treeNode<T>(data);
    add_sibling(node,sib);
}

template <class T>
void get_leaves( treeNode<T> * root, T ** arr ) {
    if ( root == NULL ) return;
    else if ( root->firstChild == NULL ) {
        **arr = root->data;
        (*arr)++;
    }
    else {
        treeNode<T> * child = root->firstChild;
        while ( child != NULL ) {
            get_leaves(child,arr);
            child = child->sibling;
        }
    }
}

template <class T>
void get_leaves( treeNode<T> * root, std::vector<T> * vec ) {
    if ( root == NULL ) return;
    else if ( root->firstChild == NULL ) {
        (*vec).push_back(root->data);
    }
    else {
        treeNode<T> * child = root->firstChild;
        while ( child != NULL ) {
            get_leaves(child,vec);
            child = child->sibling;
        }
    }
}

template <class T>
int num_nodes( treeNode<T> * root ) {
    if (root==NULL) return 0;
    int ret = 1;
    treeNode<T> * child = root->firstChild;
    while (child != NULL) {
        ret += num_nodes(child);
        child = child->sibling;
    }
    return ret;
}

template <class T>
void num_nodes_per_level( treeNode<T> * root ) {
    int level = 0;
    std::queue<treeNode<T>*> Q;
    Q.push(root);
    int qlen,i;
    treeNode<T> * child, * v, * p;

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
void print_tree( treeNode<T> * root, int max_depth=-1,
                 std::ostream& cout=std::cout, int depth=0 ) {
    for ( int i=0; i<depth; i++ ) cout << '-';
    cout << " " << root->data << std::endl;
    if (depth==max_depth) return;
    treeNode<T> * child = root->firstChild;
    while (child != NULL) {
        print_tree( child, max_depth, cout, depth+1 );
        child = child->sibling;
    }
}
/*
template <class T>
void delete_node( treeNode<T> ** node ) {
    treeNode<T> * current = (*node)->firstChild, * next;
    while (current != NULL) {
        next = current->sibling;
        delete_node(&current);
        current = next;
    }
   *node = NULL;
    delete *node;
}
*/
//template class treeNode<int32_t>;


#endif
