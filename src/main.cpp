#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *arvg[]) {
  vector<string> lines;
  string line;
  while (getline(cin, line)) {
    lines.push_back(line);
  }

  cout << "Y U NO LIKE ECHO? " << endl;

  return 0;
}
