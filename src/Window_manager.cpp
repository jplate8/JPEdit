// Window-manager.h
//
// Represents a set of windows and tracks user interatcoin with them.

#include "Window_manager.h"
#include "Window.h"
#include "Buffer.h"

// default constructor:
// creates a new Window with empty Buffer,
// and sets it as currently selected.
Window_manager::Window_manager() : Window_manager("") { }

// constructor:
// creates a new window with a Buffer for the given file path,
// and sets it as currently selected.
Window_manager::Window_manager(const std::string &path)
{
  open(path);
  selected = &windows[0];
}

// select the Window with the given ID
bool Window_manager::select(const int &window_id)
{
  if (windows.size() < window_id + 1) {
    return false;
  }
  selected = &windows[window_id];
  return true;
}

// add a window to the list
void Window_manager::add_window(Window &new_window)
{
  windows.push_back(new_window);
}

// open buffer for given path and bring it to front of selected window.
int Window_manager::open(const std::string &path)
{
  //TODO: check that no buffer exists for requested file.
  Buffer buf(path);
  buffers.push_back(buf);
  return buffers.size() - 1;
}

// relinquish control of a window to the window of the given ID.
// if relinquisher is not selected, does nothing.
// true on success.
bool Window_manager::relinuish(Window *relinquisher, const int &new_window_id)
{
  if (relinquisher != selected ||
      windows.size() < new_window_id + 1) {
    return false;
  }
  selected = &windows[new_window_id];
  return true;
}
/*
// set the window's active ncurses window.
void Window_manager::set_window(Window &window, WINDOW *new_active)
{
  window.active_window = new_active;
}
*/
