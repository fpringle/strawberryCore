#include "doublylinkedlist.h"
#include "stack.h"
#include <cstddef>
#include <limits>
#include <iostream>



template <class T>
stack<T>::stack( ) {
  doublylinkedlist<T> lst;
}

template <class T>
void stack<T>::enstack( T data ) {
  lst.prepend( data );
}

template <class T>
void stack<T>::destack( T * target ) {
  *target = lst.get_item( 0 );
  lst.deleteIndex( 0 );
}

template <class T>
T stack<T>::destack( ) {
  T ret = lst.get_item( 0 );
  lst.deleteIndex( 0 );
  return ret;
}

template <class T>
T stack<T>::peek( ) {
  T ret = lst.get_item( 0 );
  return ret;
}

template <class T>
void stack<T>::print( ) {
  lst.print( );
}


template class stack<int>;
template class stack<char>;