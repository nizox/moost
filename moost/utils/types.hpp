#ifndef UTILSTYPES
#define UTILSTYPES

#define STRONG_TYPEDEF(S, D) \
  struct D \
  { \
    explicit D(const S t_) : t(t_) {}; \
    D() {}; \
    D(const D & t_) : t(t_.t) {} \
    D & operator=(const D & rhs) { t = rhs.t; return *this;} \
    D & operator=(const S & rhs) { t = rhs; return *this;} \
    operator const S & () const {return t; } \
    operator S & () { return t; } \
    S operator==(const D & rhs) const { return t == rhs.t; } \
    S operator<(const D & rhs) const { return t < rhs.t; } \
    S t; \
  }

#endif // !UTILSTYPES
