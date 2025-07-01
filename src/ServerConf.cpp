/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/07/01 19:50:33 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ServerConf.hpp"

ServerConf::ServerConf()
{
	_autoindex = false;
	_root = "/var/www/html";
	_index.push_back("index.html");
	_bodySize = 1048576;
	_returnDirective = ""; //I don't KNOW ??
	_errorPage[404] = "/404.html";
	_serverName.push_back("default");
	_listens.push_back(listen{80, "0.0.0.0"});
}
  
ServerConf::~ServerConf() {} 
//detruire des trucs ici?

ServerConf::ServerConf(ServerConf const& other)
{
	*this = other;
}
ServerConf&	ServerConf::operator=(ServerConf const& other)
{
	if (this != &other)
	{
		this->_autoindex = other._autoindex;
		this->_root = other._root;
		this->_index = other._index;
		this->_bodySize = other._bodySize;
		this->_returnDirective = other._returnDirective;
		this->_errorPage = other._errorPage;
		this->_listens = other._listens;
		this->_serverName = other._serverName;
		this->_locations = other._locations;     
	}
	return *this;
}


bool    ServerConf::getAutoindex() const
{
	return _autoindex;
}

std::string ServerConf::getRoot() const
{
	return _root;
}
std::vector<std::string> ServerConf::getIndex() const
{
	return _index;
}
		
unsigned int ServerConf::getBodySize() const
{
	return _bodySize;
}
		
std::string ServerConf::getReturnDirective() const
{
	return _returnDirective;
}

std::map<unsigned int, std::string> ServerConf::getErrorPage() const
{
	return _errorPage;
}

std::map<std::string, LocationConf>&	ServerConf::getLocations()
{
	return _locations;
}

std::map<std::string, LocationConf>::iterator	ServerConf::getItLocations(std::string const& key)
{
	return _locations.find(key);
}	

bool	ServerConf::checkFlag(std::string const& value)
{
	for(size_t i = 0; i < _flag.size(); i++)
	{
		if (_flag[i] == value)
			return true;
	}
	_flag.push_back(value); //mettre le flag de la directive
	return false;
}

size_t ServerConf::fillListens(std::vector<std::string>& buffer, size_t i)
{
	int port = 0;
	std::string ip = "";
	
	if (i >= buffer.size() || buffer[i].empty() || i + 1 >= buffer.size()
		|| buffer[i + 1].empty() || buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error with 'listen' argument,"
		" need one argument followed by a semicolon\n"); 
	if (isInt(buffer[i])) //is an int
	{
		//std::cout << PURPLE << "enter to int\n";
		port = strToInt(buffer[i]);
		if (port < 0 || port > 65535)
			throw std::invalid_argument(" Parsing error, 'listen' port number is not correct, only int between 0 & 65535 accepted\n"); 
		ip = "0.0.0.0";
	}
	else if (isIp(buffer[i])) //is an ip address
	{
		//std::cout << PURPLE << "enter to ip address\n";
		if (!checkIpAddress(buffer[i]))
			throw std::invalid_argument(" Parsing error, 'listen' ip address is not correct\n"); 
		ip = buffer[i];
		port = 80;
	}
	else if (isSocket(buffer[i])) // is a socket address
	{
		//std::cout << PURPLE << "enter to socket address\n";
		if (!checkSocketAddress(buffer[i]))
			throw std::invalid_argument(" Parsing error, 'listen' socket address is not correct\n"); 
		ip = socketToIp(buffer[i]);
		port = socketToPort(buffer[i]);
	}
	else
		throw std::invalid_argument(" Parsing error, 'listen' argument is not correct, wrong port number, ip or socket address\n"); 
	//std::cout << BLUE << "ip: " << ip << " port = " << port << std::endl;
	if (checkFlag("listen"))
	{
		for (size_t i = 0; i < _listens.size(); i++)
		{
			if (_listens[i].ip == ip && _listens[i].port == port)
				throw std::invalid_argument(" Parsing error, duplicate 'listen' arguments\n");
		}	
		_listens.push_back(listen{port, ip});
	}
	else
		_listens[0] = listen{port, ip};
	//checker si le port et l'adresse ip sont libres ici ???
	return (i + 2);   
}

size_t	ServerConf::fillServerName(std::vector<std::string>& buffer, size_t i)
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
		_serverName.push_back(buffer[i]);
		i++;
	}
	// for (size_t i = 0; i < _serverName.size(); i++)
	// 	std::cout << PURPLE << _serverName[i] << std::endl;
	if (!checkDns(_serverName))
		throw std::invalid_argument(" Parsing error, 'server_name' arguments"
			" need to follow DNS's rules\n");
    return (i + 1);   
}

size_t	ServerConf::fillAutoIndex(std::vector<std::string>& buffer, size_t i)
{
	if ( i >= buffer.size() || buffer[i].empty())
		throw std::invalid_argument(" Parsing error, miss 'autoindex' argument\n");
	if (buffer[i] != "on" && buffer[i] != "off")
		throw std::invalid_argument(" Parsing error, 'autoindex' allow only"
			" 'on' or 'off' arguments\n");
	if (i + 1 >= buffer.size())
		throw std::invalid_argument(" Parsing error, miss semicolon after"
			" 'autoindex' argument\n");
	if (i + 1 < buffer.size() && buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error, 'autoindex' allow only"
			" one argument\n");
	if (checkFlag("autoindex"))
		throw std::invalid_argument(" Parsing error, only one 'autoindex'"
			" directive allowed by server block\n"); 
	if (buffer[i] == "on")
		_autoindex = true;
	else
		_autoindex = false;
	return (i + 2);
}

size_t	ServerConf::fillRoot(std::vector<std::string>& buffer, size_t i)
{
	if ( i >= buffer.size() || buffer[i].empty()) //a voir si utile pour chaque directive ?
		throw std::invalid_argument(" Parsing error, miss 'root' argument\n");
	if (buffer[i][0] != '/')
		throw std::invalid_argument(" Parsing error, 'root' expects an absolute path\n");
	if (i + 1 >= buffer.size())
		throw std::invalid_argument(" Parsing error, miss semicolon after"
			" 'root' argument\n");
	if (i + 1 < buffer.size() && buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error, 'root' allows only"
			" one argument\n");
	if (checkFlag("root"))
		throw std::invalid_argument(" Parsing error, only one 'root'"
			" directive allowed by server block\n"); 
	_root = buffer[i];
	return (i + 2);
}
size_t	ServerConf::fillIndex(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("index"))
		throw std::invalid_argument(" Parsing error, only one 'index'"
			" directive allowed by server blocks\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";")
		throw std::invalid_argument(" Parsing error, miss 'index' arguments\n");
	if (!_index.empty())
		_index.clear(); //supprimer l'arg par defaut
	while (i < buffer.size())
	{
		if (buffer[i] == ";")
			break;
		if (buffer[i] == "{" || buffer[i] == "}")
			throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'index' directive\n");
		_index.push_back(buffer[i]);
		i++;
	}
	// for (size_t i = 0; i < _index.size(); i++)
	// 	std::cout << PURPLE << _index[i] << std::endl;
    return (i + 1);   
}
