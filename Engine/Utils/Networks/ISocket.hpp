#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace Bored::Net {

enum Family : uint8_t { Unspec, IPv4, IPv6 };
enum Type : uint8_t { Stream, Datagram };
enum Protocol : uint8_t { UDP, TCP };

struct Conn {
  std::string address;
  int port;

  bool operator==(const Conn &other) const noexcept {
    return address == other.address && port == other.port;
  }
};


struct Msg {
  std::string from;
  int port;
  std::string payload;
};

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


// hash
namespace std {
template<>
struct hash<Bored::Net::Conn> {
  size_t operator()(const Bored::Net::Conn& c) const noexcept {
    size_t h1 = hash<std::string>{}(c.address);
    size_t h2 = hash<int>{}(c.port);
    // Better combine than simple XOR:
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};
} // namespace std
