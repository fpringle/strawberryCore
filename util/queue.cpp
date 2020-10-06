#include "doublylinkedlist.h"
#include "linkedlist.h"
#include "queue.h"
#include <cstddef>
#include <limits>
#include <iostream>
#include "tree.h"
#include <string>
#include "ntree.h"

template <class T>
queue<T>::queue ( ) {
  doublylinkedlist <T> lst;
}

template <class T>
void queue<T>::enqueue( T data ) {
  lst.append( data );
}

template <class T>
void queue<T>::dequeue( T * target ) {
  *target = lst.get_item( 0 );
  lst.deleteIndex( 0 );
}

template <class T>
T queue<T>::dequeue( ) {
  T ret = lst.get_item( 0 );
  lst.deleteIndex( 0 );
  return ret;
}

template <class T>
T queue<T>::peek( ) {
  T ret = lst.get_item( 0 );
  return ret;
}

template <class T>
bool queue<T>::is_empty( ) {
  return lst.is_empty();
}

template <class T>
void queue<T>::print( ) {
  lst.print( );
}

template <class T>
int queue<T>::len( ) {
    return lst.len();
}

template class queue<int>;
template class queue<char>;
template class queue<std::string>;
template class queue<treenode<int>*>;
template class queue<treenode<char>*>;
template class queue<treenode<std::string>*>;
template class queue<ntreeNode<int>*>;
