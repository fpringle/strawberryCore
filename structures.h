#ifndef __STRUCTURES_H
#define __STRUCTURES_H

template <class T>
struct node {
  T data;
  struct node <T> * next;
};

template <class T>
struct node <T> * newNode( );

template <class T>
struct binode {
  T data;
  struct binode <T> * next;
  struct binode <T> * prev;
};

template <class T>
struct binode <T> * newBiNode( );

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
class doublylinkedlist {
  struct binode <T> * head;
  struct binode <T>* tail;

  public:
    // constructors
    doublylinkedlist( );
    // adding values
    void append( T );
    void prepend( T );
    void insertRight( int, T );
    void insertLeft( int, T );
    // deleting values
    void deleteIndex( int );
    // get info
    int linear_search( T );
    int len( );
    T min( );
    T max( );
    T get_item( int );
    bool is_empty( );
    // do stuff
    void set_item( int, T );
    void swap( int, int );
    void bubbleSort( );
    void selectionSort( );
    void insertionSort( );
    // utility
    void print( );
    void headtail();
};

template <class T>
class linkedlist {
  struct node <T> * head;
  struct node <T> * get_tail( );

  public:
    // constructors
    linkedlist( );
    // adding values
    void append( T );
    void prepend( T );
    void insertRight( int, T );
    void insertLeft( int, T );
    // deleting values
    void deleteIndex( int );
    // get info
    int linear_search( T );
    int len( );
    T min( );
    T max( );
    T get_item( int );
    bool is_empty( );
    // do stuff
    void set_item( int, T );
    void swap( int, int );
    void bubbleSort( );
    void selectionSort( );
    void insertionSort( );
    // utility
    void print( );
};



template <class T>
void get_leaves( treenode <T> *, linkedlist <T> * );


template <class T>
class queue {
  doublylinkedlist <T> lst;
  
  public:
    queue( );
    void enqueue( T );
    // pop to target
    void dequeue( T* );
    // pop and return
    T dequeue( );
    // return head without popping
    T peek( );
    // check if empty
    bool is_empty( );
    //print
    void print( );
};

template <class T>
class stack {
  doublylinkedlist <T> lst;
  
  public:
    stack( );
    void enstack( T );
    // pop to target
    void destack( T* );
    // pop and return
    T destack( );
    // return head without popping
    T peek( );
    //print
    void print( );
};

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

#endif