// Buffer.cpp
//
// Represents an editing session with a file.

#include <iostream>
#include <fstream>
#include <list>

#include "Buffer.h"
#include "Utility.h"

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
  cursor_orig(orig),
  cursor_final(final),
  top_line(utility::min(orig.y, final.y)),
  bottom_line(utility::max(orig.y, final.y))
{
#ifndef NDEBUG
  Debug::indent();
  std::stringstream ss;
  ss << "constructing a Changeset";
  ss << " with cursors " << "(" << orig.x << "," << orig.y << ")";
  ss << " -> ";
  ss << "(" << final.x << "," << final.y << ").";
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
  Debug::log("finished constructing a Changeset");
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
// makes no changes to file text
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

  std::unique_ptr<Changeset> ret(
      new Changeset(line, 0, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_up");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// place cursor at beginning of line below.
// stops at last line.
// makes no changes to file text
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
  auto top = line;
  while(moves < num_lines && line != endln) {
    ++line;
    ++moves;
    ++cursor_pos.y;
  }
  cursor = local_first_char();
  cursor_pos.x = 0;

  std::unique_ptr<Changeset> ret(
      new Changeset(top, 0, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::log("finished performing do_down");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// move the cursor left, possibly wrapping to previous line.
// Stops at first position of first line.
// makes no changes to file text
std::unique_ptr<Buffer::Changeset>
Buffer::do_left(const int &num_moves /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  std::stringstream ss;
  ss << "performing do_left " << num_moves << " times.";
  Debug::log(ss.str());
  Debug::indent();
#endif /* NDEBUG */
  Line_list::difference_type moves = 0;
  auto orig_pos = cursor_pos;
  // very first character
  auto first = very_first_char();
  // first character of this line
  auto local_first = local_first_char();
  while (moves < num_moves && cursor != first) {
    // if should wrap left, and can
    if (cursor == local_first) {
#ifndef NDEBUG
  Debug::log("wrapping to upper line");
#endif /* NDEBUG */
      --line;
      local_first = local_first_char();
      cursor = local_end_char();  // wrap over newline, but not next char.
      --cursor_pos.y;
      cursor_pos.x = line->size() - 1;
      ++moves;
    } else { // else just move left
#ifndef NDEBUG
  Debug::log("moving left");
#endif /* NDEBUG */
      --cursor;
      --cursor_pos.x;
      ++moves;
    }
  }

  std::unique_ptr<Changeset> ret(
      new Changeset(line, 0, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::outdent();
  Debug::log("finished performing do_left");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// move the cursor right, possibly wrapping to next line.
// Stops after last position of last line.
// makes no changes to file text
std::unique_ptr<Buffer::Changeset>
Buffer::do_right(const int &num_moves /* = 1 */)
{
#ifndef NDEBUG
  Debug::indent();
  std::stringstream ss;
  ss << "performing do_right " << num_moves << " times.";
  Debug::log(ss.str());
  Debug::indent();
#endif /* NDEBUG */
  Line_list::difference_type moves = 0;
  auto orig_pos = cursor_pos;
  // after very last character
  auto last = very_end_char();
  // after last character of this line
  auto local_last = local_end_char();
  while (moves < num_moves && cursor != last) {
    // if should wrap right, and can
    if (cursor == local_last) {
#ifndef NDEBUG
  Debug::outdent();
  Debug::log("wrapping to lower line");
#endif /* NDEBUG */
      ++line;
      local_last = local_end_char();
      cursor = local_first_char();  // wrap over newline, but not next char.
      ++cursor_pos.y;
      cursor_pos.x = 0;
      ++moves;
    } else { // else just move right
#ifndef NDEBUG
  Debug::log("moving right");
#endif /* NDEBUG */
      ++cursor;
      ++cursor_pos.x;
      ++moves;
    }
  }

  std::unique_ptr<Changeset> ret(
      new Changeset(line, 0, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::outdent();
  Debug::log("finished performing do_right");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of BACKSPACE.
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
    ret->append(*do_left());
    ret->append(*do_delete());
    ++num_done;
  }
#ifndef NDEBUG
  Debug::log("finished performing do_backspace");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of DELETE.
std::unique_ptr<Buffer::Changeset>
Buffer::do_delete(const int &num_presses /* = 1 */)
{
  //TODO: update Window::update to work if lines are merged
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_delete");
#endif /* NDEBUG */
  Line_list::difference_type num_done = 0;
  int wraps = 0;
  auto endln = very_end_char();
  while (cursor != endln && num_done < num_presses) {
    if (cursor == local_end_char()) {
      // delete line break: join line with next
      auto next = line;
      ++next;
      line->insert(cursor, begin(*next), end(*next));
    } else {
      cursor = line->erase(cursor);
      // cursor ends up on element after one erased
    }
    ++num_done;
  }

  // cursor doesn't move
  std::unique_ptr<Changeset> ret(
      new Changeset(line, wraps + 1, cursor_pos, cursor_pos));
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
  Debug::indent();
#endif /* NDEBUG */
  Line_list::difference_type num_done = 0;
  auto orig_pos = cursor_pos;
  auto top = line;
  while (num_done < num_presses) {
#ifndef NDEBUG
  Debug::log("doing an enter");
#endif /* NDEBUG */
    auto curr_end = local_end_char();
    // cursor can never be past end of lines, so always safe to increment.
    ++line;
    ++cursor_pos.y;
    // insert empty line
    line = lines.emplace(line);
    // move over old elements
    auto dest = line--;
    dest->splice(begin(*dest), *line, cursor, end(*line));
    ++line;
    cursor = local_first_char();
    ++num_done;
  }
  cursor_pos.x = 0;

  std::unique_ptr<Changeset> ret(
      new Changeset(top, num_done + 1, orig_pos, cursor_pos));
#ifndef NDEBUG
  Debug::outdent();
  Debug::log("finished performing do_enter");
  Debug::outdent();
#endif /* NDEBUG */
  return ret;
}

// perform necessary actions to handle pressing of HOME.
// place cursor on first character of line.
// makes no changes to file text
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
// makes no changes to file text
std::unique_ptr<Buffer::Changeset> Buffer::do_end()
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing do_end");
#endif /* NDEBUG */
  auto orig_pos = cursor_pos;
  cursor = local_end_char();
  cursor_pos.x = line->size();

  std::unique_ptr<Changeset> ret(
      new Changeset(line, 0, orig_pos, cursor_pos));
  return ret;
#ifndef NDEBUG
  Debug::log("finished performing do_end");
  Debug::outdent();
#endif /* NDEBUG */
}

// append another Changeset to this one, so that this one includes
// information from both. Other's cursor must start where this one's ends.
// invalidates the other Changeset.
void Buffer::Changeset::append(Changeset &other)
{
#ifndef NDEBUG
  Debug::indent();
  Debug::log("performing append");
  Debug::indent();
#endif /* NDEBUG */
  if (cursor_final != other.cursor_orig) {
#ifndef NDEBUG
    Debug::outdent();
  Debug::log("finished performing append: nonadjacent input received");
    Debug::outdent();
#endif /* NDEBUG */
    return;
  }

  int lowest_top = utility::max(top_line, other.top_line);
  int highest_bottom = utility::min(bottom_line, other.bottom_line);

  // replace overlapping section with other's
  if ((lowest_top < highest_bottom) ||
      ((top_line == bottom_line) &&
       (other.top_line == other.bottom_line))) {
#ifndef NDEBUG
  Debug::log("replacing overlapping section");
#endif /* NDEBUG */
    auto overlap_top_this = begin(changed),
         overlap_bottom_this = end(changed),
         overlap_top_other = begin(other.changed),
         overlap_bottom_other = end(other.changed);
    // position top_this
    for (int i = top_line; i < lowest_top; ++i) {
      ++overlap_top_this;
    }
    // position top_other
    for (int i = other.top_line; i < lowest_top; ++i) {
      ++overlap_top_other;
    }
    // position bottom_this
    for (int i = bottom_line; i > highest_bottom; --i) {
      --overlap_bottom_this;
    }
    // position bottom_other
    for (int i = other.bottom_line; i > highest_bottom; --i) {
      --overlap_bottom_other;
    }
    
    // delete old section
    changed.erase(overlap_top_this, overlap_bottom_this);
    // insert new section
    changed.splice(overlap_top_this,
                   other.changed,
                   overlap_top_other,
                   overlap_bottom_other);
  }

  // prepend extra beginning section
  if (other.top_line < top_line) {
#ifndef NDEBUG
  Debug::log("prepending section");
#endif /* NDEBUG */
    auto prepend_bottom_other = begin(other.changed);
    // position bottom_other
    for (int i = other.top_line; i < top_line; ++i) {
      ++prepend_bottom_other;
    }

    // prepend section
    changed.splice(begin(changed),
                   other.changed,
                   begin(other.changed),
                   prepend_bottom_other);
  }

  // append extra ending section
  if (bottom_line < other.bottom_line) {
#ifndef NDEBUG
  Debug::log("appending section");
#endif /* NDEBUG */
    auto append_top_other = end(other.changed);
    // position top_other
    for (int i = other.bottom_line; i > bottom_line; --i) {
      --append_top_other;
    }

    // append section
    changed.splice(end(changed),
                   other.changed,
                   append_top_other,
                   end(other.changed));
  }

  top_line = utility::min(top_line, other.top_line);
  bottom_line = utility::max(bottom_line, other.bottom_line);
  cursor_final = other.cursor_final;

#ifndef NDEBUG
  Debug::outdent();
  Debug::log("finished performing append");
  Debug::outdent();
#endif /* NDEBUG */
}
