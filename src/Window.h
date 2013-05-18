#ifndef WINDOW_H
#define WINDOW_H

// Window.h
//
// forms an interaction between user interaction and a file Buffer.

#include <ncurses.h>

#include "Window_manager.h"
#include "Line.h"

class Buffer;

class Window {
  // window managers can set the active ncurses window.
  friend class Window_manager;

  public:
    // constructor:
    // uses given ncurses window.
    // shows the given buffer.
    Window(WINDOW *active, Buffer *b);

  private:
    // do edit mode:
    // interpret user input while updating buffer and screen.
    void edit_text();

    // buffer currently shown
    Buffer *front;

    // All operations will be performed using the active ncursrs window.
    // can be set by a window manager.
    WINDOW* active_window;
};

#endif /* WINDOW_H */
