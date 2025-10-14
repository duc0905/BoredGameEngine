#pragma once
#include "../../Utils/Networks/ISocket.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Bored::Net {

constexpr int kRetried = 3;

class Server {
public:
  Server(std::string compatible_id = "");
  ~Server();
  void Start(int port = 8080);
  void Stop();
  void BroadCastMessage(std::string payload);
  std::vector<Msg> GetAllMessage();

private:
  void initSocket();
  void listenLoop(int port);

  void handleNewConn(Conn conn);
  void handleFailedSendConn(Conn conn);

private:
  std::string compatible_id_;
  std::shared_ptr<ISocket> sock_;
  std::unordered_set<Conn> clients_;
  std::mutex c_mtx_;

  std::vector<Msg> mqueue_;
  std::mutex q_mtx_;
  std::atomic<bool> running_;

  std::thread listener_;
  std::unordered_map<Conn, int> retried_;
};

} // namespace Bored::Net
