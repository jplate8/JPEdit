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
Buffer::Buffer(const std::string &p) : path(p), line(lines.begin())
{
  // initializes Buffer state to be existing file state, if one exists.
  std::ifstream state_init(path);
  // read all existing lines into Buffer.
  std::string line;
  std::cout << "reading lines from file: " << path << "\n";
  while (getline(state_init, line)) {
    lines.push_back(line);
    std::cout << line << "\n";
  }
  std::cout << "done reading file." << std::endl;

  // done with state_init
  state_init.close();
}

bool Buffer::write()
{
  file.open(path);
  if (!file) {
    file.close();
    std::cout << "write failed" << std::endl;
    return false;
  }

  int size = lines.size();
  int lineindex = 0;
  for (std::string line : lines) {
    file << line;
    if (lineindex != size - 1) {
      file << "\n";
    }
    lineindex++;
  }
  file << std::flush;
  file.close();
  return true;
}

void Buffer::set_path(const std::string &p) {
  path = p;
}
