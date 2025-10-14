#include "NetworkClientService.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>

#ifdef _WIN32
#include "../../Utils/Networks/WindowsSocket.hpp"
#endif

namespace Bored::Net {

Client::Client(std::string compatible_id) {
  initSocket();
  compatible_id_ = compatible_id;
};

Client::~Client() { Disconnect(); };

bool Client::Connect(int port, std::string addr, int server_port) {
  if (running_.load())
    return false;

  running_ = true;

  try {

    sock_->Open(IPv4, Datagram, UDP);
    sock_->Bind("", port);
  } catch (const std::runtime_error &err) {
    std::cout << "Unable to connect: " << err.what() << std::endl;
    running_ = false;
  }

  server_addr = Conn(addr, server_port);
  sock_->SendTo(addr, server_port, compatible_id_ + ":connect_req");

  bool success = false;
  auto start_time = std::chrono::high_resolution_clock().now();
  while (std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now() - start_time)
             .count() < kConnectTimeOut * 1000) {
    if (!sock_->HasReadable(100))
      continue;
    try {
      std::string ack =
          sock_->ReceiveFrom(server_addr.address, server_addr.port);
      if (ack == compatible_id_) {
        success = true;
      }
    } catch (const std::runtime_error &e) {
      continue;
    }
  }

  if (!success) {
    running_ = false;
    sock_->Close();
    std::cout << "Time out while connecting with server" << std::endl;
    return false;
  }

  listener_ = std::thread(&Client::listenLoop, this, port);
  return true;
};

void Client::Disconnect() {
  if (!running_.load()) {
    return;
  }

  running_ = false;
  sock_->Close();

  if (listener_.joinable())
    listener_.join();
};

void Client::initSocket() {
#ifdef _WIN32
  sock_ = std::make_shared<WindowsSocket>();
#else
  throw std::runtime_error("No implementation for other platform yet");
#endif
};

void Client::listenLoop(int port) {
  while (running_.load()) {
    bool have_msg = false;
#ifdef _WIN32
    auto *ws = dynamic_cast<WindowsSocket *>(sock_.get());
    if (ws->HasReadable(100)) {
      have_msg = true;
    }
#endif

    if (!have_msg) {
      continue;
    };

    std::string from, payload;
    int port;
    try {
      payload = sock_->ReceiveFrom(from, port);
      if (Conn(from, port) != server_addr) {
        continue;
      };
    } catch (const std::runtime_error &err) {
      continue;
    }

    {
      std::lock_guard<std::mutex> lk(q_mtx_);
      mqueue_.push_back(payload);
    }
  }
};

std::vector<std::string> Client::GetAllMessage() {
  std::lock_guard<std::mutex> lk(q_mtx_);
  std::vector<std::string> out;
  out.swap(mqueue_);
  return out;
};

void Client::SendToServer(std::string payload) {
  try {
    sock_->SendTo(server_addr.address, server_addr.port, payload);
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to send " << e.what() << std::endl;
  }
}

} // namespace Bored::Net
