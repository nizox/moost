#ifndef STRING_OGLMX4L8

#define STRING_OGLMX4L8

#include <string>
#include <algorithm>
#include <functional>
#include <cctype>

namespace moost {

class string : public std::string
{
 public:
  string() : std::string() {};
  string(const std::string str) : std::string(str) {};
  string (const string& str) : std::string(str) {};
  string (const string& str, size_t pos, size_t n = std::string::npos) : std::string(str, pos, n) {};
  string (const char * s, size_t n) : std::string(s, n) {};
  string (const char * s) : std::string(s) {};
  string (size_t n, char c) : std::string(n, c) {};
  template <typename It> string(It beg, It end) : std::string(beg, end){};

  void  trim()
  {
    string::size_type pos = this->find_last_not_of(' ');
    if (pos != string::npos) {
      this->erase(pos + 1);
      pos = this->find_first_not_of(' ');
      if(pos != string::npos)
        this->erase(0, pos);
    }
    else this->erase(this->begin(), this->end()); 
  }

  void clean()
  {
    std::string tmp("\r\n");
    std::string::iterator it;
    if ((it = std::search(this->begin(), this->end(), tmp.begin(), tmp.end())) != this->end())
    {
      this->erase(it, it + tmp.size());
    }
  }
};

}
#endif /* end of include guard: STRING_OGLMX4L8 */
