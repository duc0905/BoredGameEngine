#include "Utils/Networks/WindowsSocket.hpp"
#include <iostream>

using namespace Bored::Net;

int main(int argc, char **argv) {
  const int port = (argc > 1) ? std::atoi(argv[1]) : 9000;

  try {
    WindowsSocket srv;
    srv.Open(IPv4, Datagram, UDP);
    srv.Bind("", port); // "" or "0.0.0.0" listens on all interfaces

    std::cout << "UDP echo server on port " << port << "...\n";
    // for (;;) {
    //   std::string fromAddr;
    //   int fromPort = 0;
    //   std::string payload = srv.ReceiveFrom(fromAddr, fromPort);
    //   std::cout << "Got " << payload.size() << " bytes from " << fromAddr << ":"
    //             << fromPort << " -> '" << payload << "'\n";
    //
    //   // Echo it back
    //   srv.SendTo(fromAddr, fromPort, payload);
    // }
  } catch (const std::exception &e) {
    std::cerr << "Server error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
