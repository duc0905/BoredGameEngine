#pragma once
#include "../../Utils/Networks/ISocket.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace Bored::Net {

struct ClientConn {
  std::string address;
  int port;
};

struct Msg {
  std::string from;
  int port;
  std::string payload;
};

class Server {
public:
  Server();
  ~Server();
  void Start(int port = 8080);
  void Stop();
  void BroadCastMessage();
  std::vector<Msg> GetAllMessage();

private:
  void initSocket();
  void listenLoop(int port);

private:
  std::shared_ptr<ISocket> sock_;
  std::vector<ClientConn> clients_;
  std::mutex c_mtx_;

  std::vector<Msg> mqueue_;
  std::mutex q_mtx_;
  std::atomic<bool> running_;

  std::thread listener_;
};

} // namespace Bored::Net
