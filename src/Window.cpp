// Window.h
//
// forms an interaction between user interaction and a file Buffer.

#include <ncurses.h>

#include "Window.h"

// constructor:
// uses given ncurses window.
// shows the given buffer.
Window(WINDOW *active, Buffer *b) front(b), active_window(active) { }
