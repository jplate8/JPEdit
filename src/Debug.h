#ifndef DEBUG_H
#define DEBUG_H

// Debug.h
//
// Contains debugging tools

#include <string>
#include <sstream>
#include <iostream>

class Debug {
  public:
    //TODO: perhaps add some auto-indenting logic,
    //based on a user-supplied indent level.
    //Each level would prepend a tab to the message.
    static void log(const std::string &msg)
    {
      if (indent_level > 0) {
        std::stringstream tabs;
        for (int i = 0; i < indent_level; ++i) {
          // 4 spaces
          tabs << "    ";
        }
        std::cerr << tabs.str();
      }
      std::cerr << msg << std::endl;
    }

    //TODO: overload ++ and --
    static void indent()
    {
      ++indent_level;
    }

    static void outdent()
    {
      if (indent_level > 0) {
        --indent_level;
      }
    }

  private:
    static int indent_level;
};

#endif /* DEBUG_H */
