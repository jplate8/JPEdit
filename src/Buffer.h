// Buffer.h
//
// Represents an editing session with a file.

#include <fstream>
#include <list>

class Buffer {
    // TODO: add friend method for updating buffer contents

  public:
    // default constructor:
    // does not bound to a file.
    Buffer();

    // constructor:
    // binds to the given file.
    explicit Buffer(const std::string &p);

    // write the buffer to the file.
    // true on success.
    bool write();
  private:
    // current state of this Buffer's representation of its file.
    std::list<std::string> filestate;

    // file being edited
    std::ofstream file;
    std::string path;
};
