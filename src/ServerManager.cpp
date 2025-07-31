/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:53 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/31 11:59:17 by fdi-cecc         ###   ########.fr       */
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
	// INFO I use "set" to automatically remove doubles

	std::set<std::pair<int, std::string> > uniqueListens;
	for (size_t i = 0; i < _serverData.size(); i++)
	{
		for (size_t j = 0; j < _serverData[i].getListens().size(); j++)
			uniqueListens.insert(_serverData[i].getListens()[j]);
	}
	
	for (std::set<std::pair<int, std::string> >::iterator it = uniqueListens.begin();
			it != uniqueListens.end(); ++it)
		servListen(*it);
}

void ServerManager::servListen(std::pair<int, std::string> _listens)
{
	/* INFO for each element of _listens vector:
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
		std::cerr << "Socket binding error for " << _listens.second << ":" << _listens.first << std::endl;
		close(newsocket);
	}
	
	if (listen(newsocket, 10) < 0) // change back to SOMAXCONN?
	{
		std::cerr << "Listen socket error for " << _listens.second << ":" << _listens.first << std::endl;
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

std::pair<int, std::string> ServerManager::getSocketData(int socketFd)
{
	struct sockaddr_in	socketIn;
	socklen_t			socketInLen = sizeof(socketIn);
	std::memset(&socketIn, 0, socketInLen);
	char	ipStr[INET_ADDRSTRLEN];
	int		portIn;
	
	if (getsockname(socketFd, (struct sockaddr*) &socketIn, &socketInLen) < 0)
		std::cerr << timeStamp() << "getsockname failed for socket num " << socketFd << std::endl;
	
	inet_ntop(AF_INET, &socketIn.sin_addr, ipStr, INET_ADDRSTRLEN);
	portIn = ntohs(socketIn.sin_port);

	std::cout << BLUE << "[Incoming connection data] > " << ipStr << ":" << portIn << RESET << std::endl;
	return std::make_pair(portIn, ipStr);
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
					
					std::string fullRequest; // TODO wrap everything into a "receiveRequest" function
					char buffer[4096]; // HACK i put 4096, but i don't know if it's right
					bool isComplete = false;
					int attempts = 0;
					const int maxAttempts = 100;
					
					while (!isComplete && attempts < maxAttempts)
					{
						ssize_t bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
						if (bytes > 0)
						{
							buffer[bytes] = '\0';
							fullRequest += buffer;
							
							if (fullRequest.find("\r\n\r\n") != std::string::npos)
								isComplete = true;
						}
						else if (bytes == 0)
							break;
						else if (bytes < 0)
						{
							usleep(10000); // wait and try again
							attempts++;
							continue;
						}
					}
					
					if (isComplete && !fullRequest.empty())
					{
						std::cout << timeStamp() << "Request content:\n*****\n" << std::endl;
						std::cout << fullRequest;
						std::cout << "*****" << std::endl;
						
						try {
							HttpRequest req = HttpRequest(std::make_pair<int, std::string>(_socketFd[i], fullRequest), *this);
							std::cout << "path from req: " << req.getPath() << std::endl;
							
							_response.setContent(req.getFullPath());
							_response.setClientFd(clientFd);
							_response.sendResponse();
						}
						catch (const std::exception& e) {
							std::cerr << "Error processing request: " << e.what() << std::endl;
							std::string errorResponse = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
							send(clientFd, errorResponse.c_str(), errorResponse.length(), 0);
						}
					}
					else
					{
						std::cerr << timeStamp() << "Incomplete or empty request received" << std::endl;
						std::string errorResponse = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
						send(clientFd, errorResponse.c_str(), errorResponse.length(), 0);
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

std::vector<ServerData> ServerManager::getServersList()
{
	return _serverData;
}

/* listening testing methods:
netstat -an | grep 8080
ss -ltn // Linux Only
telnet 127.0.0.1 8080
http://localhost:8080 // via browser
*/
