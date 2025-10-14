#include "NetworkServerService.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#ifdef _WIN32
#include "../../Utils/Networks/WindowsSocket.hpp"
#endif

namespace Bored::Net {

Server::Server(std::string compatible_id) {
  initSocket();
  compatible_id_ = compatible_id;
};

Server::~Server() { Stop(); };

void Server::Start(int port) {
  if (running_.load()) {
    return;
  }

  running_ = true;
  try {
    sock_->Open(IPv4, Datagram, UDP);
    sock_->Bind("", port);
  } catch (const std::runtime_error &err) {
    std::cout << "Error trying to start server: " << err.what() << std::endl;
  }

  listener_ = std::thread(&Server::listenLoop, this, port);
};

void Server::Stop() {
  if (!running_.load())
    return;
  running_ = false;
  if (listener_.joinable()) {
    listener_.join();
  }

  sock_->Close();
};

void Server::BroadCastMessage(std::string payload) {
  for (auto c : clients_) {
    try {
      sock_->SendTo(c.address, c.port, payload);
    } catch (const std::runtime_error &err) {
      std::cout << "Failed to broadcast: " << err.what() << std::endl;
      handleFailedSendConn(c);
    }
  }
};

std::vector<Msg> Server::GetAllMessage() {
  std::lock_guard<std::mutex> lk(q_mtx_);
  std::vector<Msg> out;
  out.swap(mqueue_);
  return out;
};

void Server::handleNewConn(Conn conn) {
  std::cout << "Recevied connect req from: " << conn.address << ":" << conn.port
            << std::endl;
  sock_->SendTo(conn.address, conn.port, compatible_id_);
  clients_.insert(conn);
};

void Server::handleFailedSendConn(Conn conn) {
  if (retried_.count(conn) >= kRetried) {
    std::lock_guard<std::mutex> lk(c_mtx_);
    retried_.erase(conn);
    clients_.erase(conn);
  }

  retried_[conn]++;
}

void Server::initSocket() {
#ifdef _WIN32
  sock_ = std::make_shared<WindowsSocket>();
#else
  // TODO: extend to more than win
  throw std::runtime_error("Haven't got support for current platform yet");
#endif
};

void Server::listenLoop(int port) {
  while (running_.load()) {

    bool have_msg = false;
#ifdef _WIN32
    auto *ws = dynamic_cast<WindowsSocket *>(sock_.get());
    if (ws->HasReadable(200)) {
      have_msg = true;
    }
#endif

    if (!running_.load()) {
      break;
    };
    if (!have_msg) {
      continue;
    }

    std::string from;
    int port;
    std::string payload;
    try {
      payload = sock_->ReceiveFrom(from, port);
    } catch (const std::runtime_error &e) {
      std::cout << "Recevied from error: " << e.what() << std::endl;
      if (!running_.load()) {
        break;
      }
      continue;
    }

    {
      std::lock_guard<std::mutex> lk(c_mtx_);
      Conn new_c(from, port);
      const std::string suffix = ":connect_req";
      if (!clients_.count(new_c) && payload.ends_with(suffix)) {
        std::string sent_id = payload.substr(0, payload.size() - suffix.size());
        if (sent_id == compatible_id_) {
          handleNewConn(new_c);
        }
      } else {
	if(retried_.find(new_c) != retried_.end()) {retried_.erase(new_c);};
        std::lock_guard<std::mutex> lk(q_mtx_);
        Msg new_m(from, port, payload);
        mqueue_.push_back(new_m);
      }
    }
  }
};

} // namespace Bored::Net
