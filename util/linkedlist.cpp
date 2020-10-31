#include "linkedlist.h"
#include <cstddef>
#include <limits>
#include <iostream>
#include <string>

template <class T>
struct node <T> * newNode() {
    node <T> * new_node = new node<T>;
    new_node->next = NULL;
    return new_node;
}

template <class T>
struct node <T> * linkedlist<T>::get_tail() {
    if (head == NULL) throw;

    node <T> * p = head;
    while (p->next != NULL) p = p->next;

    return p;
}

template <class T>
linkedlist<T>::linkedlist() {
    head = NULL;
}

template <class T>
void linkedlist<T>::append(T tailData) {
    node <T> * tail = newNode<T>();
    tail->data = tailData;
    tail->next = NULL;
    if (head == NULL) head = tail;
    else {
        node <T> * p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = tail;
    }
}

template <class T>
void linkedlist<T>::prepend(T headData) {
    node <T> * newHead = newNode<T>();
    newHead->data = headData;
    if (head == NULL) head = newHead;
    else {
        newHead->next = head;
        head = newHead;
    }
}

template <class T>
void linkedlist<T>::insertRight(int nodeIndex, T nodeData) {
    if (nodeIndex < 0) throw;

    node <T> * new_node = newNode<T>();
    new_node->data = nodeData;
    if (head == NULL) {
        if (nodeIndex == 0) {
            new_node->next = NULL;
            head = new_node;
            return;
        }
        else throw;
    }

    node <T> * p = head;
    int i = 0;

    while (p != NULL && i != nodeIndex) {
        p = p->next;
        i++;
    }

    if (i == nodeIndex) {
        new_node->next = p->next;
        p->next = new_node;
    }

    else throw;
}

template <class T>
void linkedlist<T>::insertLeft(int nodeIndex, T nodeData) {
    if (nodeIndex < 0) throw;

    else if (nodeIndex == 0) {
        prepend(nodeData);
    }

    else {
        if (head == NULL) throw;
        else insertRight(nodeIndex - 1, nodeData);
    }
}

template <class T>
void linkedlist<T>::deleteIndex(int nodeIndex) {
    if (nodeIndex < 0) throw;
    else if (head == NULL) throw;

    else if (nodeIndex == 0) {
        if (head->next == NULL) {
            head = NULL;
        }
        else {
            head = head->next;
        }
        return;
    }

    node <T> * p = head;
    int i = 0;

    while (p != NULL) {
        if (i == nodeIndex - 1) {
            if (p->next == NULL) throw;
            p->next = p->next->next;
            return;
        }
        p = p->next;
        i++;
    }
    throw;
}

template <class T>
int linkedlist<T>::linear_search(T searchData) {
    if (head == NULL) throw;

    node <T> * p = head;
    int i = 0;

    while (p != NULL) {
        if (p->data == searchData) return i;
        p = p->next;
        i++;
    }
    throw;
}

template <class T>
int linkedlist<T>::len() {
    if (head == NULL) return 0;

    node <T> * p = head;
    int i = 1;

    while (p->next != NULL) {
        p = p->next;
        i++;
    }

    return i;
}

template <class T>
T linkedlist<T>::min() {
    if (head == NULL) throw;

    T ret = std::numeric_limits<T>::max();
    node <T> * p = head;

    while (p != NULL) {
        if (p->data < ret) ret = p->data;
        p = p->next;
    }

    return ret;
}

template <class T>
T linkedlist<T>::max() {
    if (head == NULL) throw;

    T ret = std::numeric_limits<T>::min();
    node <T> * p = head;

    while (p != NULL) {
        if (p->data > ret) ret = p->data;
        p = p->next;
    }

    return ret;
}

template <class T>
T linkedlist<T>::get_item(int nodeIndex) {
    if (nodeIndex < 0) throw;
    if (head == NULL) throw;

    node <T> * p = head;
    int i = 0;

    while (p != NULL) {
        if (i == nodeIndex) return p->data;
        p = p->next;
        i++;
    }

    throw;
}

template <class T>
bool linkedlist<T>::is_empty() {
    return ( head == NULL);
}

template <class T>
void linkedlist<T>::set_item(int nodeIndex, T nodeData) {
    if (nodeIndex < 0 || head == NULL) throw;

    int i = 0;
    node <T> * p = head;

    while (p != NULL) {
        if (i == nodeIndex) {
            p->data = nodeData;
            return;
        }
        p = p->next;
        i++;
    }

    throw;
}

template <class T>
void linkedlist<T>::swap(int index1, int index2) {
    if (index1 < 0 || index2 < 0) throw;
    if (head == NULL) throw;

    int ind1, ind2;
    ind1 = std::min(index1, index2);
    ind2 = std::max(index1, index2);
    //  ind2 = index1 + index2 - ind1;

    node <T> * q;
    node <T> * p = head;
    int j, i = 0;
    T tmp;

    while (p != NULL) {
        if (i == ind1) break;
        p = p->next;
        i++;
    }

    q = p;
    j = i;

    while (q != NULL) {
        if (j == ind2) break;
        q = q->next;
        j++;
    }

    if (p == NULL || q == NULL) throw;

    tmp = q->data;
    q->data = p->data;
    p->data = tmp;
}

template <class T>
void linkedlist<T>::bubbleSort() {
    if (head == NULL || head->next == NULL) return;
    int length = len();
    int i, j;
    node <T> * p, * q;
    T tmp;

    for (i = length - 1; i > 0; i--) {
        p = head;
        for (j = 0; j < i; j++) {
            if (p->data > p->next->data) {
                tmp = p->next->data;
                p->next->data = p->data;
                p->data = tmp;
            }
            p = p->next;
        }
    }
}

template <class T>
void linkedlist<T>::selectionSort() {
    if (head == NULL || head->next == NULL) return;

    T tmp;
    node <T> * p = head;
    node <T> * q;
    node <T> * minim;

    while (p->next != NULL) {
        q = p->next;
        minim = p;
        while (q != NULL) {
            if (q->data < minim->data) minim = q;
            q = q->next;
        }

        tmp = minim->data;
        minim->data = p->data;
        p->data = tmp;

        p = p->next;
    }
}

template <class T>
void linkedlist<T>::insertionSort() {
    if (head == NULL || head->next == NULL) return;

    node <T> * p = head->next;
    node <T> * prev = head;
    node <T> * q;
    node <T> * tmp;

    while (p != NULL) {
        if (prev->data <= p->data) {
            p = p->next;
            prev = prev->next;
            //print();
            continue;
        }
        if (p->data <= head->data) {
            prev->next = p->next;
            p->next = head;
            head = p;
            p = prev->next;
            continue;
        }
        q = head;
        while (p->data >= q->next->data) q = q->next;
        if (q == head) {
            prev->next = p->next;
            p->next = head->next;
            head->next = p;
            p = prev->next;
        }
        else if (q->next->next == p) {
            prev->next = p->next;
            q->next = p;
            p->next = prev;
            p = prev->next;
        }
        else {
            tmp = q->next;
            q->next = p;
            p = p->next;
            q->next->next = tmp;
            prev->next = p;
        }
        //print();
    }
}

template <class T>
void linkedlist<T>::print() {
    node <T> * p = head;
    while (p != NULL) {
        std::cout << p->data << ", ";
        p = p->next;
    }
    std::cout << "\n";
}

template class linkedlist<char>;
template class linkedlist<int>;
template class linkedlist<std::string>;