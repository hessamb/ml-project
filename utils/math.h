#ifndef __UTILS_MATH__
#define __UTILS_MATH__

template <typename T> int sign(T val) {
  return (T(0) < val) - (val < T(0));
}


#endif
