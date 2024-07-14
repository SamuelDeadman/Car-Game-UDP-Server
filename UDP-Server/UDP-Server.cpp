#include "map.hpp"
#include "tcp_chat.h"
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
	//sf::UdpSocket socket;
	if (socket->bind(50001) != sf::Socket::Done)//bind port 50001
	{
		return -1;
	}
	Receiver r(socket, queue);
	// i am a thread :) 
	std::thread  t1(&Receiver::recv_loop, Receiver(r)); // i might not work ! READ ME IF IT NO WORK !
	tcp_chat t();
	std::thread  t2(&tcp_chat::recv_loop, tcp_chat()); 


	//mutex logic
	Map<std::string, peer> peers;
	while (true)
	{
		message m = queue.pop();
		peers.insert(m); // This will only insert when the key is not there
		//std::cout << "Server main read: \"" << m.first << "\" " << peers.size() << std::endl;
		
	}
	return 0;
}
