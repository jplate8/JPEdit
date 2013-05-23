#ifndef POINT_H
#define POINT_H

struct Point {
  Point() : Point(0, 0) { }
  Point(int x_, int y_) : x(x_), y(y_) { }

  int x;
  int y;
};

// if they contain the same coordinates
inline bool operator==(const Point &lhs, const Point &rhs)
{
  return lhs.x == rhs.x &&
         lhs.y == rhs.y;
}

// if they contain the same coordinates
inline bool operator!=(const Point &lhs, const Point &rhs)
{
  return !(lhs == rhs);
}

// Point.y is weighted more heavily than Point.x
inline bool operator<(const Point &lhs, const Point &rhs)
{
  if (lhs.y < rhs.y) {
    return true;
  } else if (lhs.y > rhs.y) {
    return false;
  } else { // same y
    if (lhs.x < rhs.x) {
      return true;
    } else {
      return false;
    }
  }
}

// Point.y is weighted more heavily than Point.x
inline bool operator>(const Point &lhs, const Point &rhs)
{
  return (lhs != rhs) && !(lhs < rhs);
}

inline bool operator<=(const Point &lhs, const Point &rhs)
{
  return !(lhs > rhs);
}

inline bool operator>=(const Point &lhs, const Point &rhs)
{
  return !(lhs < rhs);
}

// find smaller of two points
inline Point &min(Point &lhs, Point &rhs)
{
  return (lhs < rhs) ? lhs : rhs;
}

// find larger of two points
inline Point &max(Point &lhs, Point &rhs)
{
  return (lhs > rhs) ? lhs : rhs;
}

#endif /* POINT_H */
