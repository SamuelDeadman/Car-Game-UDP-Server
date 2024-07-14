#pragma once
#include <SFML/Network.hpp>
class tcp_chat
{
public:
	tcp_chat();
	void recv_loop();
	void send(sf::TcpSocket& client, std::string message);
};

