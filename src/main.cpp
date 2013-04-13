#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {
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
  return 0;

  /*
  vector<string> lines;
  string line;
  while (getline(cin, line)) {
    lines.push_back(line);
  }

  cout << "Y U NO LIKE ECHO? " << endl;

  return 0;
  */
}
