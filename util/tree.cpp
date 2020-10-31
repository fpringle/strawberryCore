#include "linkedlist.h"
#include "doublylinkedlist.h"
#include "tree.h"
#include "queue.h"
#include <cstddef>
#include <limits>
#include <iostream>
#include <string>

template <class T>
struct treenode <T> * newTreeNode() {
    treenode<T> * new_tree_node = new treenode<T>;
    new_tree_node->left = NULL;
    new_tree_node->right = NULL;
    return new_tree_node;
}

template <class T>
bool is_leaf_node(struct treenode <T> * node) {
    return ( node->left == NULL && node->right == NULL);
}

template <class T>
void insert_after_leaf_left(treenode <T> * leaf, T newData) {
    //if ( ! is_leaf_node( leaf ) ) throw;
    leaf->left = newTreeNode<T>();
    leaf->left->data = newData;
}

template <class T>
void insert_after_leaf_right(treenode <T> * leaf, T newData) {
    //if ( ! is_leaf_node( leaf ) ) throw;
    leaf->right = newTreeNode<T>();
    leaf->right->data = newData;
}

template <class T>
void get_leaves(treenode <T> * root, linkedlist <T> * target) {
    if (root == NULL) {
    }
    else if (is_leaf_node(root)) {
        (*target).append(root->data);
    }
    else {
        get_leaves(root->left, target);
        get_leaves(root->right, target);
    }
}

template <class T>
void append(treenode <T> ** root, T newData) {
    treenode <T> * new_node = newTreeNode<T>();
    new_node->data = newData;

    if (*root == NULL) {
        *root = new_node;
        return;
    }

    queue<treenode <T> *> q;
    q.enqueue(*root);
    treenode <T> * node;

    while (!q.is_empty()) {
        node = q.dequeue();
        if (node->left == NULL) {
            node->left = new_node;
            return;
        }
        else if (node->right == NULL) {
            node->right = new_node;
            return;
        }
        else {
            q.enqueue(node->left);
            q.enqueue(node->right);
        }
    }
}

template <class T>
void tree<T>::print_leaves() {
    linkedlist<T> leaves;
    get_leaves(root, &leaves);
    std::cout << "leaves: ";
    leaves.print();
}

template <class T>
tree<T>::tree() {
    root = NULL;
}

template <class T>
tree<T>::tree(T rootData) {
    root = newTreeNode<T>();
    root->data = rootData;
}

template <class T>
void tree<T>::insert(T newData) {
    append(&root, newData);
}


template class tree<int>;
template class tree<char>;
template class tree<std::string>;