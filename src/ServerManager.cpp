/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:53 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/14 16:05:44 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerManager.hpp"
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

Client::Client(int client_fd, struct sockaddr_in client_addr, ServerManager &manager, std::pair<int, std::string> incoming)
	: fd(client_fd), addr(client_addr), addrLen(sizeof(addr)), request(incoming, manager), response(request), bytes_sent(0)
{
	time(&last_action_time);
}

Client::~Client() {}

ServerManager::ServerManager(ParsingConf &parsData) : _running(false), _reqCount(0), _rspCount(0)
{
	_serverData = parsData.servers;
}

ServerManager::~ServerManager() {}

void ServerManager::servSetup()
{
	for (size_t i = 0; i < _serverData.size(); i++)
	{
		for (size_t j = 0; j < _serverData[i].getListens().size(); j++)
			_uniqueListens.insert(_serverData[i].getListens()[j]);
	}
	graTopLine();
	graTime("Listening Sockets Setup");
	graEmptyLine();
	for (std::set<std::pair<int, std::string> >::iterator it = _uniqueListens.begin(); it != _uniqueListens.end(); ++it)
		servListen(*it);
	graBottomLine();
}

void ServerManager::servListen(std::pair<int, std::string> _listens)
{
	int newsocket = socket(AF_INET, SOCK_STREAM, 0);
	if (newsocket < 0)
		return graError("Socket creation error");

	int opt = 1;
	if (setsockopt(newsocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(newsocket);
		return graError("setsockopt error");
	}

	if (fcntl(newsocket, F_SETFL, O_NONBLOCK) < 0)
	{
		close(newsocket);
		return graError("Nonblocking setup error");
	}

	struct sockaddr_in newaddr;
	std::memset(&newaddr, 0, sizeof(newaddr));
	newaddr.sin_family		= AF_INET;
	newaddr.sin_port		= htons(_listens.first);
	newaddr.sin_addr.s_addr = inet_addr(_listens.second.c_str());

	if (bind(newsocket, (struct sockaddr *)&newaddr, sizeof(newaddr)) < 0)
	{
		close(newsocket);
		return graError("Binding error for " + _listens.second + ":" + intToString(_listens.first));
	}

	if (listen(newsocket, 128) < 0)
	{
		close(newsocket);
		return graError("Listen error for " + _listens.second + ":" + intToString(_listens.first));
	}

	_socketFd.push_back(newsocket);
	_servAddr.push_back(newaddr);
	graTextElement(_listens.second + ":" + intToString(_listens.first));
}

void ServerManager::servPollSetup()
{
	_polls.clear();
	for (size_t i = 0; i < _socketFd.size(); ++i)
	{
		struct pollfd pfd;
		pfd.fd		= _socketFd[i];
		pfd.events	= POLLIN;
		pfd.revents = 0;
		_polls.push_back(pfd);
	}
	struct pollfd stdinfd;
	stdinfd.fd		= STDIN_FILENO;
	stdinfd.events	= POLLIN;
	stdinfd.revents = 0;
	_polls.push_back(stdinfd);
}

void ServerManager::servRun()
{
	servPollSetup();
	_running = true;
	printBoxMsg("Server Running");

	while (_running)
	{
		int check = poll(&_polls[0], _polls.size(), 5000);
		if (check < 0)
		{
			if (errno == EINTR)
				continue;
			printBoxError("Poll error");
			break;
		}

		for (size_t i = 0; i < _polls.size(); ++i)
		{
			if (_polls[i].revents == 0)
				continue;

			if (_polls[i].fd == STDIN_FILENO)
			{
				if (_polls[i].revents & POLLIN)
				{
					servInput();
				}
				continue;
			}

			bool is_listener = false;
			for (size_t j = 0; j < _socketFd.size(); ++j)
			{
				if (_polls[i].fd == _socketFd[j])
				{
					is_listener = true;
					break;
				}
			}

			if (_polls[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				if (is_listener)
					printBoxError("Error on listener socket " + intToString(_polls[i].fd));
				else
					removeClient(_polls[i].fd);
				continue;
			}
			if (_polls[i].revents & POLLIN)
			{
				if (is_listener)
					handleNewConnection(_polls[i].fd);
				else
					handleClientRead(_polls[i].fd);
			}
			else if (_polls[i].revents & POLLOUT)
			{
				handleClientWrite(_polls[i].fd);
			}
		}
	}

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	_clients.clear();
	for (size_t i = 0; i < _socketFd.size(); ++i)
		close(_socketFd[i]);
}

void ServerManager::handleNewConnection(int listener_fd)
{
	struct sockaddr_in client_addr;
	socklen_t		   client_len = sizeof(client_addr);
	int				   client_fd  = accept(listener_fd, (struct sockaddr *)&client_addr, &client_len);

	if (client_fd < 0)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			printBoxError("Accept error: " + std::string(strerror(errno)));
		return;
	}

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		printBoxError("Fcntl error on new client fd " + intToString(client_fd));
		close(client_fd);
		return;
	}

	struct pollfd pfd;
	pfd.fd		= client_fd;
	pfd.events	= POLLIN;
	pfd.revents = 0;
	_polls.push_back(pfd);

	std::pair<int, std::string> incoming = getSocketData(listener_fd);
	_clients[client_fd]					 = new Client(client_fd, client_addr, *this, incoming);

	printBoxMsg("New connection accepted on fd: " + intToString(client_fd));
}

void ServerManager::handleClientRead(int client_fd)
{
	char							  buffer[4096];
	ssize_t							  bytes = recv(client_fd, buffer, sizeof(buffer), 0);
	std::map<int, Client *>::iterator it	= _clients.find(client_fd);
	if (it == _clients.end())
		return;

	if (bytes <= 0)
	{
		if (bytes == 0)
			printBoxMsg("Client " + intToString(client_fd) + " disconnected gracefully.");
		else
			printBoxError("Recv error on client " + intToString(client_fd) + ": " + std::string(strerror(errno)));
		removeClient(client_fd);
		return;
	}

	_reqCount++;
	it->second->request.sendBuffer(buffer, bytes);
	time(&it->second->last_action_time);

	if (it->second->request.getParsingState() <= 0)
	{
		Response &resp = it->second->response;

		resp.setContent(it->second->request.getFullPath(), it->second->request.getHttpMethod());

		resp.prepResponse(getSocketData(it->second->fd));
		it->second->response_data = resp.getResponse();

		for (size_t i = 0; i < _polls.size(); ++i)
		{
			if (_polls[i].fd == client_fd)
			{
				_polls[i].events = POLLOUT;
				break;
			}
		}
	}
}

void ServerManager::handleClientWrite(int client_fd)
{
	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	if (it == _clients.end())
		return;

	std::string &resp		= it->second->response_data;
	size_t		&bytes_sent = it->second->bytes_sent;

	size_t	len_to_send = resp.length() - bytes_sent;
	ssize_t sent		= send(client_fd, resp.c_str() + bytes_sent, len_to_send, 0);

	if (sent < 0)
	{
		printBoxError("Send error on client " + intToString(client_fd) + ": " + std::string(strerror(errno)));
		removeClient(client_fd);
		return;
	}

	bytes_sent += sent;

	if (bytes_sent == resp.length())
	{
		_rspCount++;
		printRaw(it->second->response.getResponse()); // DBG
		removeClient(client_fd);
	}
}

void ServerManager::removeClient(int client_fd)
{
	close(client_fd);

	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}

	for (size_t i = 0; i < _polls.size(); ++i)
	{
		if (_polls[i].fd == client_fd)
		{
			_polls.erase(_polls.begin() + i);
			break;
		}
	}
	printBoxMsg("Client " + intToString(client_fd) + " removed.");
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

void ServerManager::servQuit()
{
	_running = false;
	printBoxMsg("Server shutting down...");
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
std::vector<ServerData> ServerManager::getServersList() const
{
	return _serverData;
}

Script &ServerManager::getScript()
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

std::string ServerManager::createSession(const std::string &username)
{
	std::string sessionId = generateCookieId();
	CookieData	newSession;
	newSession.username		   = username;
	newSession.isAuthenticated = true;
	newSession.lastAccessTime  = std::time(NULL);
	this->_sessions[sessionId] = newSession;
	return sessionId;
}

CookieData *ServerManager::getSession(const std::string &sessionId)
{
	if (this->_sessions.find(sessionId) == this->_sessions.end())
		return NULL;

	const int	SESSION_TIMEOUT = 3600;
	time_t		now				= std::time(NULL);
	CookieData &session			= this->_sessions[sessionId];

	if (now - session.lastAccessTime > SESSION_TIMEOUT)
	{
		this->_sessions.erase(sessionId);
		return NULL;
	}
	session.lastAccessTime = now;
	return &session;
}