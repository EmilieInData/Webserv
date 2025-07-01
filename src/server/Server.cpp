/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/01 17:06:04 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server/Server.hpp"

Server::Server(): _createdTime(time(NULL))
{ 
	std::cout << utilsTimestamp() << "Server Created" << std::endl;
}

Server::Server(Server const &copy)
{
	*this = copy;
}

Server &Server::operator=(Server const &copy)
{
	(void)copy;
	return *this;
}

Server::~Server()
{
	std::cout << utilsTimestamp() << "Server destroyed" << std::endl;
}

time_t Server::servTimeGet() // delete??
{
	return this->_createdTime;
}

void Server::servRun()
{	
	std::cout << utilsTimestamp() << "Server started" << std::endl;
	
	while (true)
	{
		time_t loop = time(NULL);

		while ((difftime(time(NULL), loop)) <= 3)
			;
		
		std::cout << utilsTimestamp() <<  "\nServer running\n";
		// std::cout << "Socket fd: " << this->_testSocket.getSocketFd() << std::endl;
	}
}

void Server::servSetup()
{
	/* creating socket */
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFd < 0)
		std::cerr << "Socket creation error" << std::endl;


	/* setting socket to non blocking as by subject 
	using fcntl() first to get the flags (F_GETFL)
	and then setting them (F_SETFL)*/
	int flags = fcntl(_socketFd, F_GETFL, 0);
	if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) < 0)
		std::cerr << "Nonblocking setup error" << std::endl;

	
	/* using the sockaddr_in structure to store the
	socket address information */
	struct sockaddr_in address; // move this to Server class
	std::memset(&address, 0, sizeof(address));	
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = inet_addr("127.0.0.1"); // for now set to localhost

	/* binding the socket we created to the address we
	have set above */
	if (bind(_socketFd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		std::cerr << "Socket binding error" << std::endl;
		close(_socketFd);
	}
	
	/* setup the socket to listen */
	if (listen(_socketFd, SOMAXCONN) < 0)
	{
		std::cerr << "Listen socket setup error" << std::endl;
		close(_socketFd);
	}
}

/* listening testing methods:
netstat -an | grep 8080
ss -ltn
telnet 127.0.0.1 8080
*/