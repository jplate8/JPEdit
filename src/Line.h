// Line.h
//
// Represents a line of text.
// Many member functions are defined in declaration because many are short.

#include <iostream>
#include <list>

class Line {
  // private: internal use only
  using word = std::string;
  using word_list = std::list<word>;
  using pos = word_list::iterator;
  // about cursor and pos iterator:
  //  ideally: would extend the word_list::iterator and make a public
  //  Line::iterator class that, in addition to all the rest of its duties,
  //  updates length. Would use insertion iterators.
  public:
    // default constructor:
    // no special initialization.
    Line() = default;

    // delete all words in the line.
    // can chain with other commands.
    Line &clear_all()
    {
      words.clear();
      length = 0;
      return this;
    }

    // insert word before word currently selected.
    // can chain with other commands.
    // TODO: find out whether this can/should be const.
    Line &insert(std::string &new_word)
    {
      words.insert(cursor, new_word);
      ++length;
      return this;
    }

    // erase the word under the cursor, if one exists.
    // if cursor not over word, does nothing.
    // can chain with other commands.
    Line &erase()
    {
      if (is_at_word()) {
        words.erase(cursor);
        --length;
      }
      return this;
    }

    // whether the cursor is currently at a word.
    bool is_at_word()
    {
      return cursor_pos != words.end();
    }

    // move cursor to first word.
    // can chain with other commands.
    Line &goto_first()
    {
      cursor = words.begin();
      return this;
    }

    // move cursor to one past last word.
    // can chain with other commands.
    Line &goto_end()
    {
      cursor = words.end();
      return this;
    }

    // whether the cursor is currently at a word.
    bool is_at_word()
    {
      return cursor != words.end();
    }
    
    int get_length()
    {
      return length;
    }

  private:
    // words in this line.
    // a word is any whitespace-separated string (like a WORD in vim).
    word_list words;

    // which word the cursor is currently at
    pos cursor;

    // length of this line, in characters.
    // TODO: decide what to do about TABs.
    //  decided: add tabwidth to set of Buffer-specific options.
    //  possibly take this option set in constructor, or
    //  possibly reference it and look up settings dynamically 
    int length;
};
