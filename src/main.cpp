#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>
#include <cstring>

#include "Window_manager.h"
#include "Window.h"
#include "Buffer.h"

void testFileIO(int argc, char *argv[]);
void start_editor(const std::string &path);
void old_start_editor();

int main(int argc, char *argv[])
{
  std::string first;
  if (argc > 1) {
    first = argv[1];
  }

  start_editor(first);

  return 0;
}

void testFileIO(int argc, char *argv[])
{
  if (argc > 1) {
    Buffer buff(argv[1]);
    // write unedited buffer
    buff.write();
  } else {
    Buffer buff;
    buff.write();
    std::cout << "pass a file as a parameter" << std::endl;
  }
}

void start_editor(const std::string &path)
{
  // ncurses pre-configuration:
  // allocate needed screen memory. usually clears screen.
  initscr();
  // turn off echoing
  noecho();
  // make keypresses immediately. let interrupt sequences still work.
  cbreak();
  // set timeout for ESC
  timeout(100);
  // allow arrow keys, function keys, etc.
  // TODO: when using multiple screens, do this for all of them.
  keypad(stdscr, true);

  //TODO: figure out some control loop
  Window_manager wm(path);

  // deallocate screen stuff. get back normal terminal mode.
  endwin();
}

void old_start_editor()
{
  char mesg[] = "welcome to JPEdit. It sucks.";
  char input[80];
  int row, col;

  // allocate needed screen memory. usually clears screen.
  initscr();
  getmaxyx(stdscr, row, col);

  mvprintw(row/2, (col-strlen(mesg))/2, "%s", mesg);
  getch();
  clear();
  refresh();

  move(0, 0);
  char last;
  char command[80];
  int lastx, lasty;
  while ((last = getch()) != EOF) {
    if (last == ':') {
      getyx(stdscr, lasty, lastx);
      mvprintw(row - 2, 0, ":");
      getstr(command);
      printw("executing %s", command);
      getch();
      move(row - 2, 0);
      clrtoeol();
      move(row - 1, 0);
      clrtoeol();
      move(lasty, lastx);
    }
  }

  // deallocate screen stuff. get back normal terminal mode.
  endwin();
}
