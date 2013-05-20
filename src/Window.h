#ifndef WINDOW_H
#define WINDOW_H

// Window.h
//
// forms an interaction between user interaction and a file Buffer.

#include <ncurses.h>

#include "Window_manager.h"
#include "Buffer.h"
#include "Line.h"

#define KEY_ESC 27

class Window_manager;

class Window {
  // window managers can set the active ncurses window.
  friend class Window_manager;

  public:
    // constructor:
    // uses given ncurses window.
    // shows the given buffer.
    Window(Window_manager *manager_, int buff_id, WINDOW *active);

    // do edit mode:
    // interpret user input while updating buffer and screen.
    void edit_text();

  private:
    // this window's manager
    Window_manager *manager;

    // id of buffer currently shown
    int buffer_id;

    // All operations will be performed using the active ncurses window.
    // can be set by a window manager.
    WINDOW* active_window;

    // choose and execute appropriate buffer-editing function.
    // returns nullptr on ESC.
    std::unique_ptr<Buffer::Changeset> do_keystroke(const int &key);

    // update active ncurses window to reflect Buffer changes.
    void update(std::unique_ptr<Buffer::Changeset> change);
};

#endif /* WINDOW_H */
