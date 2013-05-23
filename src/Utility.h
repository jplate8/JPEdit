#ifndef UTILITY_H
#define UTILITY_H

// Utility.h
//
// Contains miscellaneous utilities.

namespace utility {

inline const int &min(const int &x, const int &y)
{
  return (x < y) ? x : y;
}

inline const int &max(const int &x, const int &y)
{
  return (x > y) ? x : y;
}

}

#endif /* UTILITY_H */
