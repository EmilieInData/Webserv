/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:53 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/28 10:09:16 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerManager.hpp"

ServerManager::ServerManager(ParsingConf &parsData) 
{
	_serverData = parsData.servers;
}

ServerManager::~ServerManager() {}

void ServerManager::servSetup()
{
	for (size_t i = 0; i < _serverData.size(); i++)
	{
		for (size_t j = 0; j < _serverData[i].getListens().size(); j++)
			servListen(_serverData[i].getListens()[j]);
	
	}
}

void ServerManager::servListen(std::pair<int, std::string> _listens)
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
	
	_socketFd.push_back(newsocket);
	_servAddr.push_back(newaddr);
	std::cout << timeStamp() << "Socket set: " << _listens.second << ":" << _listens.first << " Number:" << newsocket << std::endl;
}

struct pollfd *ServerManager::servPoll(size_t totalSocket)
{	
	struct pollfd *polls = new pollfd[totalSocket];
	
	for (size_t i = 0; i < totalSocket; i++)
	{
		polls[i].fd = _socketFd[i];
		polls[i].events = POLLIN;
		std::cout << "poll for " << polls[i].fd << " " << polls[i].events << "setup" << std::endl;
	}

	return polls;
}

void ServerManager::servRun()
{
	const size_t socketsize = _socketFd.size();
	
	std::cout << timeStamp() << "Number of listening sockets: " << _socketFd.size() << std::endl;
	
	struct pollfd *polls = servPoll(socketsize);	
	
	while (true)
	{
		int check = poll(polls, socketsize, 5000);
		if (check < 0)
		{
			std::cerr << "Poll error" << std::endl;
			break ;
		}
		else if (check == 0)
		{
			std::cout << timeStamp() << "Still waiting for connection" << std::endl;
			continue ;
		}
		for (size_t i = 0; i < socketsize; i++)
		{
			if (polls[i].revents & POLLIN)
			{
				std::cout << timeStamp() << "POLLIN at " << polls[i].fd << std::endl;
				int					clientFd;
				struct sockaddr_in	clientAddr;
				socklen_t			clientLen = sizeof(clientAddr);
				clientFd = accept(_socketFd[i], (struct sockaddr *)&clientAddr, &clientLen);
				if (clientFd >= 0)
				{
					std::cout << timeStamp() << "New connection accepted" << std::endl;
					char buffer[4096] = {0};
					ssize_t bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
					if (bytes > 0)
					{
						buffer[bytes] = '\0';
						std::cout << timeStamp() << "Request content:\n*****\n" << std::endl;
						std::cout << buffer;
						std::cout << "*****" << std::endl;
						
						std::string location = "/Users/palmiro/42/webserv/test/page.html";
						_response.setContent(location);
						_response.setClientFd(clientFd);
						_response.sendResponse();
					}
					close(clientFd);
				}
				else
					std::cerr << "Send failed" << std::endl;
			}
		}		
	}
	for (size_t i = 0; i < _socketFd.size(); i++)
		close(_socketFd[i]);
	delete[] polls;
}


/* listening testing methods:
netstat -an | grep 8080
ss -ltn // Linux Only
telnet 127.0.0.1 8080
http://localhost:8080 // via browser
*/
