#pragma once
#include "ISocket.hpp"
#include <WinSock2.h>
#include <string>

namespace Bored::Net {

class WindowsSocket : public ISocket {
public:
  WindowsSocket();
  ~WindowsSocket() override {};

  void Open(Family family, Type type, Protocol proto) override;
  void Close() override;
  void Bind(const std::string &address, const int port) override;
  void SendTo(const std::string &address, const int port,
              const std::string &message) override;
  std::string ReceiveFrom(std::string &out_address, int &out_port) override;
  bool HasReadable(int timeout_ms = 0) override;

private:
  SOCKET sock_ = INVALID_SOCKET;

  static std::string getLastError();

  int mapFamily(Family fam);
  int mapProtocol(Protocol proto);
  int mapType(Type type);
};

} // namespace Bored::Net
