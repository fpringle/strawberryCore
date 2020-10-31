#include "doublylinkedlist.h"
#include "tree.h"
#include "ntree.h"
#include <cstddef>
#include <limits>
#include <iostream>

template <class T>
struct binode <T> * newBiNode() {
    binode <T> * new_binode = new binode<T>;
    new_binode->prev = NULL;
    new_binode->next = NULL;
    return new_binode;
}

template <class T>
doublylinkedlist<T>::doublylinkedlist() {
    head = NULL;
    tail = NULL;
}

template <class T>
void doublylinkedlist<T>::append(T tailData) {
    binode <T> * new_tail = newBiNode<T>();
    new_tail->data = tailData;
    if (head == NULL) {
        new_tail->prev = NULL;
        new_tail->next = NULL;
        head = new_tail;
        tail = new_tail;
    }
    else {
        new_tail->next = NULL;
        tail->next = new_tail;
        new_tail->prev = tail;
        tail = new_tail;
    }
}

template <class T>
void doublylinkedlist<T>::prepend(T headData) {
    binode <T> * new_head = newBiNode<T>();
    new_head->data = headData;
    if (head == NULL) {
        new_head->prev = NULL;
        new_head->next = NULL;
        head = new_head;
        tail = new_head;
    }
    else {
        new_head->prev = NULL;
        new_head->next = head;
        head->prev = new_head;
        head = new_head;
    }
}

template <class T>
void doublylinkedlist<T>::insertRight(int binodeIndex, T binodeData) {
    if (binodeIndex < 0) throw;

    binode <T> * new_binode = newBiNode<T>();
    new_binode->data = binodeData;
    if (head == NULL) {
        if (binodeIndex == 0) {
            new_binode->next = NULL;
            new_binode->prev = NULL;
            head = new_binode;
            tail = new_binode;
            return;
        }
        else throw;
    }

    binode <T> * p = head;
    int i = 0;

    while (p != NULL) {
        if (i == binodeIndex) {
            new_binode->next = p->next;
            if (p->next != NULL) p->next->prev = new_binode;
            else tail = new_binode;
            new_binode->prev = p;
            p->next = new_binode;
            return;
        }
        p = p->next;
        i++;
    }
    throw;
}

template <class T>
void doublylinkedlist<T>::insertLeft(int binodeIndex, T binodeData) {
    if (binodeIndex < 0) throw;

    else if (binodeIndex == 0) {
        prepend(binodeData);
    }

    else {
        if (head == NULL) throw;
        else insertRight(binodeIndex - 1, binodeData);
    }
}

template <class T>
void doublylinkedlist<T>::deleteIndex(int binodeIndex) {
    if (binodeIndex < 0) throw;
    else if (head == NULL) throw;

    else if (binodeIndex == 0) {
        if (head->next == NULL) {
            head = NULL;
            tail = NULL;
        }
        else {
            head = head->next;
            head->prev = NULL;
        }
        return;
    }

    binode <T> * p = head;
    int i = 0;

    while (p != NULL) {
        if (i == binodeIndex - 1) {
            if (p->next == NULL) throw;
            p->next = p->next->next;
            if (p->next != NULL) p->next->prev = p;
            else tail = p;
            return;
        }
        p = p->next;
        i++;
    }
    throw;
}

template <class T>
int doublylinkedlist<T>::linear_search(T searchData) {
    if (head == NULL) throw;

    binode <T> * p = head;
    int i = 0;

    while (p != NULL) {
        if (p->data == searchData) return i;
        p = p->next;
        i++;
    }
    throw;
}

template <class T>
int doublylinkedlist<T>::len() {
    if (head == NULL) return 0;

    binode <T> * p = head;
    int i = 1;

    while (p->next != NULL) {
        p = p->next;
        i++;
    }

    return i;
}


// FIX THESE

template <class T>
T doublylinkedlist<T>::min() {
    if (head == NULL) throw;

    T ret = std::numeric_limits<T>::max();
    //T ret = T::limit_max();
    binode <T> * p = head;

    while (p != NULL) {
        if (p->data < ret) ret = p->data;
        p = p->next;
    }

    return ret;
}

template <class T>
T doublylinkedlist<T>::max() {
    if (head == NULL) throw;

    T ret = std::numeric_limits<T>::min();
    //T ret = T::limit_min();
    binode <T> * p = head;

    while (p != NULL) {
        if (p->data > ret) ret = p->data;
        p = p->next;
    }

    return ret;
}

template <class T>
T doublylinkedlist<T>::get_item(int binodeIndex) {
    if (binodeIndex < 0) throw;
    if (head == NULL) throw;

    binode <T> * p = head;
    int i = 0;

    while (p != NULL) {
        if (i == binodeIndex) return p->data;
        p = p->next;
        i++;
    }

    throw;
}

template <class T>
bool doublylinkedlist<T>::is_empty() {
    return ( head == NULL);
}

template <class T>
void doublylinkedlist<T>::set_item(int binodeIndex, T binodeData) {
    if (binodeIndex < 0 || head == NULL) throw;

    int i = 0;
    binode <T> * p = head;

    while (p != NULL) {
        if (i == binodeIndex) {
            p->data = binodeData;
            return;
        }
        p = p->next;
        i++;
    }

    throw;
}

template <class T>
void doublylinkedlist<T>::swap(int index1, int index2) {
    if (index1 < 0 || index2 < 0) throw;
    if (head == NULL) throw;

    int ind1, ind2;
    ind1 = std::min(index1, index2);
    ind2 = std::max(index1, index2);

    binode <T> * q;
    binode <T> * p = head;

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
void doublylinkedlist<T>::bubbleSort() {
    if (head == NULL || head->next == NULL) return;
    int length = len();
    int i, j;
    binode <T> * p;
    binode <T> * q;
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
void doublylinkedlist<T>::selectionSort() {
    if (head == NULL || head->next == NULL) return;

    T tmp;
    binode <T> * p = head;
    binode <T> * q;
    binode <T> * minim;

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
void doublylinkedlist<T>::insertionSort() {
    if (head == NULL || head->next == NULL) return;

    binode <T> * p = head->next;
    binode <T> * q;
    T tmp;

    while (p != NULL) {
        if (p->data >= p->prev->data) {
            p = p->next;
            continue;
        }
        q = p;
        p = p->next;
        while (q->prev != NULL && q->prev->data > q->data) {
            tmp = q->prev->data;
            q->prev->data = q->data;
            q->data = tmp;
            q = q->prev;
        }
    }
}

template <class T>
void doublylinkedlist<T>::print() {
    binode <T> * p = head;
    while (p != NULL) {
        std::cout << p->data << ", ";
        p = p->next;
    }
    std::cout << "\n";
}

template <class T>
void doublylinkedlist<T>::headtail() {
    if (head != NULL && tail != NULL) {
        std::cout << "head: " << head->data << ", "
                  << "tail: " << tail->data << "\n";
    }
}



template class doublylinkedlist<int>;
template class doublylinkedlist<char>;
template class doublylinkedlist<std::string>;
template class doublylinkedlist<treenode<int>*>;
template class doublylinkedlist<treenode<char>*>;
template class doublylinkedlist<treenode<std::string>*>;
template class doublylinkedlist<ntreeNode<int>*>;
