#ifndef STREAMBUF_VF0TE6W2

#define STREAMBUF_VF0TE6W2

#include <string>
#include <sstream>

namespace moost {

class streambuf : private std::stringbuf
{
 public:
  streambuf() : std::stringbuf(), actual_size(0){};
  size_t sputn(char const *s, size_t n)
  {
    this->actual_size += n;
    return static_cast<size_t>(std::stringbuf::sputn(s, n));
  }

  size_t sgetn(char *s, size_t n)
  {
    this->actual_size -= n;
    return static_cast<size_t>(std::stringbuf::sgetn(s, n));
  }

  int sgetc()
  {
    return this->sgetc();
  }

  std::string *str()
  {
    return new std::string(std::stringbuf::str());
  }

  size_t size() const
  {
    return actual_size;
  }
 private:
  size_t actual_size;
};

}


#endif /* end of include guard: STREAMBUF_VF0TE6W2 */
