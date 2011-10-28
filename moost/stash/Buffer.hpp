#ifndef BUFFER_HPP_
#define BUFFER_HPP_

namespace moost {
namespace stash {

class Buffer
{
 public:
  inline Buffer(void *data, size_t size) : _data(data), _size(size) {}
  inline explicit Buffer(Buffer const &buf) : _data(buf._data), _size(buf._size) {}
  inline Buffer(std::string const &str) : _data(const_cast<char*>(str.c_str())), _size(str.size()) {};
  inline Buffer &operator=(Buffer const &buf)
  {
    if (this != &buf)
    {
      _data = buf._data;
      _size = buf._size;
    }
    return *this;
  }
  inline ~Buffer() {}

  template <typename T>
  inline T   data()
  {
    return reinterpret_cast<T>(this->_data);
  }

  template <typename T>
  inline T const data() const
  {
    return reinterpret_cast<T const>(this->_data);
  }

  inline size_t size() const
  {
    return this->_size;
  }

 private:
  void*            _data;
  size_t           _size;
};

} // stash
} // moost

#endif
