#include "structures.h"
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <limits.h>

template <class T>
struct node <T> * linkedlist<T>::get_tail( ) {
  if ( head == NULL ) throw;

  node <T> * p = head;
  while ( p->next != NULL ) p = p->next;

  return p;
}

template <class T>
linkedlist<T>::linkedlist( ) {
  head = NULL;
}

template <class T>
void linkedlist<T>::append( T tailData ) {
  node <T> * tail = newNode<T>();
  tail->data = tailData;
  tail->next = NULL;
  if ( head == NULL ) head = tail;
  else {
    node <T> * p = head;
    while ( p->next != NULL ) {
      p = p->next;
    }
    p->next = tail;
  }
}

template <class T>
void linkedlist<T>::prepend( T headData ) {
  node <T> * newHead = newNode<T>();
  newHead->data = headData;
  if ( head == NULL ) head = newHead;
  else {
    newHead->next = head;
    head = newHead;
  }
}

template <class T>
void linkedlist<T>::insertRight( int nodeIndex, T nodeData ) {
  if ( nodeIndex < 0 ) throw;

  node <T> * new_node = newNode<T>();
  new_node->data = nodeData;
  if ( head == NULL ) {
    if ( nodeIndex == 0 ) {
      new_node->next = NULL;
      head = new_node;
      return;
    }
    else throw;
  }

  node <T> * p = head;
  int i=0;

  while ( p != NULL && i != nodeIndex) {
    p = p->next;
    i++;
  }

  if ( i == nodeIndex ) {
    new_node->next = p->next;
    p->next = new_node;
  }

  else throw;
}

template <class T>
void linkedlist<T>::insertLeft( int nodeIndex, T nodeData ) {
  if ( nodeIndex < 0 ) throw;

  else if ( nodeIndex == 0 ) {
    prepend( nodeData );
  }

  else {
    if ( head == NULL ) throw;
    else insertRight( nodeIndex-1, nodeData );
  }
}

template <class T>
void linkedlist<T>::deleteIndex( int nodeIndex ) {
  if ( nodeIndex < 0 ) throw;
  else if ( head == NULL ) throw;

  else if ( nodeIndex == 0 ) {
    if ( head->next == NULL ) {
      head = NULL;
    }
    else {
      head = head->next;
    }
    return;
  }

  node <T> * p = head;
  int i = 0;

  while ( p != NULL ) {
    if ( i == nodeIndex-1 ) {
      if ( p->next == NULL ) throw;
      p->next = p->next->next;
      return;
    }
    p = p->next;
    i++;
  }
  throw;
}

template <class T>
int linkedlist<T>::linear_search( T searchData ) {
  if ( head == NULL ) throw;

  node <T> * p = head;
  int i=0;

  while ( p != NULL ) {
    if ( p->data == searchData ) return i;
    p = p->next;
    i++;
  }
  throw;
}

template <class T>
int linkedlist<T>::len( ) {
  if ( head == NULL ) return 0;

  node <T> * p = head;
  int i=1;

  while ( p->next != NULL ) {
    p = p->next;
    i++;
  }

  return i;
}

template <class T>
T linkedlist<T>::min( ) {
  if ( head == NULL ) throw;

  T ret = std::numeric_limits<T>::max();
  node <T> * p = head;

  while ( p != NULL ) {
    if ( p->data < ret ) ret = p->data;
    p = p->next;
  }

  return ret;
}

template <class T>
T linkedlist<T>::max( ) {
  if ( head == NULL ) throw;

  T ret = std::numeric_limits<T>::min();
  node <T> * p = head;

  while ( p != NULL ) {
    if ( p->data > ret ) ret = p->data;
    p = p->next;
  }

  return ret;
}

template <class T>
T linkedlist<T>::get_item( int nodeIndex ) {
  if ( nodeIndex < 0 ) throw;
  if ( head == NULL ) throw;

  node <T> * p = head;
  int i=0;

  while ( p != NULL ) {
    if ( i == nodeIndex ) return p->data;
    p = p->next;
    i++;
  }

  throw;
}

template <class T>
bool linkedlist<T>::is_empty( ) {
  return ( head == NULL );
}

template <class T>
void linkedlist<T>::set_item( int nodeIndex, T nodeData ) {
  if ( nodeIndex < 0 || head == NULL ) throw;

  int i=0;
  node <T> * p = head;

  while ( p != NULL ) {
    if ( i == nodeIndex ) {
      p->data = nodeData;
      return;
    }
    p = p->next;
    i++;
  }

  throw;
}

template <class T>
void linkedlist<T>::swap ( int index1, int index2 ) {
  if ( index1 < 0 || index2 < 0 ) throw;
  if ( head == NULL ) throw;

  int ind1,ind2;
  ind1 = std::min( index1, index2 );
  ind2 = std::max( index1, index2 );
//  ind2 = index1 + index2 - ind1;

  node <T> * q;
  node <T> * p = head;
  int j, i = 0;
  T tmp;

  while ( p != NULL ) {
    if ( i == ind1 ) break;
    p = p->next;
    i++;
  }

  q = p;
  j = i;

  while ( q != NULL ) {
    if ( j == ind2 ) break;
    q = q->next;
    j++;
  }

  if ( p == NULL || q == NULL ) throw;

  tmp = q->data;
  q->data = p->data;
  p->data = tmp;
}

template <class T>
void linkedlist<T>::bubbleSort( ) {
  if ( head == NULL || head->next == NULL ) return;
  int length = len();
  int i,j;
  node <T> * p, * q;
  T tmp;

  for ( i=length-1; i>0; i-- ) {
    p = head;
    for ( j=0; j<i; j++ ) {
      if ( p->data > p->next->data ) {
        tmp = p->next->data;
        p->next->data = p->data;
        p->data = tmp;
      }
      p = p->next;
    }
  }
}

template <class T>
void linkedlist<T>::selectionSort( ) {
  if ( head == NULL || head->next == NULL ) return;

  T tmp;
  node <T> * p = head;
  node <T> * q;
  node <T> * minim;

  while ( p->next != NULL ) {
    q = p->next;
    minim = p;
    while ( q != NULL ) {
      if ( q->data < minim->data ) minim = q;
      q = q->next;
    }

    tmp = minim->data;
    minim->data = p->data;
    p->data = tmp;

    p = p->next;
  }
}

template <class T>
void linkedlist<T>::insertionSort( ) {
  if ( head == NULL || head->next == NULL ) return;

  node <T> * p = head->next;
  node <T> * prev = head;
  node <T> * q;
  node <T> * tmp;

  while ( p != NULL ) {
    if ( prev->data <= p->data ) {
      p = p->next;
      prev = prev->next;
      //print();
      //std::cout << "head: " << head->data << ", p: " << p->data << ", prev: " << prev->data << ", next: " << p->next->data << ", prev->next: " << prev->next->data << "\n\n";
      continue;
    }
    if ( p->data <= head->data ) {
      prev->next = p->next;
      p->next = head;
      head = p;
      p = prev->next;
      continue;
    }
    q = head;
    while ( p->data >= q->next->data ) q = q->next;
    if ( q == head ) {
      prev->next = p->next;
      p->next = head->next;
      head->next = p;
      p = prev->next;
    }
    else if ( q->next->next == p ) {
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
  //std::cout << "head: " << head->data << ", p: " << p->data << ", prev: " << prev->data << ", next: " << p->next->data << ", prev->next: " << prev->next->data << "\n\n";
  }
}

template <class T>
void linkedlist<T>::print( ) {
  node <T> * p = head;
  while ( p != NULL ) {
    std::cout << p->data << ", ";
    p = p->next;
  }
  std::cout << "\n";
}

template class linkedlist<char>;
template class linkedlist<int>;

template <class T>
doublylinkedlist<T>::doublylinkedlist( ) {
  head = NULL;
  tail = NULL;
}

template <class T>
void doublylinkedlist<T>::append( T tailData ) {
  binode <T> * new_tail = newBiNode<T>();
  new_tail->data = tailData;
  if ( head == NULL) {
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
void doublylinkedlist<T>::prepend( T headData ) {
  binode <T> * new_head = newBiNode<T>();
  new_head->data = headData;
  if ( head == NULL) {
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
void doublylinkedlist<T>::insertRight( int binodeIndex, T binodeData ) {
  if ( binodeIndex < 0 ) throw;

  binode <T> * new_binode = newBiNode<T>();
  new_binode->data = binodeData;
  if ( head == NULL ) {
    if ( binodeIndex == 0 ) {
      new_binode->next = NULL;
      new_binode->prev = NULL;
      head = new_binode;
      tail = new_binode;
      return;
    }
    else throw;
  }

  binode <T> * p = head;
  int i=0;

  while ( p != NULL ) {
    if ( i == binodeIndex ) {
      new_binode->next = p->next;
      if ( p->next != NULL ) p->next->prev = new_binode;
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
void doublylinkedlist<T>::insertLeft( int binodeIndex, T binodeData ) {
  if ( binodeIndex < 0 ) throw;

  else if ( binodeIndex == 0 ) {
    prepend( binodeData );
  }

  else {
    if ( head == NULL ) throw;
    else insertRight( binodeIndex-1, binodeData );
  }
}

template <class T>
void doublylinkedlist<T>::deleteIndex( int binodeIndex ) {
  if ( binodeIndex < 0 ) throw;
  else if ( head == NULL ) throw;

  else if ( binodeIndex == 0 ) {
    if ( head->next == NULL ) {
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

  while ( p != NULL ) {
    if ( i == binodeIndex-1 ) {
      if ( p->next == NULL ) throw;
      p->next = p->next->next;
      if ( p->next != NULL ) p->next->prev = p;
      else tail = p;
      return;
    }
    p = p->next;
    i++;
  }
  throw;
}

template <class T>
int doublylinkedlist<T>::linear_search( T searchData ) {
  if ( head == NULL ) throw;

  binode <T> * p = head;
  int i=0;

  while ( p != NULL ) {
    if ( p->data == searchData ) return i;
    p = p->next;
    i++;
  }
  throw;
}

template <class T>
int doublylinkedlist<T>::len( ) {
  if ( head == NULL ) return 0;

  binode <T> * p = head;
  int i=1;

  while ( p->next != NULL ) {
    p = p->next;
    i++;
  }

  return i;
}


// FIX THESE
template <class T>
T doublylinkedlist<T>::min( ) {
  if ( head == NULL ) throw;

  T ret = std::numeric_limits<T>::max();
  //T ret = T::limit_max();
  binode <T> * p = head;

  while ( p != NULL ) {
    if ( p->data < ret ) ret = p->data;
    p = p->next;
  }

  return ret;
}

template <class T>
T doublylinkedlist<T>::max( ) {
  if ( head == NULL ) throw;

  T ret = std::numeric_limits<T>::min();
  //T ret = T::limit_min();
  binode <T> * p = head;

  while ( p != NULL ) {
    if ( p->data > ret ) ret = p->data;
    p = p->next;
  }

  return ret;
}

template <class T>
T doublylinkedlist<T>::get_item( int binodeIndex ) {
  if ( binodeIndex < 0 ) throw;
  if ( head == NULL ) throw;

  binode <T> * p = head;
  int i=0;

  while ( p != NULL ) {
    if ( i == binodeIndex ) return p->data;
    p = p->next;
    i++;
  }

  throw;
}

template <class T>
bool doublylinkedlist<T>::is_empty( ) {
  return ( head == NULL );
}

template <class T>
void doublylinkedlist<T>::set_item( int binodeIndex, T binodeData ) {
  if ( binodeIndex < 0 || head == NULL ) throw;

  int i=0;
  binode <T> * p = head;

  while ( p != NULL ) {
    if ( i == binodeIndex ) {
      p->data = binodeData;
      return;
    }
    p = p->next;
    i++;
  }

  throw;
}

template <class T>
void doublylinkedlist<T>::swap ( int index1, int index2 ) {
  if ( index1 < 0 || index2 < 0 ) throw;
  if ( head == NULL ) throw;

  int ind1,ind2;
  ind1 = std::min( index1, index2 );
  ind2 = std::max( index1, index2 );

  binode <T> * q;
  binode <T> * p = head;
  
  int j, i = 0;
  T tmp;

  while ( p != NULL ) {
    if ( i == ind1 ) break;
    p = p->next;
    i++;
  }

  q = p;
  j = i;

  while ( q != NULL ) {
    if ( j == ind2 ) break;
    q = q->next;
    j++;
  }

  if ( p == NULL || q == NULL ) throw;

  tmp = q->data;
  q->data = p->data;
  p->data = tmp;
}

template <class T>
void doublylinkedlist<T>::bubbleSort( ) {
  if ( head == NULL || head->next == NULL ) return;
  int length = len();
  int i,j;
  binode <T> * p;
  binode <T> * q;
  T tmp;

  for ( i=length-1; i>0; i-- ) {
    p = head;
    for ( j=0; j<i; j++ ) {
      if ( p->data > p->next->data ) {
        tmp = p->next->data;
        p->next->data = p->data;
        p->data = tmp;
      }
      p = p->next;
    }
  }
}

template <class T>
void doublylinkedlist<T>::selectionSort( ) {
  if ( head == NULL || head->next == NULL ) return;

  T tmp;
  binode <T> * p = head;
  binode <T> * q;
  binode <T> * minim;

  while ( p->next != NULL ) {
    q = p->next;
    minim = p;
    while ( q != NULL ) {
      if ( q->data < minim->data ) minim = q;
      q = q->next;
    }

    tmp = minim->data;
    minim->data = p->data;
    p->data = tmp;

    p = p->next;
  }
}

template <class T>
void doublylinkedlist<T>::insertionSort( ) {
  if ( head == NULL || head->next == NULL ) return;

  binode <T> * p = head->next;
  binode <T> * q;
  T tmp;

  while ( p != NULL ) {
    if ( p->data >= p->prev->data ) {
      p = p->next;
      continue;
    }
    q = p;
    p = p->next;
    while ( q->prev != NULL && q->prev->data > q->data ) {
      tmp = q->prev->data;
      q->prev->data = q->data;
      q->data = tmp;
      q = q->prev;
    }
  }
}

template <class T>
void doublylinkedlist<T>::print( ) {
  binode <T> * p = head;
  while ( p != NULL ) {
  std::cout << p->data << ", ";
    p = p->next;
  }
  std::cout << "\n";
}

template <class T>
void doublylinkedlist<T>::headtail( ) {
  if ( head != NULL && tail != NULL ) std::cout << "head: " << head->data << ", tail: " << tail->data << "\n";
}



template class doublylinkedlist<int>;
template class doublylinkedlist<char>;


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

template class queue<int>;
template class queue<char>;

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


template <class T>
bool is_leaf_node( struct treenode <T> * node ) {
  return ( node->left == NULL && node->right == NULL );
}

template <class T>
void insert_after_leaf_left( treenode <T> * leaf, T newData ) {
  //if ( ! is_leaf_node( leaf ) ) throw;
  leaf->left = newTreeNode<T>( );
  leaf->left->data = newData;
}

template <class T>
void insert_after_leaf_right( treenode <T> * leaf, T newData ) {
  //if ( ! is_leaf_node( leaf ) ) throw;
  leaf->right = newTreeNode<T>( );
  leaf->right->data = newData;
}

template <class T>
void get_leaves( treenode <T> * root, linkedlist <T> * target ) {
  if ( root == NULL ) {}
  else if ( is_leaf_node( root ) ) {
    (*target).append( root->data );
  }
  else {
    get_leaves( root->left, target );
    get_leaves( root->right, target );
  }
}

template <class T>
void append( treenode <T> ** root, T newData ) {
  treenode <T> * new_node = newTreeNode<T>( );
  new_node->data = newData;
  
  if ( *root == NULL ) {
    *root = new_node;
    return;
  }
  
  queue<treenode <T> *> q;
  q.enqueue( *root );
  treenode <T> * node;
  
  while ( ! q.is_empty( ) ) {
    node = q.dequeue( );
    if ( node->left == NULL ) {
      node->left = new_node;
      return;
    }
    else if ( node->right == NULL ) {
      node->right = new_node;
      return;
    }
    else {
      q.enqueue( node->left );
      q.enqueue( node->right );
    }
  }
}

template <class T>
void tree<T>::print_leaves( ) {
      linkedlist<T> leaves;
      get_leaves( root, &leaves );
      std::cout << "leaves: ";
      leaves.print( );
    }

template <class T>
tree<T>::tree( ) {
  root = NULL;
}

template <class T>
tree<T>::tree( T rootData ) {
  root = newTreeNode<T>( );
  root->data = rootData;
}

template <class T>
void tree<T>::insert( T newData ) {
  append( &root, newData );
}


template class tree<int>;
template class tree<char>;