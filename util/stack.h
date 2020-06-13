/* 
 * File:   stack.h
 * Author: pi
 *
 * Created on June 13, 2020, 6:11 PM
 */

#ifndef STACK_H
#define STACK_H


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

#endif /* STACK_H */

