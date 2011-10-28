#include "unit.h"

void unit::start(connection_mgr::connection_ptr b)
{
  char c = 0;
  moost::Error err;
  Buffer buf(&c, 1);
  this->_mutex.lock();
  this->_queue.push_back(b);
  this->_mutex.unlock();
  this->_out->send(buf, err);
}

void  unit::handle_signal()
{
  Buffer buf(_buf, 16);
  this->_mutex.lock();
  connection_mgr::connection_ptr b(this->_queue.back());
  this->_queue.pop_back();
  this->_mutex.unlock();
  _cm.start(b);
  this->_in->async_receive(buf, [this](size_t, moost::Error &err) {
                             if (!err)
                              this->handle_signal();
                           });
}

void  unit::run()
{
  moost::Error err;
  Buffer buf(_buf, 16);
  this->_in->async_receive(buf, [this](size_t, moost::Error &err) {
                             if (!err)
                               this->handle_signal();
                           });
  this->_disp.run(err, true);
  if (err)
    std::cerr <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
  std::cerr << "leave the thread" << std::endl;
}
