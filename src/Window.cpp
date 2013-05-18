// Window.h
//
// forms an interaction between user interaction and a file Buffer.

#include <ncurses.h>

#include "Window.h"
#include "Buffer.h"

#define KEY_ESC 27

// constructor:
// uses given ncurses window.
// shows the given buffer.
Window::Window(WINDOW *active, Buffer *b) : front(b), active_window(active) { }

// do edit mode:
// interpret user input while updating buffer and screen.
void Window::edit_text()
{
  int last_char;
  bool done = false;

  // edit until user exits session
  do {
    last_char = wgetch(active_window);
    //TODO: change to a lookup table. Look up each key in table and if
    //something is found then call that, otherwise use default (type it).
    //then can probably make this inline.
    switch(last_char) {
      case KEY_UP:
        front->cursormv_up();
        break;
      case KEY_DOWN:
        front->cursormv_down();
        break;
      case KEY_LEFT:
        front->cursormv_left();
        break;
      case KEY_RIGHT:
        front->cursormv_right();
        break;
      case KEY_BACKSPACE:
        front->do_backspace();
        break;
      case KEY_DC:
        front->do_delete();
        break;
      case KEY_ENTER:
        front->do_enter();
        break;
      case KEY_HOME:
        front->do_home();
        break;
      case KEY_END:
        front->do_end();
        break;
      case KEY_ESC:
        done = true;
        break;
      default:
        front->insert(last_char);
        break;
    }
  } while (!done);
}
