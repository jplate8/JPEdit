// Window.h
//
// forms an interaction between user interaction and a file Buffer.

#include <memory>
#include <ncurses.h>

#include "Window.h"
#include "Buffer.h"

#ifndef NDEBUG
#include <sstream>
#include "Debug.h"
#endif /* NDEBUG */

// constructor:
// uses given ncurses window.
// shows the given buffer.
Window::Window(Window_manager *manager_, int buff_id, WINDOW *active)
  : manager(manager_), buffer_id(buff_id), active_window(active)
{
  // empty
}

// do edit mode:
// interpret user input while updating buffer and screen.
void Window::edit_text()
{
  int last_key;
  bool done = false;
  Buffer &front = manager->get_buffer(buffer_id);
  std::unique_ptr<Buffer::Changeset> last_change(nullptr);
#ifndef NDEBUG
  Debug::indent();
  Debug::log("entering editing loop");
  Debug::indent();
#endif /* NDEBUG */

  // edit until user exits session
  do {
#ifndef NDEBUG
    Debug::log("starting an editing iteration");
#endif /* NDEBUG */
    last_key = wgetch(active_window);
    if (last_key != ERR) {
      last_change = do_keystroke(last_key, front);
      if (last_change != nullptr) {
        update(std::move(last_change));
      } else {
        done = true;
      }
    }
#ifndef NDEBUG
    Debug::log("ending an editing iteration");
#endif /* NDEBUG */
  } while (!done);
#ifndef NDEBUG
  Debug::outdent();
  Debug::log("exiting editing loop");
  Debug::outdent();
#endif /* NDEBUG */
}

// choose and execute appropriate buffer-editing function.
// returns nullptr on ESC.
std::unique_ptr<Buffer::Changeset>
Window::do_keystroke(const int &key, Buffer &front)
{
#ifndef NDEBUG
  Debug::indent();
  std::stringstream ss;
  ss << "performing keystroke for " << static_cast<char>(key);
  Debug::log(ss.str());
#endif /* NDEBUG */
  //TODO: change to a lookup table. Look up each key in table and if
  //something is found then call that, otherwise use default (type it).
  //then can probably make this inline.
  switch(key) {
    case KEY_UP:
#ifndef NDEBUG
  Debug::log("about to perform do_up");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_up();
      break;
    case KEY_DOWN:
#ifndef NDEBUG
  Debug::log("about to perform do_down");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_down();
      break;
    case KEY_LEFT:
#ifndef NDEBUG
  Debug::log("about to perform do_left");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_left();
      break;
    case KEY_RIGHT:
#ifndef NDEBUG
  Debug::log("about to perform do_right");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_right();
      break;
    case KEY_BACKSPACE:
#ifndef NDEBUG
  Debug::log("about to perform do_backspace");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_backspace();
      break;
    case KEY_DC:
#ifndef NDEBUG
  Debug::log("about to perform do_delete");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_delete();
      break;
    case KEY_ENTER:
#ifndef NDEBUG
  Debug::log("about to perform do_enter");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_enter();
      break;
    case KEY_HOME:
#ifndef NDEBUG
  Debug::log("about to perform do_home");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_home();
      break;
    case KEY_END:
#ifndef NDEBUG
  Debug::log("about to perform do_end");
  Debug::outdent();
#endif /* NDEBUG */
      return front.do_end();
      break;
    case KEY_ESC:
#ifndef NDEBUG
  Debug::log("received ESC: about to return nullptr");
  Debug::outdent();
#endif /* NDEBUG */
      return nullptr;
      break;
    default:
#ifndef NDEBUG
  std::stringstream ss;
  ss << "about to insert " << static_cast<char>(key);
  Debug::log(ss.str());
#endif /* NDEBUG */
      return front.insert(key);
      break;
  }
#ifndef NDEBUG
  std::string s("finished ");
  s.append(ss.str());
  Debug::log(s);
  Debug::outdent();
#endif /* NDEBUG */
}

// update active ncurses window to reflect Buffer changes.
void Window::update(std::unique_ptr<Buffer::Changeset> change)
{
  //TODO: add an options lookup table.
  //If a certain option is set, type each character in a random color.
  int y = change->cursor_orig.y;
  for (std::string line : change->changed) {
    move(y, 0);
    clrtoeol();
    wprintw(active_window, line.c_str());
    ++y;
  }
  move(change->cursor_final.y, change->cursor_final.x);
  wrefresh(active_window);
}
