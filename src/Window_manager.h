#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

// Window-manager.h
//
// Represents a set of windows and tracks user interatcoin with them.

#include <string>
#include <vector>
#include <list>
#include <memory>

#include <ncurses.h>

class Window;
class Buffer;


class Window_manager {
  public:
    using window_list = std::list<std::unique_ptr<Window>>;

    // constructor:
    // creates a new window with a Buffer for the given file path,
    // and sets it as currently selected.
    // TODO: figure out variadics and open an arbitrary number of buffers.
    explicit Window_manager(const std::string &path = "");

    // currently selected window.
    // TODO: perhaps hide this and exit if no more windows.
    // or perhaps go to window-opening shell.
    window_list::iterator selected;

    // add and select a window to the list
    // with the given buffer and ncurses window.
    // defaults to first buffer and standard screen.
    void add_window(int buff_id = 0, WINDOW *nc_win = stdscr);

    // open buffer for given path and add it to the list.
    // returns the new buffer's ID, aka the index of the buffer in the vector.
    int open(const std::string &path);

    // get the buffer for the given ID.
    Buffer &get_buffer(const int &buffer_id);

  private:
    // all the Windows managed by this manager
    window_list windows;

    // all the Buffers that this manager's Windows can be assigned to.
    std::vector<std::unique_ptr<Buffer>> buffers;
};

#endif /* WINDOW_MANAGER_H */
