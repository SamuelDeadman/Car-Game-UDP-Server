#include "receiver.hpp"
#include "util.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>
#include <tuple>

Receiver::Receiver(std::shared_ptr<sf::UdpSocket> s,
        Queue<message> &q):
    socket_(s),
    queue_(q)//use this to swap data between threads
{
}

void Receiver::recv_loop()
{
	//sf::IpAddress local(127, 0, 0, 1);
	sf::IpAddress local(0, 0, 0, 0); //uses all network addresses
	sf::IpAddress allnet(255, 255, 255, 255);// broardcast to all on subnet
	std::cout << "receiver loop started" << std::endl;
	sf::Socket::Status status;
	const std::size_t bufferSize = 256;//assumbtion recived packet is 256 large
    char buffer[bufferSize];
    std::size_t received;
    sf::IpAddress address;
    unsigned short port;
	unsigned short portlist[6]; //assumption max players per game 6
	for (int i = 0; i < 6; i++)
		portlist[i] = 0;
	bool newport = false;
    while(true) // forever loop if exit thread "dies"
    {
        std::memset(buffer, 0, bufferSize);//buffer clean

		status = socket_->receive((void*) buffer, bufferSize, received, address, port); //in send package address and port in front
		if ((status != sf::Socket::Done) && (status != sf::Socket::Disconnected))
		{
			std::cout << "reciver loop exited" << status << std::endl;
			return; // ERROR !
		}
		newport = false;
		int i = 0;
		while (portlist[i] != 0)
		{
			if (portlist[i] == port)
				newport = true; // if portlist found then dont add it again
			i++;
		}
		if (newport == false)
			portlist[i] = port; //ad port if new to list
		
		float x, y, z; //z is angle
		memcpy(&x, buffer + 6,  sizeof(x));
		memcpy(&y, buffer + 6 + sizeof(x),  sizeof(y));
		memcpy(&z, buffer + 6 + sizeof(x) + sizeof(y),  sizeof(z)); //reciver code USE PORT AT KEY IN STORAGE LAYER

		//Furture inprovment, save values on server allowing for checks / usage later on 

		//above manualy creates the packet to send
		//sf::Packet packet;
		//packet << x << s << d;
		//above code could be used to made packet, however there is no way to knowing or validate if the packet is made correctly saving no real time
		//sf packet has no execption handleing "data is interpreted correctly according to the endianness"

		//std::cout << "received: \"" << x << "\"" << y << "\"" << z << "\"" << received << "\"" << std::endl;
		//std::cout << "port number\"" << port << "\"" << std::endl;
		
        //queue_.push(std::make_pair(std::string(buffer), std::make_pair(address, port)));

		// send logic

		int offset = 6 + sizeof(x) + sizeof(y) + sizeof(z);
		unsigned int IP4 = address.toInteger();
		memcpy(buffer + offset, &IP4, 4);
		memcpy(buffer + offset + 4 , &port, sizeof(port));

		for (int i = 0; i < 6; i++)
		{
			unsigned short sport = portlist[i];
			if ((sport != port)&&(sport != 0)) // does not send it back to itself 
			{
				int bsize = sizeof(buffer);//buffer size
				status = socket_->send(buffer, bsize, allnet, sport);
				if (status != sf::Socket::Done)
				{
					std::cout << "send loop exited" << status << std::endl;
					return; // ERROR !
				}
				//std::cout << "DATA SENT OUT !" << std::endl;
			}
				
		}
    }
}

