// Window-manager.h
//
// Represents a set of windows and tracks user interatcoin with them.

#include <iostream>

#include "Window_manager.h"
#include "Window.h"
#include "Buffer.h"

#ifndef NDEBUG
#include "Debug.h"
#endif /* NDEBUG */

// constructor:
// creates a new window with a Buffer for the given file path,
// and sets it as currently selected.
// defualts to empty path.
Window_manager::Window_manager(const std::string &path /* = "" */)
{
  open(path);
  add_window();
  //TODO: if I want mulit-modality,
  //implement some sort of control that doesn't involve editing mode.
#ifndef NDEBUG
  Debug::log("about to edit text...");
#endif /* NDEBUG */
  (*selected)->edit_text();
}

// add and select a window to the list
// with the given buffer and ncurses window.
// defaults to first buffer and standard screen.
void Window_manager::add_window(int buff_id /* = 0 */, 
                                WINDOW *nc_win /* = stdscr */)
{
  std::unique_ptr<Window> p(new Window(this, buff_id, nc_win));
  windows.push_back(std::move(p));
  selected = --end(windows);
}

// open buffer for given path and bring it to front of selected window.
int Window_manager::open(const std::string &path)
{
  //TODO: check that no buffer exists for requested file.
  std::unique_ptr<Buffer> p(new Buffer(path));
  buffers.push_back(std::move(p));
  return buffers.size() - 1;
}

// get the buffer for the given ID.
Buffer &Window_manager::get_buffer(const int &buffer_id)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("starting to get buffer");
#endif /* NDEBUG */
  //TODO: what to do if not a valid id?
  //When a buffer is deleted, will all Windows be visited to make
  //sure that they aren't referring to an old one?
#ifndef NDEBUG
  Debug::log("finished getting buffer");
  Debug::outdent();
#endif /* NDEBUG */
  return *buffers[buffer_id];
}
