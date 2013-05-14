#ifndef POINT_H
#define POINT_H

struct Point {
  Point() : Point(0, 0) { }
  Point(int x_, int y_) : x(x_), y(y_) { }

  int x;
  int y;
};

#endif /* POINT_H */
