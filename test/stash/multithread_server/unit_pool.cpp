#include "unit_pool.h"

unit& unit_pool::get_unit()
{
  if (this->current_index == this->size)
    this->current_index = 0;
  return *_units[this->current_index++];
}

void unit_pool::run()
{
  for (unsigned int i = 0; i < this->size; ++i)
  {
    moost::Thread *th = new moost::Thread([&_units, i](){
                                            _units[i]->run();
                                          });
    threads.push_back(th);
  }
}


void unit_pool::stop()
{
  std::for_each(threads.begin(), threads.end(), [](moost::Thread *tptr) {
                  delete tptr;
                });
  std::for_each(_units.begin(), _units.end(), [](unit *uptr) {\
                  delete uptr;
                });
}
