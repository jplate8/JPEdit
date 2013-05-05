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
    //
    //some Buffer functions return ints and some don't. Can't simply make a
    //map of ints to function pointers because types are different. Idea:
    //make local functions that handle these keypresses during edit mode,
    //and make them all take and return void. This way buffer switching
    //works, using a map works, cleaner mapping, modular to allow extension,
    //and lets me learn how to use maps. Probably should make them private.
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

// reach into the front Buffer and get their insides.
std::list<Line> &Window::front_lines()
{
  return front->lines;
}

std::list<Line>::iterator &Window::front_line()
{
  return front->line;
}

Line::iterator &Window::front_cursor()
{
  return front->cursor;
}

