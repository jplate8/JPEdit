// Window.h
//
// forms an interaction between user interaction and a file Buffer.

#include <ncurses.h>

class Window {
  // window managers can set the active ncurses window of windows.
  friend Window_manager::set_window(Window &window, WINDOW *active);

  public:
    // constructor:
    // uses given ncurses window.
    // shows the given buffer.
    Window(WINDOW *active, Buffer *b);

  private:
    // buffer currently shown
    Buffer *front;

    // All operations will be performed using the active ncursrs window.
    // can be set by a window manager.
    WINDOW* active_window;
};
