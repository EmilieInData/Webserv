/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/23 12:21:53 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "HttpRequest.hpp"
#include "ParsingConf.hpp"

// Server::Server(): _createdTime(time(NULL))
// { 
// 	std::cout << utilsTimestamp() << "Server Created" << std::endl;
// }

Server::Server(ServerData& servData) : _createdTime(time(NULL))
{
	serverInit(servData);
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

void Server::servListen(std::pair<int, std::string> _listens)
{
	/* for each element of _listens vector:
			create _socketFD
			set flags to nonblock
			setup struct
			bind socket to struct
			setup listen */
		int					newsocket = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in	newaddr;
		
		if (newsocket < 0)
			std::cerr << "Socket creation error" << std::endl;
		
		int flags = fcntl(newsocket, F_GETFL, 0);
		if (fcntl(newsocket, F_SETFL, flags | O_NONBLOCK) < 0)	
		std::cerr << "Nonblocking setup error" << std::endl;  	
		
		std::memset(&newaddr, 0, sizeof(newaddr));	
		newaddr.sin_family = AF_INET;                                                	
		newaddr.sin_port = htons(_listens.first);
		newaddr.sin_addr.s_addr = inet_addr(_listens.second.c_str()); 
		
		if (bind(newsocket, (struct sockaddr*)&newaddr, sizeof(newaddr)) < 0)
		{
			std::cerr << "Socket binding error" << std::endl;
			close(newsocket);
		}

		if (listen(newsocket, 10) < 0) // change back to SOMAXCONN?
		{
			std::cerr << "Listen socket setup error" << std::endl;
			close(newsocket);
		}
}

void Server::serverInit(ServerData &servData)
{

}

void Server::servStart()
{	
	std::cout << utilsTimestamp() << "Server started" << std::endl;
	
	_pollFd[0].fd = _socketFd;
	_pollFd[0].events = POLLIN;
	
	std::cout << utilsTimestamp() << "Waiting for connection on port 8080" << std::endl;
	
	while (true)
	{
		int check = poll(_pollFd, 1, 5000);
		if (check < 0)
		{
			std::cerr << "Poll error" << std::endl;
			break ;
		}
		else if (check == 0)
		{
			std::cout << utilsTimestamp() << "Still waiting for connection" << std::endl;
			continue ;
		}
		
		if (_pollFd[0].revents & POLLIN)
		{
			socklen_t clientLen = sizeof(_clientAddr);
			_clientFd = accept(_socketFd, (struct sockaddr *)&_clientAddr, &clientLen);
			if (_clientFd >= 0)
			{
				std::cout << utilsTimestamp() << "New connection accepted" << std::endl;
				char buffer[4096];
				ssize_t bytes = recv(_clientFd, buffer, sizeof(buffer), 0);
				if (bytes > 0)
					buffer[bytes] = '\0';
		/*		std::cout << utilsTimestamp() << "Request content:\n*****\n" << std::endl;
				std::cout << buffer;
				std::cout << "*****" << std::endl;
		*/

				HttpRequest	req = HttpRequest( buffer, *this );
				
				std::string response = 
				"HTTP/1.1 200 OK\n"
				"Content-Type: text/html\n"
				"Content-Length: 85\n"
				"\n"
				"<html><body><h1>Bonjour!</h1></body></html>";
			
				send(_clientFd, response.c_str(), response.size(), 0);
			
				close(_clientFd);
			}
		}
	}
	close(_socketFd);
	
}

// std::vector<ServerData> const&	Server::getServersList() const
// {
// 	return _serversList;
// }

std::map<int, std::pair<std::string, std::string> > const&	Server::getDefaultErrorPages() const
{
	return _defaultErrorPages;
}


/* listening testing methods:
netstat -an | grep 8080
ss -ltn // Linux Only
telnet 127.0.0.1 8080
http://localhost:8080 // via browser
*/
