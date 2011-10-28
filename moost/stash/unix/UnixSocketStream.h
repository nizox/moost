#ifndef UNIXSOCKETSTREAM_H
#define UNIXSOCKETSTREAM_H

#include "moost/stash/Buffer.hpp"
#include "moost/stash/unix/UnixSocket.hpp"

namespace moost {
namespace stash {

template<typename Protocol, typename Dispatcher>
class UnixSocketStream: public UnixSocket<Protocol, Dispatcher>
{
 public:
  UnixSocketStream(Dispatcher &);
  UnixSocketStream(Dispatcher &, UnixProtocol<Protocol> const &);
  UnixSocketStream(Dispatcher &, UnixEndpoint<Protocol> const &);
  ~UnixSocketStream();

  template <typename ReceiveHandlerUntil>
  void async_receive_until(moost::streambuf &, std::string const &, ReceiveHandlerUntil);
  size_t receive_until(moost::streambuf &, std::string const &, Error &err);
};

template <typename Protocol, typename Dispatcher, typename ReceiveHandlerUntil>
class ReceiveUntilOperation : public IOperation
{
 public:
  inline ReceiveUntilOperation(UnixSocketStream<Protocol, Dispatcher> &socket,
                        moost::streambuf &sb,
                        std::string const &stop,
                        ReceiveHandlerUntil cb)
      : _socket(socket), _sb(sb), _stop(stop), _cb(cb) {};
  inline virtual ~ReceiveUntilOperation(){};

  inline virtual int operator()()
  {
    Error err;
    size_t bytes;
    char *find;
    char buff[512];
    Buffer b(buff, 512);

    bytes = _socket.receive(b, err);
    if (err)
    {
      if (err == EAGAIN)
        return (0);
      _cb(0, err);
    }
    else
    {
      _sb.sputn(b.data<char*>(), bytes);
      if ((find = std::search(buff, buff + bytes, _stop.begin(), _stop.end())) == (buff + bytes))
      {
        _socket.async_receive_until(_sb, _stop, _cb);
      }
      else
      {
        bytes = _sb.size() - (bytes - (find - buff)) + _stop.size();
        _cb(bytes, err);
      }
    }
    return (1);
  }

 private:
  UnixSocketStream<Protocol, Dispatcher> &_socket;
  moost::streambuf &_sb;
  std::string const _stop;
  ReceiveHandlerUntil _cb;
};

} // stash
} // moost



#endif /* !UNIXSOCKETSTREAM_H */
