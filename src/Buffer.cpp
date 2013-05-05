// Buffer.cpp
//
// Represents an editing session with a file.

#include <iostream>
#include <fstream>
#include <list>

#include "Buffer.h"

// default constructor:
// does not bind to a file.
Buffer::Buffer() : Buffer("") { }

// constructor:
// binds to the given file.
Buffer::Buffer(const std::string &p) : path(p)
{
  // initializes Buffer state to be existing file state, if one exists.
  std::ifstream state_init(path);
  // read all existing lines into Buffer.
  std::string file_line;
  std::cout << "reading lines from file: " << path << "\n";
  while (getline(state_init, file_line)) {
    // must be a list<char>, not a string
    lines.emplace_back(file_line.begin(), file_line.end());
    std::cout << file_line << "\n";
  }
  std::cout << "done reading file." << std::endl;

  // done with state_init
  state_init.close();

  // place cursor at start of file.
  line = lines.begin();
  cursor = very_first_char();
}

bool Buffer::write()
{
  std::ofstream file;
  file.open(path);
  if (!file) {
    file.close();
    std::cout << "write failed" << std::endl;
    return false;
  }

  int size = lines.size();
  int lineindex = 0;
  for (Line line : lines) {
    std::string file_line(line.begin(), line.end());
    file << file_line;
    if (lineindex != size - 1) {
      file << "\n";
    }
    lineindex++;
  }
  file << std::flush;
  file.close();
  return true;
}

void Buffer::set_path(const std::string &p)
{
  path = p;
}

// insert the given character before the cursor.
void Buffer::insert(const char &character)
{
  //TODO: update this when line length limiting is implemented.
  (*line).insert(cursor, character);
}

// place cursor at beginning of line above.
// stops at first line.
// returns number of lines moved up.
int Buffer::cursormv_up(const int &num_lines /* = 1 */)
{
  int moves = 0;
  auto first = lines.begin();
  while(moves < num_lines && line != first) {
    --line;
    ++moves;
  }
  cursor = local_first_char();
  return moves;
}

// place cursor at beginning of line below.
// stops at last line.
// returns number of lines moved down.
int Buffer::cursormv_down(const int &num_lines /* = 1 */)
{
  int moves = 0;
  auto end = --(lines.end());
  while(moves < num_lines && line != end) {
    ++line;
    ++moves;
  }
  cursor = local_first_char();
  return moves;
}

// move the cursor left, possibly wrapping to previous line.
// Stops at first position of first line.
// returns number of positions moved left.
int Buffer::cursormv_left(const int &num_moves /* = 1 */)
{
  int taken = 0;
  // very first character
  auto first = very_first_char();
  // first character of this line
  auto local_first = local_first_char();
  while (taken < num_moves && cursor != first) {
    while (cursor != local_first) {
      --cursor;
      ++taken;
    }
    // if should wrap left, and can
    if (taken < num_moves && cursor != first) {
      --line;
      local_first = local_first_char();
      cursor = local_end_char();  // wrap over newline, but not next char.
      ++taken;
    }
  }
  return taken;
}

// move the cursor right, possibly wrapping to next line.
// Stops after last position of last line.
// returns number of positions moved right.
int Buffer::cursormv_right(const int &num_moves /* = 1 */)
{
  int taken = 0;
  // after very last character
  auto last = very_end_char();
  // after last character of this line
  auto local_last = local_end_char();
  while (taken < num_moves && cursor != last) {
    while (cursor != local_last) {
      ++cursor;
      ++taken;
    }
    // if should wrap right, and can
    if (taken < num_moves && cursor != last) {
      ++line;
      local_last = local_end_char();
      cursor = local_first_char();  // wrap over newline, but not next char.
    }
  }
  return taken;
}

// perform necessary actions to handle pressing of BACKSPACE.
// return numbers of backspace operations performed successfully.
int Buffer::do_backspace(const int &num_presses /* = 1 */)
{
  int num_done = 0;
  auto first = very_first_char();
  while (cursor != first && num_done < num_presses) {
    cursormv_left();
    do_delete();
  }
  return num_done;
}

// perform necessary actions to handle pressing of DELETE.
// returns number of delete operations performed successfully.
int Buffer::do_delete(const int &num_presses /* = 1 */)
{
  int num_done = 0;
  auto end = very_end_char();
  while (cursor != end && num_done < num_presses) {
    if (cursor == local_end_char()) {
      // delete line break: join line with next
      auto next = line;
      ++next;
      (*line).insert(cursor, (*next).begin(), (*next).end());
    } else {
      curr_line().erase(cursor);
      // cursor ends up on element after one erased
    }
    ++num_done;
  }
  return num_done;
}

// perform necessary actions to handle pressing of ENTER.
// insert a line break before character under cursor.
void Buffer::do_enter(const int &num_presses /* = 1 */)
{
  auto curr_end = local_end_char();
  // cursor can never be past end of line, so always safe to increment.
  ++line;
  // insert line after cursor's one that has all elements from
  // current cursor position to end of line.
  lines.emplace(line, cursor, curr_end);
  cursor = local_first_char();
}

// perform necessary actions to handle pressing of HOME.
// place cursor on first character of line.
void Buffer::do_home()
{
  cursor = local_first_char();
}

// perform necessary actions to handle pressing of END.
// place cursor after last character of line.
void Buffer::do_end()
{
  cursor = local_end_char();
}

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
