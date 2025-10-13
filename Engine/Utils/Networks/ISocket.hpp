#pragma once
#include <cstdint>
#include <string>

namespace Bored::Net {

enum Family : uint8_t { Unspec, IPv4, IPv6 };
enum Type : uint8_t { Stream, Datagram };
enum Protocol : uint8_t { UDP, TCP };

class ISocket {

public:
  ISocket() = default;
  virtual ~ISocket() = default;

  virtual void Open(Family family, Type type, Protocol proto) = 0;
  virtual void Close() = 0;
  virtual void Bind(const std::string &address, const int port) = 0;
  virtual void SendTo(const std::string &address, const int port,
                      const std::string &message) = 0;
  virtual std::string ReceiveFrom(std::string &out_address, int &out_port) = 0;
  virtual bool HasReadable(int timeout_ms = 0) = 0;
};

} // namespace Bored::Net
