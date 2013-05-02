// Buffer.h
//
// Represents an editing session with a file.

#include <fstream>
#include <list>

class Buffer {
  //TODO: decide how to implement set of Buffer-specific options.
  //  decided: use a lookup table.
  //  See Line.h for ideas on how to use it.
  public:
    // default constructor:
    // does not bind to a file.
    Buffer();

    // constructor:
    // binds to the given file.
    explicit Buffer(const std::string &p);

    // write the buffer to the file.
    // true on success.
    bool write();

    // set the path to which this buffer will write.
    void set_path(const std::string &p);

    // current line being edited.
    Line::iterator line;
  private:
    // current state of this Buffer's representation of its file.
    std::list<Line> lines;

    // file being edited.
    std::ofstream file;
    std::string path;
};
