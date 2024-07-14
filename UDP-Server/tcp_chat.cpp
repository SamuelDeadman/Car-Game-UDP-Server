#include "tcp_chat.h"
#include "map.hpp"
#include "receiver.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <tuple>
#include <list>

tcp_chat::tcp_chat()
{
	//constructor
}

void tcp_chat::recv_loop()
{
	std::cout << "tcp recv_loop started\n" << std::endl;
	sf::TcpListener tcp_listener;
	tcp_listener.listen(60001);
	
	std::list<sf::TcpSocket*> tcp_clients;
	sf::SocketSelector socket_selector;
	socket_selector.add(tcp_listener);
	// loop that waits for new connections
	bool running = true;
	while (running)
	{
		if (socket_selector.wait())
		{
			if (socket_selector.isReady(tcp_listener))
			{
				sf::TcpSocket* client = new sf::TcpSocket;
				if (tcp_listener.accept(*client) == sf::Socket::Done)
				{
					tcp_clients.push_back(client);//list of connected clients
					socket_selector.add(*client); // add to list
				}
				else
				{
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (std::list<sf::TcpSocket*>::iterator it = tcp_clients.begin(); it != tcp_clients.end(); ++it) //looks thought list
				{
					sf::TcpSocket& client = **it;
					if (socket_selector.isReady(client))
					{
						// The client has sent some data so recive
						sf::Packet packet;
						sf::Socket::Status status;
						status = client.receive(packet);
						if (status == sf::Socket::Done)
						{
							std::string mess;
							packet >> mess;
							std::cout << "tcp server recived: " << mess << std::endl;
							for (std::list<sf::TcpSocket*>::iterator it2 = tcp_clients.begin(); it2 != tcp_clients.end(); ++it2) //looks thought list of connected sockets
							{
								sf::TcpSocket& client2 = **it2;
								send(client2,mess);
							}
						}
					}
				}
			}
		}
	}
}

void tcp_chat::send(sf::TcpSocket& client, std::string message)
{
	//std::string message = "hello from server";

	sf::Packet packet;
	packet << message;//inbuild size work

	client.send(packet);
}
