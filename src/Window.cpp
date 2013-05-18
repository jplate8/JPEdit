// Window.h
//
// forms an interaction between user interaction and a file Buffer.

#include <memory>
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
  int last_key;
  bool done = false;
  std::unique_ptr<Buffer::Changeset> last_change(nullptr);

  // edit until user exits session
  do {
    last_key = wgetch(active_window);
    last_change = do_keystroke(last_key);
    if (last_change != nullptr) {
      // update active_window according to last_change
    } else {
      done = true;
    }
  } while (!done);
}

// choose and execute appropriate buffer-editing function.
// returns nullptr on ESC.
std::unique_ptr<Buffer::Changeset> Window::do_keystroke(const int &key)
{
    //TODO: change to a lookup table. Look up each key in table and if
    //something is found then call that, otherwise use default (type it).
    //then can probably make this inline.
    switch(key) {
      case KEY_UP:
        return front->cursormv_up();
        break;
      case KEY_DOWN:
        return front->cursormv_down();
        break;
      case KEY_LEFT:
        return front->cursormv_left();
        break;
      case KEY_RIGHT:
        return front->cursormv_right();
        break;
      case KEY_BACKSPACE:
        return front->do_backspace();
        break;
      case KEY_DC:
        return front->do_delete();
        break;
      case KEY_ENTER:
        return front->do_enter();
        break;
      case KEY_HOME:
        return front->do_home();
        break;
      case KEY_END:
        return front->do_end();
        break;
      case KEY_ESC:
        return nullptr;
        break;
      default:
        return front->insert(key);
        break;
    }
}
