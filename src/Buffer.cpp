// Buffer.cpp
//
// Represents an editing session with a file.

#include <iostream>
#include <fstream>
#include <list>

#include "Buffer.h"

#ifndef NDEBUG
#include "Debug.h"
#endif /* NDEBUG */

// default constructor:
// does not bind to a file.
Buffer::Buffer() : Buffer("")
{
  // empty
}

// constructor:
// binds to the given file.
Buffer::Buffer(const std::string &p) : path(p)
{
  // initializes Buffer state to be existing file state, if one exists.
  std::ifstream state_init(path);
  // read all existing lines into Buffer.
  std::string file_line;
#ifndef NDEBUG
  std::stringstream ss;
  ss << "reading lines from file: " << path;
  Debug::log(ss.str());
#endif /* NDEBUG */
  while (getline(state_init, file_line)) {
    // must be a list<char>, not a string
    lines.emplace_back(file_line.begin(), file_line.end());
  }
  
  // fails if no line in lines (empty file).
  // works with empty one because no newline written on last line.
  if (lines.empty()) {
    Line empty_line;
    lines.push_back(empty_line);
  }
#ifndef NDEBUG
  std::string s("finished ");
  s.append(ss.str());
  Debug::log(s);
#endif /* NDEBUG */

  // done with state_init
  state_init.close();

  // place cursor at start of file.
  line = lines.begin();
  cursor = very_first_char();
}

// constructor:
// takes topmost line that was changed,
// number of lines that were changed, and
// starting and final positions of the cursor.
Buffer::Changeset::Changeset(Line_list::iterator topln,
                             Line_list::difference_type lines_edited,
                             Point orig,
                             Point final) :
  cursor_orig(orig), cursor_final(final), top(topln)
{
#ifndef NDEBUG
  Debug::indent();
  std::stringstream ss;
  ss << "constructing a Changeset";
  ss << " from top line of length " << topln->size();
  Debug::log(ss.str());
  Debug::indent();
#endif /* NDEBUG */
  while (lines_edited > 0) {
    changed.emplace_back(begin(*topln), end(*topln));
    ++topln;
    --lines_edited;
#ifndef NDEBUG
  Debug::log("completed a Changeset construction loop iteration");
#endif /* NDEBUG */
  }
#ifndef NDEBUG
  Debug::outdent();
  Debug::log("entering editing loop...");
  Debug::outdent();
#endif /* NDEBUG */
}

bool Buffer::write()
{
  std::ofstream file(path);
  if (!file) {
    file.close();
    std::cout << "write failed" << std::endl;
    return false;
  }

  int size = lines.size();
  int lineindex = 0;
  for (Line line : lines) {
    for (char letter : line) {
      file << letter;
    }
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
#ifndef NDEBUG
  Debug::indent();
  std::stringstream ss;
  ss << "starting to set path";
  ss << " to: " << p;
  Debug::log(ss.str());
#endif /* NDEBUG */
  path = p;
#ifndef NDEBUG
  Debug::log("finished setting path");
  Debug::outdent();
#endif /* NDEBUG */
}

// insert the given character before the cursor.
std::unique_ptr<Buffer::Changeset>
Buffer::insert(const int &character)
{
#ifndef NDEBUG
  Debug::indent();
  std::stringstream ss;
  ss << "performing insert for " << static_cast<char>(character);
  Debug::log(ss.str());
#endif /* NDEBUG */
  //TODO: update this when line length limiting is implemented.
  line->insert(cursor, character);
  auto orig_pos = cursor_pos;
  ++cursor_pos.x;
  
  std::unique_ptr<Changeset> ret(
      new Changeset(line, 1, orig_pos, cursor_pos));
#ifndef NDEBUG
  std::string s("finished ");
  s.append(ss.str());
  Debug::log(s);
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// place cursor at beginning of line above.
// stops at first line.
// returns number of lines moved up.
std::unique_ptr<Buffer::Changeset>
Buffer::do_up(const int &num_lines /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_up");
#endif /* NDEBUG */
  Line_list::difference_type moves = 0;
  auto orig_pos = cursor_pos;
  auto first = begin(lines);
  while(moves < num_lines && line != first) {
    --line;
    --cursor_pos.y;
    ++moves;
  }
  cursor = local_first_char();
  cursor_pos.x = 0;

  // cursor positions reversed because things change upwards.
  std::unique_ptr<Changeset> ret(
      new Changeset(line, moves, cursor_pos, orig_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_up");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// place cursor at beginning of line below.
// stops at last line.
// returns number of lines moved down.
std::unique_ptr<Buffer::Changeset>
Buffer::do_down(const int &num_lines /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_down");
#endif /* NDEBUG */
  Line_list::difference_type moves = 0;
  auto orig_pos = cursor_pos;
  auto endln = --end(lines);
  while(moves < num_lines && line != endln) {
    ++line;
    ++moves;
    ++cursor_pos.y;
  }
  cursor = local_first_char();
  cursor_pos.x = 0;

  std::unique_ptr<Changeset> ret(
      new Changeset(line, moves, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_down");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// move the cursor left, possibly wrapping to previous line.
// Stops at first position of first line.
// returns number of positions moved left.
std::unique_ptr<Buffer::Changeset>
Buffer::do_left(const int &num_moves /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_left");
#endif /* NDEBUG */
  Line_list::difference_type taken = 0;
  int wraps = 0;
  auto orig_pos = cursor_pos;
  // very first character
  auto first = very_first_char();
  // first character of this line
  auto local_first = local_first_char();
  while (taken < num_moves && cursor != first) {
    while (cursor != local_first) {
      --cursor;
      --cursor_pos.x;
      ++taken;
    }
    // if should wrap left, and can
    if (taken < num_moves && cursor != first) {
      --line;
      local_first = local_first_char();
      cursor = local_end_char();  // wrap over newline, but not next char.
      --cursor_pos.y;
      cursor_pos.x = line->size() - 1;
      ++taken;
      ++wraps;
    }
  }
  // cursor positions reversed because things change upwards.

  std::unique_ptr<Changeset> ret(
      new Changeset(line, wraps + 1, cursor_pos, orig_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_left");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// move the cursor right, possibly wrapping to next line.
// Stops after last position of last line.
// returns number of positions moved right.
std::unique_ptr<Buffer::Changeset>
Buffer::do_right(const int &num_moves /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_right");
#endif /* NDEBUG */
  Line_list::difference_type taken = 0;
  int wraps = 0;
  auto orig_pos = cursor_pos;
  // after very last character
  auto last = very_end_char();
  // after last character of this line
  auto local_last = local_end_char();
  while (taken < num_moves && cursor != last) {
    while (cursor != local_last) {
      ++cursor;
      ++cursor_pos.x;
      ++taken;
    }
    // if should wrap right, and can
    if (taken < num_moves && cursor != last) {
      ++line;
      local_last = local_end_char();
      cursor = local_first_char();  // wrap over newline, but not next char.
      ++cursor_pos.y;
      cursor_pos.x = 0;
      ++wraps;
    }
  }

  std::unique_ptr<Changeset> ret(
      new Changeset(line, wraps + 1, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_right");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of BACKSPACE.
// return numbers of backspace operations performed successfully.
std::unique_ptr<Buffer::Changeset>
Buffer::do_backspace(const int &num_presses /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_backspace");
#endif /* NDEBUG */
  Line_list::difference_type num_done = 0;
  std::unique_ptr<Changeset> ret(
    new Changeset(line, 0, cursor_pos, cursor_pos));
  auto first = very_first_char();
  while (cursor != first && num_done < num_presses) {
    ret->combine(*do_left());
    ret->combine(*do_delete());
  }
#ifndef NDEBUG
  Debug::log("finished performing do_backspace");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of DELETE.
// returns number of delete operations performed successfully.
std::unique_ptr<Buffer::Changeset>
Buffer::do_delete(const int &num_presses /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_delete");
#endif /* NDEBUG */
  Line_list::difference_type num_done = 0;
  int wraps = 0;
  auto orig_pos = cursor_pos;
  auto endln = very_end_char();
  while (cursor != endln && num_done < num_presses) {
    if (cursor == local_end_char()) {
      // delete line break: join line with next
      auto next = line;
      ++next;
      line->insert(cursor, begin(*next), end(*next));
    } else {
      line->erase(cursor);
      // cursor ends up on element after one erased
    }
    ++num_done;
  }

  std::unique_ptr<Changeset> ret(
      new Changeset(line, wraps, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_delete");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of ENTER.
// insert a line break before character under cursor.
std::unique_ptr<Buffer::Changeset>
Buffer::do_enter(const int &num_presses /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_enter");
#endif /* NDEBUG */
  Line_list::difference_type num_done = 0;
  auto orig_pos = cursor_pos;
  while (num_done < num_presses) {
    auto curr_end = local_end_char();
    // cursor can never be past end of line, so always safe to increment.
    ++line;
    ++cursor_pos.y;
    // insert line after cursor's one that has all elements from
    // current cursor position to end of line.
    lines.emplace(line, cursor, curr_end);
    cursor = local_first_char();
    ++num_done;
  }
  cursor_pos.x = 0;

  std::unique_ptr<Changeset> ret(
      new Changeset(line, num_done, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_enter");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of HOME.
// place cursor on first character of line.
std::unique_ptr<Buffer::Changeset> Buffer::do_home()
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_home");
#endif /* NDEBUG */
  auto orig_pos = cursor_pos;
  cursor = local_first_char();
  cursor_pos.x = 0;

  std::unique_ptr<Changeset> ret(
      new Changeset(line, 0, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_home");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of END.
// place cursor after last character of line.
std::unique_ptr<Buffer::Changeset> Buffer::do_end()
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_end");
#endif /* NDEBUG */
  auto orig_pos = cursor_pos;
  cursor = local_end_char();
  cursor_pos.x = line->size() - 1;

  std::unique_ptr<Changeset> ret(
      new Changeset(line, 0, orig_pos, cursor_pos));
  return ret;
#ifndef NDEBUG
  Debug::log("finished performing do_end");
  Debug::outdent();
#endif /* NDEBUG */
}

// include another Changeset's information in this one.
void Buffer::Changeset::combine(const Changeset &other)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing combine");
#endif /* NDEBUG */
  // if stuff to add on top
  if (cursor_orig.y > other.cursor_final.y) {
    auto ln = other.top;
    cursor_orig = other.cursor_orig;
    while (ln != top) {
      changed.emplace_front(begin(*ln), end(*ln));
    }
  }

  // if stuff to add on bottom
  if (cursor_final.y < other.cursor_final.y) {
    auto endln = top;
    for (int i = cursor_orig.y; i <= cursor_final.y; ++i) {
      ++endln;
    }

    while (endln != other.top) {
      changed.emplace_back(begin(*endln), end(*endln));
    }
  }
#ifndef NDEBUG
  Debug::log("finished performing combine");
  Debug::outdent();
#endif /* NDEBUG */
}
