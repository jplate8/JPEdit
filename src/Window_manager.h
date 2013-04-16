// Window-manager.h
//
// Represents a set of windows and tracks user interatcoin with them.

class Window_manager {
  public:
    // default constructor:
    // default initializes Window storage object
    //  to contain default initialized Window
    //  which contains empty buffer
    Window_manager() = default;

    // constructor:
    // initializes Window storage object
    //  to contain a Window object
    //  bound to the given file
    // TODO: find out how to file pionters or something,
    //       and how to represent Window storage

    // select the Window with the given ID
    void select(int window_id);
  private:
    // TODO: figure out how to store Windows

    // currently selected window
    Window *selected;
}
