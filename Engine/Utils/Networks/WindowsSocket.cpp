#include "WindowsSocket.hpp"
#include <WinSock2.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // for InetPtonA

namespace Bored::Net {

std::string WindowsSocket::getLastError() {
  return std::to_string(WSAGetLastError());
}

int WindowsSocket::mapFamily(Family fam) {
  switch (fam) {
  case Unspec:
    return AF_UNSPEC;
  case IPv4:
    return AF_INET;
  case IPv6:
    return AF_INET6;
  default:
    return -1;
  }
}

int WindowsSocket::mapProtocol(Protocol proto) {
  switch (proto) {
  case UDP:
    return IPPROTO_UDP;
  case TCP:
    return IPPROTO_TCP;
  default:
    return -1;
  }
}

int WindowsSocket::mapType(Type type) {
  switch (type) {
  case Stream:
    return SOCK_STREAM;
  case Datagram:
    return SOCK_DGRAM;
  default:
    return -1;
  }
}

WindowsSocket::WindowsSocket() {
  WSADATA wsadata;

  int result = 0;
  result = WSAStartup(MAKEWORD(2, 2), &wsadata);
  if (result != NO_ERROR) {
    std::cout << "WSAStartup failed to initialized" << std::endl;
  };
}


void WindowsSocket::Open(Family family, Type type, Protocol proto) {
  if (sock_ != INVALID_SOCKET) {
    std::cout << "Socket already open, create a new one if needed" << std::endl;
  }
  sock_ = socket(mapFamily(family), mapType(type), mapProtocol(proto));
  if (sock_ == INVALID_SOCKET) {
    std::cout << "Failed to open a new socket" << std::endl;
  }
}

void WindowsSocket::Close() {
  if (sock_ == INVALID_SOCKET)
    return;

  if (closesocket(sock_) == SOCKET_ERROR) {
    throw std::runtime_error("Failed to close socket");
  }

  WSACleanup();
}

void WindowsSocket::Bind(const std::string &address, const int port) {
  sockaddr_in bind_address;
  // TODO: Fix this hardcode later
  bind_address.sin_family = AF_INET;
  bind_address.sin_port = htons(port);
  if (address.empty() || address == "*" || address == "0.0.0.0") {
    bind_address.sin_addr.s_addr = INADDR_ANY;
  } else {
    if (InetPtonA(AF_INET, address.c_str(), &bind_address.sin_addr) != 1) {
      throw std::runtime_error("InetPton failed for '" + address + "'");
    }
  }

  if (bind(sock_, reinterpret_cast<sockaddr *>(&bind_address),
           sizeof(bind_address)) == SOCKET_ERROR) {
    throw std::runtime_error("bind() failed: " + getLastError());
  }
}

void WindowsSocket::SendTo(const std::string &address, const int port,
                           const std::string &message) {
  return;
}

std::string WindowsSocket::ReceiveFrom(std::string &out_address,
                                       int &out_port) {
  return "";
}

} // namespace Bored::Net
