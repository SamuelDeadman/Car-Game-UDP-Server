#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "queue.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>
#include <tuple>

class Receiver {
    public:
		Receiver(std::shared_ptr<sf::UdpSocket> s, Queue<message>& queue);
        void recv_loop();
    private:
        std::shared_ptr<sf::UdpSocket> socket_;
        Queue<message> &queue_;
};
#endif
