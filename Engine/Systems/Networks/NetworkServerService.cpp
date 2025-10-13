#include "NetworkServerService.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#ifdef _WIN32
#include "../../Utils/Networks/WindowsSocket.hpp"
#endif

namespace Bored::Net {

Server::Server() { initSocket(); };

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

void Server::BroadCastMessage() {

};

std::vector<Msg> Server::GetAllMessage() {
  std::lock_guard<std::mutex> lk(q_mtx_);
  std::vector<Msg> out;
  out.swap(mqueue_);
  return out;
};

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
      ClientConn new_c(from, port);
      bool is_added = false;
      for (auto c : clients_) {
        if (c.port == port && c.address == from)
          is_added = true;
      }

      if (!is_added) {
        clients_.push_back(new_c);
      };
    }

    {
      std::lock_guard<std::mutex> lk(q_mtx_);
      Msg new_m(payload, port, from);
      mqueue_.push_back(new_m);
    }
  }
};

} // namespace Bored::Net
