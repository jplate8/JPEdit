// Window-manager.h
//
// Represents a set of windows and tracks user interatcoin with them.

class Window_manager {
  public:
    // default constructor:
    // creates a new Window with empty Buffer,
    // and sets it as currently selected.
    Window_manager();

    // constructor:
    // creates a new window with a Buffer for the given file path,
    // and sets it as currently selected.
    Window_manager(const std::string &path);

    // select the Window with the given ID.
    // true on success.
    bool select(const int &window_id);

    // add a window to the list
    void add_window(Window &new_window);

    // open buffer for given path and add it to the list.
    // returns the new buffer's ID, aka the index of the buffer in the vector.
    int open(const std::string &path);

    // relinquish control of a window to the window of the given ID.
    // if relinquisher is not selected, does nothing.
    // true on success.
    bool relinuish(Window *relinquisher, const int &new_window_id); 
    
  private:
    // set the window's active ncurses window.
    void set_window(Window &window, WINDOW *new_active);

    // set the window's front buffer.
    void set_buffer(Window &window, Buffer *new_front);

    // all the Windows managed by this manager
    vector<Window> windows;

    // all the Buffers that this manager's Windows can be assigned to.
    vector<Buffer> buffers;

    // currently selected window
    Window *selected;
}
