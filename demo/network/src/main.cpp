#include "Utils/Networks/WindowsSocket.hpp"
#include <iostream>

using namespace Bored::Net;

int run_server(int port) {

  try {
    WindowsSocket srv;
    srv.Open(IPv4, Datagram, UDP);
    srv.Bind("", port); // "" or "0.0.0.0" listens on all interfaces

    std::cout << "UDP echo server on port " << port << "...\n";
    for (;;) {
      std::string fromAddr;
      int fromPort = 0;
      std::string payload = srv.ReceiveFrom(fromAddr, fromPort);
      std::cout << "Got " << payload.size() << " bytes from " << fromAddr << ":"
                << fromPort << " -> '" << payload << "'\n";

      // Echo it back
      srv.SendTo(fromAddr, fromPort, payload);
    }
  } catch (const std::exception &e) {
    std::cerr << "Server error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}

int run_client(int port, const char *msg, const char *host) {
  try {
    WindowsSocket cli;
    cli.Open(IPv4, Datagram, UDP);
    cli.SendTo(host, port, msg);
    std::cout << "Sent: '" << msg << "' to " << host << ":" << port << "\n";

    std::string fromAddr;
    int fromPort = 0;
    std::string reply = cli.ReceiveFrom(fromAddr, fromPort);
    std::cout << "Reply from " << fromAddr << ":" << fromPort << " -> '"
              << reply << "'\n";
  } catch (const std::exception &e) {
    std::cerr << "Client error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    std::cout << "Usage network.exe <mode> <port> <host> <message>"
              << std::endl;
    return 0;
  }
  const std::string mode = argv[1];

  const int port = (argc > 2) ? std::atoi(argv[2]) : 9000;
  const char *msg = (argc > 3) ? argv[3] : "hello via ISocket";

  const char *host = (argc > 4) ? argv[4] : "127.0.0.1";
  std::cout << "Using mode: " << mode << " with port " << port << "& host " << host
            << "& mes " << msg << std::endl;

  if (mode == "server") {
    return run_server(port);
  } else if (mode == "client") {
    return run_client(port, msg, host);
  }
  return 0;
}
