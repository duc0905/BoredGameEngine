#include "Utils/Networks/WindowsSocket.hpp"
#include <Systems/Networks/NetworkClientService.hpp>
#include <Systems/Networks/NetworkServerService.hpp>
#include <iostream>
#include <memory>

using namespace Bored::Net;

int run_server(int port) {

  std::shared_ptr<Server> server = std::make_shared<Server>();
  server->Start(port);

  while (true) {
    char command;
    std::cout << "Input your action: " << std::endl;
    std::cin >> command;

    switch (command) {
    case 'q':
      std::cout << "Exit!";
      return 0;

    case 'e':
      std::cout << "e";
      break;

    case 'g': {
      std::vector<Msg> msg = server->GetAllMessage();
      if (msg.size() == 0) {
        std::cout << "No message found" << std::endl;
        break;
      };
      for (Msg m : msg) {
        std::cout << m.payload << " from " << m.from << ":" << m.port
                  << std::endl;
      };
      break;
    }

    case 's': {
      std::string msg = "Broadcasting";
      server->BroadCastMessage(msg);
      break;
    }

    default:
      std::cout << "Unrecognized command" << std::endl;
    }
  };

  return 0;
  // try {
  // WindowsSocket srv;
  // srv.Open(IPv4, Datagram, UDP);
  // srv.Bind("", port); // "" or "0.0.0.0" listens on all interfaces
  //
  //   std::cout << "UDP echo server on port " << port << "...\n";
  //   for (;;) {
  //     std::string fromAddr;
  //     int fromPort = 0;
  //     std::string payload = srv.ReceiveFrom(fromAddr, fromPort);
  //     std::cout << "Got " << payload.size() << " bytes from " << fromAddr <<
  //     ":"
  //               << fromPort << " -> '" << payload << "'\n";
  //
  //     // Echo it back
  //     srv.SendTo(fromAddr, fromPort, payload);
  //   }
  // } catch (const std::exception &e) {
  //   std::cerr << "Server error: " << e.what() << "\n";
  //   return 1;
  // }
  // return 0;
}

int run_client(int port, const char *msg, const char *host) {

  std::shared_ptr<Client> client = std::make_shared<Client>();
  bool ok = client->Connect(9000, host, port);
  if (!ok) {
    return 0;
  };

  while (true) {
    char command;
    std::cout << "Input your action: " << std::endl;
    std::cin >> command;

    switch (command) {
    case 'q':
      std::cout << "Exit!";
      return 0;

    case 'e':
      std::cout << "e";
      break;

    case 'g': {
      std::vector<std::string> msg = client->GetAllMessage();
      if (msg.size() == 0) {
        std::cout << "No message found" << std::endl;
        break;
      };
      for (auto m : msg) {
        std::cout << "Received: " << m << std::endl;
      };
      break;
    }

    case 's': {
				std::string msg = "Boom!";
				client->SendToServer(msg);
				break;
			}

    default:
      std::cout << "Unrecognized command" << std::endl;
    }
  };

  return 0;
  // try {
  //   WindowsSocket cli;
  //   cli.Open(IPv4, Datagram, UDP);
  //   cli.SendTo(host, port, msg);
  //   bool haveSth = cli.HasReadable();
  //
  //   std::cout << "Have message?: " << haveSth << std::endl;
  //   std::cout << "Sent: '" << msg << "' to " << host << ":" << port << "\n";
  //
  //   std::string fromAddr;
  //   int fromPort = 0;
  //   haveSth = cli.HasReadable(500);
  //   // std::cout << "Have message?: " << haveSth << std::endl;
  //   // std::string reply = cli.ReceiveFrom(fromAddr, fromPort);
  //   // std::cout << "Reply from " << fromAddr << ":" << fromPort << " -> '"
  //   // << reply << "'\n";
  // } catch (const std::exception &e) {
  //   std::cerr << "Client error: " << e.what() << "\n";
  //   return 1;
  // }
  // return 0;
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
  std::cout << "Using mode: " << mode << " with port " << port << "& host "
            << host << "& mes " << msg << std::endl;

  if (mode == "server") {
    return run_server(port);
  } else if (mode == "client") {
    return run_client(port, msg, host);
  }
  return 0;
}
