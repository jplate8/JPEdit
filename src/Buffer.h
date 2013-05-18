#ifndef BUFFER_H
#define BUFFER_H

// Buffer.h
//
// Represents an editing session with a file.

#include <string>
#include <fstream>
#include <memory>
#include <list>

#include "Point.h"
#include "Line.h"

class Window;

class Buffer {
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
    struct Changeset;

    // write the buffer to the file.
    // true on success.
    bool write();

    // set the path to which this buffer will write.
    void set_path(const std::string &p);

    // insert the given character before the cursor.
    std::unique_ptr<Changeset> insert(const int &character);

    // place cursor at beginning of line above.
    // stops at first line.
    // returns number of lines moved up.
    std::unique_ptr<Changeset> cursormv_up(const int &num_lines = 1);

    // place cursor at beginning of line below.
    // stops at last line.
    // returns number of lines moved down.
    std::unique_ptr<Changeset> cursormv_down(const int &num_lines = 1);

    // move the cursor left, possibly wrapping to previous line.
    // Stops at first position of first line.
    // returns number of positions moved left.
    std::unique_ptr<Changeset> cursormv_left(const int &num_moves = 1);

    // move the cursor right, possibly wrapping to next line.
    // Stops after last position of last line.
    // returns number of positions moved right.
    std::unique_ptr<Changeset> cursormv_right(const int &num_moves = 1);

    // perform necessary actions to handle pressing of BACKSPACE.
    // return numbers of backspace operations performed successfully.
    std::unique_ptr<Changeset> do_backspace(const int &num_presses = 1);

    // perform necessary actions to handle pressing of DELETE.
    // returns number of delete operations performed successfully.
    std::unique_ptr<Changeset> do_delete(const int &num_presses = 1);

    // perform necessary actions to handle pressing of ENTER.
    // insert a line break before character under cursor.
    std::unique_ptr<Changeset> do_enter(const int &num_presses = 1);

    // perform necessary actions to handle pressing of HOME.
    // place cursor on first character of line.
    std::unique_ptr<Changeset> do_home();
    
    // perform necessary actions to handle pressing of END.
    // place cursor after last character of line.
    std::unique_ptr<Changeset> do_end();

  private:
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
    Point cursor_pos;

    // file being edited.
    std::string path;
};

struct Buffer::Changeset {
  // constructor:
  // takes topmost line that was changed,
  // number of lines that were changed, and
  // starting and final positions of the cursor.
  Changeset(Line_list::iterator topln,
      Line_list::difference_type lines_edited,
      Point orig,
      Point final);

  // include another Changeset's information in this one.
  void combine(const Changeset &other);

  // changed lines.
  std::list<std::string> changed;

  // starting (top) and final (bottom) positions of the cursor.
  Point cursor_orig;
  Point cursor_final;

  private:
    // used for combining.
    Line_list::iterator top;
};

// position of first character on first line.
inline Line::iterator Buffer::very_first_char()
{
  return begin(*begin(lines));
}

// inline function definitions

// position AFTER last character on last line.
inline Line::iterator Buffer::very_end_char()
{
  // last line iterator
  auto last_line = begin(lines) == end(lines) ?
                   end(lines) :
                   --end(lines);
  // after very last character
  return end(*last_line);
}

// position of first character on current line.
inline Line::iterator Buffer::local_first_char()
{
  return begin(*line);
}

// position AFTER last character on current line.
inline Line::iterator Buffer::local_end_char()
{
  return end(*line);
}

#endif /* BUFFER_H */
