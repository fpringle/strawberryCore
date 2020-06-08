// define some custom exceptions


#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

#include <exception>
using namespace std;

class indexException: public exception {
  virtual const char * what() const throw() {
    return "Index out of range";
  }
} indexex;

#endif
