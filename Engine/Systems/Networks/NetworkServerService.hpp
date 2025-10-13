#pragma once
#include "../../Utils/Networks/ISocket.hpp"
#include <memory>
#include <vector>

#ifdef _WIN32
#include "../../Utils/Networks/WindowsSocket.hpp";
#endif

namespace Bored::Net {

struct client_conn {
  std::string address;
  int port;
};

class Server {
public:
  Server();
  ~Server();
  void Start();
  void BroadCastMessage();
  std::vector<std::string> GetAllMessage();
  void Stop();

private:
  void initSocket();

private:
  std::shared_ptr<ISocket> sock_;
  std::vector<client_conn> clients_;
};

} // namespace Bored::Net
