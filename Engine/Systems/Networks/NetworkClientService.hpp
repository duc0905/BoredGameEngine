#pragma once
#include "../../Utils/Networks/ISocket.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace Bored::Net {

constexpr int kConnectTimeOut = 2; // ms

class Client {
public:
  Client(std::string compatible_id = "");
  ~Client();

  bool Connect(int port, std::string addr, int server_port);
  void Disconnect();

  void SendToServer(std::string payload);
  std::vector<std::string> GetAllMessage();

private:
  void initSocket();
  void listenLoop(int port);
  Conn server_addr;  
  std::string compatible_id_;

  std::shared_ptr<ISocket> sock_;

  std::vector<std::string> mqueue_;
  std::mutex q_mtx_;
  std::atomic<bool> running_;
  std::thread listener_;
};

} // namespace Bored::Net
