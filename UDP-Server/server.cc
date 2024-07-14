#include "map.hpp"
#include "receiver.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <tuple>

int main()
{
    Queue<message> queue;
    std::shared_ptr<sf::UdpSocket> socket = std::make_shared<sf::UdpSocket>();
    // TODO: we need to bind our socket to a port
    Receiver r{socket, queue};
    // TODO: we need to start a receiver thread.

    // This is tricky:
    // We want to identify the clients by their nicknames?
    // Alternatively we could identify the clients with their port numbers.
    // The reason is that all clients are running on the same machine.
    // They will all have a different port number but the same address.
    Map<std::string, peer> peers;
    while(true)
    {
        message m = queue.pop();
        peers.insert(m); // This will only insert when the key is not there
        std::cout << "Server main read: \"" << m.first << "\" " << peers.size() << std::endl;
        // TODO: We need to send the message to everyone in peers.
        // We need to do it in a thread safe way.
    }
    return 0;
}
