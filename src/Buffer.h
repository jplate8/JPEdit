#ifndef BUFFER_H
#define BUFFER_H

// Buffer.h
//
// Represents an editing session with a file.

#include <string>
#include <fstream>
#include <list>

#include "Window.h"
//#include "Line.h"

class Buffer {
  //friend std::list<Line> &Window::front_lines();
  //friend std::list<Line>::iterator &Window::front_line();
  //friend Line::iterator &Window::front_cursor();
  friend class Window;
  
  //TODO: decide how to implement set of Buffer-specific options.
  //  decided: use a lookup table.
  //  See Line.h for ideas on how to use it.
  public:
    using Line = std::list<char>;
    using Line_list = std::list<Line>;
  
    // default constructor:
    // does not bind to a file.
    Buffer();

    // constructor:
    // binds to the given file.
    explicit Buffer(const std::string &p);

    // set of changes made by Buffer edit commands.
    class Changelog;

    // write the buffer to the file.
    // true on success.
    bool write();

    // set the path to which this buffer will write.
    void set_path(const std::string &p);

    // insert the given character before the cursor.
    Changelog insert(const char &character);

    // place cursor at beginning of line above.
    // stops at first line.
    // returns number of lines moved up.
    Changelog cursormv_up(const int &num_lines = 1);

    // place cursor at beginning of line below.
    // stops at last line.
    // returns number of lines moved down.
    Changelog cursormv_down(const int &num_lines = 1);

    // move the cursor left, possibly wrapping to previous line.
    // Stops at first position of first line.
    // returns number of positions moved left.
    Changelog cursormv_left(const int &num_moves = 1);

    // move the cursor right, possibly wrapping to next line.
    // Stops after last position of last line.
    // returns number of positions moved right.
    Changelog cursormv_right(const int &num_moves = 1);

    // perform necessary actions to handle pressing of BACKSPACE.
    // return numbers of backspace operations performed successfully.
    Changelog do_backspace(const int &num_presses = 1);

    // perform necessary actions to handle pressing of DELETE.
    // returns number of delete operations performed successfully.
    Changelog do_delete(const int &num_presses = 1);

    // perform necessary actions to handle pressing of ENTER.
    // insert a line break before character under cursor.
    Changelog do_enter(const int &num_presses = 1);

    // perform necessary actions to handle pressing of HOME.
    // place cursor on first character of line.
    Changelog do_home();
    
    // perform necessary actions to handle pressing of END.
    // place cursor after last character of line.
    Changelog do_end();

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
    Line_list lines;

    // current line being edited.
    Line_list::iterator line;

    // cursor position on current line.
    Line::iterator cursor;

    // file being edited.
    //std::ofstream file;
    std::string path;
};

class Buffer::Changelog {
  public:
    // constructor.
    //
    // takes line that cursor ended on,
    // number of lines cursor has moved.
    Changelog(Line_list::iterator end,
                      Line_list::difference_type diff)
      : ending_line(end), line_diff(diff) { }

    // line that cursor ended on.
    Line_list::iterator ending_line;

    // number of lines cursor moved.
    Line_list::difference_type line_diff;
};

// current line object being edited, as opposed to iterator.
inline Line &Buffer::curr_line()
{
  return *line;
}

// position of first character on first line.
inline Line::iterator Buffer::very_first_char()
{
  return (*lines.begin()).begin();
}

// position AFTER last character on last line.
inline Line::iterator Buffer::very_end_char()
{
  // last line iterator
  auto last_line = lines.begin() == lines.end() ?
              lines.end() :
              --(lines.end());
  // after very last character
  return (*last_line).end();
}

// position of first character on current line.
inline Line::iterator Buffer::local_first_char()
{
  return curr_line().begin();
}

// position AFTER last character on current line.
inline Line::iterator Buffer::local_end_char()
{
  return curr_line().end();
}

#endif /* BUFFER_H */
