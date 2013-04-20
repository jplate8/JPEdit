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
  // read all existing lines into filestate
  std::string line;
  std::cout << "reading lines from file: " << path << "\n";
  while (getline(state_init, line)) {
    filestate.push_back(line);
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

  int size = filestate.size();
  int lineindex = 0;
  for (std::string line : filestate) {
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

