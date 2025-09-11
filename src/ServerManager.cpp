/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:53 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/11 18:39:33 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerManager.hpp"
#include "HttpRequest.hpp"
#include <fcntl.h>
#include <time.h>

ServerManager::ServerManager(ParsingConf &parsData) : _running(false), _reqCount(0), _rspCount(0)
{
	_serverData = parsData.servers;
}

ServerManager::~ServerManager() {}

void ServerManager::servSetup()
{
	// INFO I use "set" to automatically remove doubles

	for (size_t i = 0; i < _serverData.size(); i++)
	{
		for (size_t j = 0; j < _serverData[i].getListens().size(); j++)
			_uniqueListens.insert(_serverData[i].getListens()[j]);
	}

	graTopLine();
	graTime("Listening Sockets Setup");
	graEmptyLine();
	for (std::set<std::pair<int, std::string> >::iterator it = _uniqueListens.begin();
		 it != _uniqueListens.end(); ++it)
		servListen(*it);
	graBottomLine();
}

void ServerManager::servListen(std::pair<int, std::string> _listens)
{
	/* INFO for each element of _listens vector:
	create _socketFD
	set flags to nonblock
	setup struct
	bind socket to struct
	setup listen */
	int				   newsocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in newaddr;

	if (newsocket < 0)
		graError("Socket creation error");

	int flags = fcntl(newsocket, F_GETFL, 0);
	if (fcntl(newsocket, F_SETFL, flags | O_NONBLOCK) < 0)
		graError("Nonblocking setup error");

	std::memset(&newaddr, 0, sizeof(newaddr));
	newaddr.sin_family = AF_INET;
	newaddr.sin_port   = htons(_listens.first);
	newaddr.sin_addr.s_addr = inet_addr(_listens.second.c_str()); // TODO check if inet_addr can be used

	if (bind(newsocket, (struct sockaddr *)&newaddr, sizeof(newaddr)) < 0)
	{
		graError("Binding error for " + _listens.second + ":" + intToString(_listens.first));
		close(newsocket);
	}

	if (listen(newsocket, 10) < 0) // change back to SOMAXCONN?
	{
		graError("Listen error for " + _listens.second + ":" + intToString(_listens.first));
		close(newsocket);
	}

	_socketFd.push_back(newsocket);
	_servAddr.push_back(newaddr);
	graTextElement(_listens.second + ":" + intToString(_listens.first));
}

struct pollfd *ServerManager::servPoll(size_t totalSocket)
{
	struct pollfd *polls = new pollfd[totalSocket + 1]; // FABIO added listening socket for terminal input

	graTopLine();
	graTime("Poll Setup");
	graTextHeader("Number of unique listening sockets");
	graTextElement(intToString(_socketFd.size()));
	graTextHeader("Polls and methods");
	for (size_t i = 0; i < totalSocket; i++)
	{
		polls[i].fd		= _socketFd[i];
		polls[i].events = POLLIN;
		graTextElement("Socket fd: " + intToString(polls[i].fd) +
					   " setup to event: " + intToString(polls[i].events));
	}
	polls[totalSocket].fd	  = STDIN_FILENO; // FABIO for input reading
	polls[totalSocket].events = POLLIN;
	_inputFd				  = polls[totalSocket].fd;
	graBottomLine();
	return polls;
}

std::pair<int, std::string> ServerManager::getSocketData(int socketFd)
{
	struct sockaddr_in socketIn;
	socklen_t		   socketInLen = sizeof(socketIn);
	std::memset(&socketIn, 0, socketInLen);
	char ipStr[INET_ADDRSTRLEN];
	int	 portIn;

	if (getsockname(socketFd, (struct sockaddr *)&socketIn, &socketInLen) < 0)
		std::cerr << timeStamp() << "getsockname failed for socket num " << socketFd << std::endl;

	inet_ntop(AF_INET, &socketIn.sin_addr, ipStr, INET_ADDRSTRLEN);
	portIn = ntohs(socketIn.sin_port);

	return std::make_pair(portIn, ipStr);
}

bool ServerManager::servReceive(ClientConnection &connection, HttpRequest &req)
{
	bool isComplete = false;

	if (connection.clientFd >= 0)
	{
		printBoxMsg("New connection accepted");

		char	  buffer[10];
		int		  attempts	  = 0;
		const int maxAttempts = 100;

		time_t start, check;
		time(&start);

		while (!isComplete && attempts < maxAttempts)
		{
			ssize_t bytes = recv(connection.clientFd, buffer, sizeof(buffer) - 1, 0);

			int flags = fcntl(connection.clientFd, F_GETFL, 0);
			fcntl(connection.clientFd, F_SETFL, flags | O_NONBLOCK);

			if (bytes > 0)
			{
				connection.fullRequest.append(buffer, bytes); // DBG to remove
				buffer[bytes] = '\0';

				connection.fullRequest = buffer;

				req.sendBuffer(buffer, bytes); //CLEO the max_body_size is checked in the request

				if (req.getParsingState() == DONE)
					std::cout << "STATE: " << req.getParsingState() << std::endl;

				if (req.getParsingState() <= 0)
					isComplete = true;
			}
			else if (bytes == 0)
			{
				std::cout << "BYTES = 0" << std::endl;
				return false;
			}
			else if (bytes < 0)
			{
				time(&check);
				if (req.getParsingState() >= SKIP && req.getParsingState() <= REQ_LINE &&
					difftime(check, start) > 0.5)
				{
					isComplete = true;
					req.setStatusCode(E_400);
				}
				else if (req.getParsingState() == HEADERS &&
						 difftime(check, start) > CLIENT_HEADER_TIMEOUT)
				{
					isComplete = true;
					req.setStatusCode(E_408);
				}
				else if (req.getParsingState() == BODY && difftime(check, start) > CLIENT_BODY_TIMEOUT)
				{
					isComplete = true;
					req.setStatusCode(E_408);
				}
			}
		}
	}
	// std::cout << GREEN << connection.fullRequest << RESET << std::endl; // TODO delete when done
	//printRaw(connection.fullRequest);
	req.printBodies(); // DBG
	return isComplete;
}

void ServerManager::servRespond(ClientConnection &connection, HttpRequest &req,
								std::pair<int, std::string> incoming)
{
	(void)incoming;


//	try
//	{
		Response	resp(req);
		std::string fullPath = req.getFullPath().first +
								   req.getFullPath().second; // TODO make error management for bad request
		//	printRequest(*this, _socketFd[connection.socketIndex], connection.fullRequest, fullPath,
		//				 req.getHttpMethod());
		if (req.getStatusCode() < 400)
			resp.setContent(req.getFullPath(), req.getHttpMethod());
		resp.setClientFd(connection.clientFd);
		resp.prepResponse();
		resp.sendResponse();
		_rspCount++;
		printResponse(*this, incoming, resp.getResponse(), fullPath);
//	}
//	catch (const std::exception &e) // TODO check with errors because they happen in HttpRequest
//	{
//		std::cerr << "Error processing request: " << e.what() << std::endl;
//		std::string errorResponse = "HTTP/1.1 400 Bad Request\r\nContent-Length: "
//									"0\r\nConnection: close\r\n\r\n";
//		send(connection.clientFd, errorResponse.c_str(), errorResponse.length(), 0);
//	}
}

void ServerManager::servIncoming(struct pollfd *polls, const size_t socketsize)
{
	ClientConnection connection;

	for (size_t i = 0; i < socketsize + 1; i++)
	{
		if (polls[i].revents & POLLIN)
		{
			if (polls[i].fd == _inputFd)
			{
				servInput();
				continue;
			}
			_reqCount++;
			connection.socketIndex = i;
			connection.clientFd	   = accept(_socketFd[i], (struct sockaddr *)&connection.clientAddr,
											&connection.clientLen);

			std::pair<int, std::string> incoming = getSocketData(_socketFd[connection.socketIndex]);

			HttpRequest req = HttpRequest(incoming, *this);

			if (servReceive(connection, req))
			{
				servRespond(connection, req, incoming);
			}
			else
			{
				printBoxError("Incomplete or empty request received");
				std::string errorResponse = "HTTP/1.1 400 Bad Request\r\nContent-Length: "
											"0\r\nConnection: close\r\n\r\n";
				send(connection.clientFd, errorResponse.c_str(), errorResponse.length(), 0);
			}
			close(connection.clientFd);
		}
	}
}

void ServerManager::servRun()
{
	const size_t socketsize = _socketFd.size();

	struct pollfd *polls = servPoll(socketsize);

	_running = true;

	printBoxMsg("Server Running");

	while (_running == true)
	{
		int check = poll(polls, socketsize + 1, 5000);
		if (check < 0)
		{
			if (errno == EINTR) // FABIO here errno ok here i think
				continue;
			else
			{
				printBoxError("Poll error");
				break;
			}
		}
		else if (check == 0)
			continue;
		servIncoming(polls, socketsize);
	}
	for (size_t i = 0; i < _socketFd.size(); i++)
		close(_socketFd[i]);
	delete[] polls;
}

std::vector<ServerData> ServerManager::getServersList() const
{
	return _serverData;
}

Script	&ServerManager::getScript()
{
	return _script;
}

int ServerManager::getReqCount() const
{
	return _reqCount;
}

int ServerManager::getRspCount() const
{
	return _rspCount;
}

std::set<std::pair<int, std::string> > ServerManager::getUniqueListens()
{
	return _uniqueListens;
}

void ServerManager::servQuit()
{
	if (_running == true)
		_running = false;

	printBoxMsg("Server quit");
}

void ServerManager::servInput()
{
	char   buffer[256];
	size_t charsRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (charsRead > 0)
	{
		buffer[charsRead] = '\0';
		std::string cmd(buffer);

		if (!cmd.empty() && cmd[cmd.length() - 1] == '\n')
			cmd.erase(cmd.length() - 1);

		if (cmd == "quit" || cmd == "q")
			servQuit();
		else if (cmd == "status")
			printServersStatus(*this);
		else
			printBoxError("Command unavailable");
	}
}

/* listening testing methods:
netstat -an | grep 8080
ss -ltn // Linux Only
telnet 127.0.0.1 8080
http://localhost:8080 // via browser
*/
