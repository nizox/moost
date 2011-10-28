#include <moost/stash.hpp>

using namespace moost::stash;

int test1(local::datagram::Socket &s1, local::datagram::Socket &s2, moost::Error &err)
{
  char pwet[] = "test";
  char caca[10] = { '\0' };
  Buffer b1(pwet, strlen(pwet));
  Buffer b2(caca, sizeof(caca));

  s1.send(b1, err);
  if (!err)
    s2.receive(b2, err);
  return strcmp(caca, pwet);
}

int test2(Dispatcher &disp, local::datagram::Socket &s1, local::datagram::Socket &s2, moost::Error &err)
{
  char pwet[] = "test";
  char caca[10] = { '\0' };
  Buffer b1(pwet, strlen(pwet));
  Buffer b2(caca, sizeof(caca));

  s1.async_receive(b2, [](size_t, moost::Error &) {});
  s2.async_send(b1, [](size_t, moost::Error &) {});
  disp.run(err, false);
  return strcmp(caca, pwet);
}

int main()
{

  Dispatcher disp;
  local::datagram::Socket s1(disp);
  local::datagram::Socket s2(disp);
  moost::Error err;

  local::SocketPair<local::datagram>(s1, s2, err);
  if (err)
    return 1;

  test1(s1, s2, err);
  test2(disp, s1, s2, err);
  return 0;
}
