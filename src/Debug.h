#ifndef DEBUG_H
#define DEBUG_H

// Debug.h
//
// Contains debugging tools

#include <string>
#include <iostream>

namespace Debug
{
  void log(const std::string &msg)
  {
    std::cerr << msg << std::endl;
  }
}

#endif /* DEBUG_H */
