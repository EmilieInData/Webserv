/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/07/23 16:56:52 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ServerData.hpp"

ServerData::ServerData()
{
	_listens.push_back(std::make_pair(8080, "127.0.0.1")); //accepter localhost
	// _serverName.push_back("default");
}
  
ServerData::~ServerData() {} 
//detruire des trucs ici?

ServerData::ServerData(ServerData const& other) : ABlockBase(other)
{
	*this = other;
}

ServerData&	ServerData::operator=(ServerData const& other)
{
	if (this != &other)
	{
  		ABlockBase::operator=(other);
		this->_listens = other._listens;
		this->_serverName = other._serverName;
		this->_locations = other._locations;     
	}
	return *this;
}

bool    ServerData::getAutoindex() const
{
	return _autoindex;
}

std::map<std::string, LocationConf>&	ServerData::getLocations()
{
	return _locations;
}

std::map<std::string, LocationConf>::iterator	ServerData::getItLocations(std::string const& key)
{
	return _locations.find(key);
}

std::vector<std::pair<int, std::string> >	ServerData::getListens()
{
	return _listens;
}

std::vector<std::string>	ServerData::getServerName() const
{
	return _serverName;
}

size_t ServerData::fillListens(std::vector<std::string>& buffer, size_t i)
{
	int port = 0;
	std::string ip = "";
	
	if (i >= buffer.size() || buffer[i].empty() || i + 1 >= buffer.size()
		|| buffer[i + 1].empty() || buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error with 'listen' argument,"
		" need one argument followed by a semicolon\n"); 
	if (isInt(buffer[i])) //is an int
	{
		port = strToInt(buffer[i]);
		ip = "127.0.0.1";
	}
	else if (isLocal(buffer[i])) //is an local address
	{
		port = strToInt(buffer[i].substr(10));;
		ip = "127.0.0.1";
	}
	else if (isSocket(buffer[i])) // is a socket address
	{
		if (!checkSocketAddress(buffer[i]))
			throw std::invalid_argument(" Parsing error, 'listen' socket address is not correct\n"); 
		ip = socketToIp(buffer[i]);
		port = socketToPort(buffer[i]);
	}
	else
		throw std::invalid_argument(" Parsing error, 'listen' argument is not correct\n"); 
	// std::cout << BLUE << "ip: " << ip << " port = " << port << std::endl;
	if (checkFlag("listen"))
	{
		for (size_t i = 0; i < _listens.size(); i++)
		{
			if (_listens[i].first == port && _listens[i].second == ip)
				throw std::invalid_argument(" Parsing error, duplicate 'listen' arguments\n");
		}	
		_listens.push_back(std::make_pair(port, ip));
	}
	else
		_listens[0] = std::make_pair(port, ip);
	return (i + 2);   
}

size_t	ServerData::fillServerName(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("server_name"))
		throw std::invalid_argument(" Parsing error, only one 'server_name'"
			" directive allowed by server block\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";")
		throw std::invalid_argument(" Parsing error, miss 'server_name' arguments\n");
	if (!_serverName.empty())
		_serverName.clear(); //supprimer l'arg par defaut (vide tout le vector)
	while (i < buffer.size())
	{
		if (buffer[i] == ";")
			break;
		if (buffer[i] == "{" || buffer[i] == "}")
			throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'server_name' directive\n");
		for (size_t j = 0; j < _serverName.size(); j++)
		{
			if (buffer[i] == _serverName[j])
				throw std::invalid_argument(" Parsing error, 'server_name' argument duplicated\n");
		}
		_serverName.push_back(buffer[i]);
		i++;
	}
	if (!checkDns(_serverName))
		throw std::invalid_argument(" Parsing error, 'server_name' arguments"
			" need to follow DNS's rules\n");
    return (i + 1);   
}

void ServerData::servListen(std::pair<int, std::string> _listens)
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
}

void ServerData::serverInit()
{
	for (size_t i = 0; i < _listens.size(); i++)
	servListen(_listens[i]);
}

void ServerData::servStart()
{	
	std::cout << timeStamp() << __PRETTY_FUNCTION__ << std::endl;
	
	struct pollfd *polls = new pollfd[_socketFd.size()];
	
	for(size_t i = 0; i < _socketFd.size(); i++)
	{
		polls[i].fd = _socketFd[i];
		polls[i].events = POLLIN;
	}
	
	std::cout << timeStamp() << "Waiting for connection on port 8080" << std::endl;
	
	while (true)
	{
		int check = poll(polls, 1, 5000);
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
		
		for (size_t i = 0; i < _socketFd.size(); i++)
		{
			if (polls[i].revents & POLLIN)
			{
				int					clientFd;
				struct sockaddr_in	clientAddr;
				socklen_t			clientLen = sizeof(clientAddr);
				clientFd = accept(_socketFd[i], (struct sockaddr *)&clientAddr, &clientLen);
				if (clientFd >= 0)
				{
					std::cout << timeStamp() << "New connection accepted" << std::endl;
					char buffer[4096];
					ssize_t bytes = recv(clientFd, buffer, sizeof(buffer), 0);
					if (bytes > 0)
					buffer[bytes] = '\0';
					std::cout << timeStamp() << "Request content:\n*****\n" << std::endl;
					std::cout << buffer;
					std::cout << "*****" << std::endl;
					
					// HttpRequest	req = HttpRequest( buffer, *this );
					
					std::string response = 
					"HTTP/1.1 200 OK\n"
					"Content-Type: text/html\n"
					"Content-Length: 85\n"
					"\n"
					"<html><body><h1>Bonjour!</h1></body></html>";
					
					send(clientFd, response.c_str(), response.size(), 0);
					
					close(clientFd);
				}
			}
		}
		
		for (size_t i = 0; i < _socketFd.size(); i++)
		close(_socketFd[i]);
		delete[] polls;
	}
}