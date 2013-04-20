#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstring>

#include "Buffer.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc > 1) {
    Buffer buff(argv[1]);
    // write unedited buffer
    buff.write();
  } else {
    Buffer buff;
    buff.write();
    cout << "pass a file as a parameter" << std::endl;
  }

  return 0;
}

void startEditor() {
  char mesg[] = "welcome to JPEdit. It sucks.";
  char input[80];
  int row, col;

  // allocate needed memory screen. usually clears screen.
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
