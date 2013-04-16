// Window.h
//
// forms an interaction between user interaction and a file Buffer.

class Window {
  public:
    // default constructor:
    // creates empty buffer
    Window() = default;

    // constructor:
    // shows the given buffer
    Window(Buffer &b) front(b) { }

  private:
    // buffer currently shown
    Buffer *front;
};
