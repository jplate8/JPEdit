#ifndef BUFFER_H
#define BUFFER_H

// Buffer.h
//
// Represents an editing session with a file.

#include <string>
#include <fstream>
#include <list>

#include "Window.h"
#include "Line.h"

class Buffer {
  //friend std::list<Line> &Window::front_lines();
  //friend std::list<Line>::iterator &Window::front_line();
  //friend Line::iterator &Window::front_cursor();
  friend class Window;
  
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

    // insert the given character before the cursor.
    void insert(const char &character);

    // place cursor at beginning of line above.
    // stops at first line.
    // returns number of lines moved up.
    int cursormv_up(const int &num_lines = 1);

    // place cursor at beginning of line below.
    // stops at last line.
    // returns number of lines moved down.
    int cursormv_down(const int &num_lines = 1);

    // move the cursor left, possibly wrapping to previous line.
    // Stops at first position of first line.
    // returns number of positions moved left.
    int cursormv_left(const int &num_moves = 1);

    // move the cursor right, possibly wrapping to next line.
    // Stops after last position of last line.
    // returns number of positions moved right.
    int cursormv_right(const int &num_moves = 1);

    // perform necessary actions to handle pressing of BACKSPACE.
    // return numbers of backspace operations performed successfully.
    int do_backspace(const int &num_presses = 1);

    // perform necessary actions to handle pressing of DELETE.
    // returns number of delete operations performed successfully.
    int do_delete(const int &num_presses = 1);

    // perform necessary actions to handle pressing of ENTER.
    // insert a line break before character under cursor.
    void do_enter(const int &num_presses = 1);

    // perform necessary actions to handle pressing of HOME.
    // place cursor on first character of line.
    void do_home();
    
    // perform necessary actions to handle pressing of END.
    // place cursor after last character of line.
    void do_end();

  private:
    // current line object being edited, as opposed to iterator.
    Line &curr_line();

    // position of first character on current line.
    Line::iterator local_first_char();

    // position AFTER last character on current line.
    Line::iterator local_end_char();

    // position of first character on first line.
    Line::iterator very_first_char();

    // position AFTER last character on last line.
    Line::iterator very_end_char();

    // current state of this Buffer's representation of its file.
    std::list<Line> lines;

    // current line being edited.
    std::list<Line>::iterator line;

    // cursor position on current line.
    Line::iterator cursor;

    // file being edited.
    //std::ofstream file;
    std::string path;
};

#endif /* BUFFER_H */
