/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/06/25 19:37:19 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ServerConf.hpp"

ServerConf::ServerConf()
{
	autoindex = false;
	root = "/var/www/html";
	index = "index.html";
	bodySize = 1048576;
	returnDirective = ""; //I don't KNOW ??
	errorPage[404] = "/404.html";
	serverName[0] = "default";
	listens.push_back(listen{80, "0.0.0.0"});
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
		this->autoindex = other.autoindex;
		this->root = other.root;
		this->index = other.index;
		this->bodySize = other.bodySize;
		this->returnDirective = other.returnDirective;
		this->errorPage = other.errorPage;
		this->listens = other.listens;
		this->serverName = other.serverName;
		this->locations = other.locations;     
	}
	return *this;
}


bool    ServerConf::getAutoindex() const
{
	return autoindex;
}

std::string ServerConf::getRoot() const
{
	return root;
}
std::string ServerConf::getIndex() const
{
	return index;
}
		
unsigned int ServerConf::getBodySize() const
{
	return bodySize;
}
		
std::string ServerConf::getReturnDirective() const
{
	return returnDirective;
}

std::map<unsigned int, std::string> ServerConf::getErrorPage() const
{
	return errorPage;
}

bool	ServerConf::checkFlag(std::string const& value)
{
	for(size_t i = 0; i < flag.size(); i++)
	{
		if (flag[i] == value)
			return true;
	}
	flag.push_back(value); //mettre le flag de la directive
	return false;
}
	
size_t ServerConf::fillListens(std::vector<std::string>& buffer, size_t i)
{
	int port = 0;
	std::string ip = "";
	
	if (buffer[i].empty() || buffer[i + 1].empty() || buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error with 'listen' arguments\n"); 
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
		for (size_t i = 0; i < listens.size(); i++)
		{
			if (listens[i].ip == ip && listens[i].port == port)
				throw std::invalid_argument(" Parsing error, duplicate 'listen' arguments\n");
		}	
		listens.push_back(listen{port, ip});
	}
	else
		listens[0] = listen{port, ip};
	//checker si le port et l'adresse ip sont libres ici ???
	return (i + 2);   
}

size_t	ServerConf::fillServerName(std::vector<std::string>& buffer, size_t i)
{
	
    return;   
}

Pas de caractères interdits (caractères spéciaux hors tirets et points).

www.example.com

www est un label, example est un autre label, com est un autre label

Composé de lettres [a-zA-Z], chiffres [0-9], et tirets -
Longueur totale du nom ≤ 255 caractères.

Chaque label doit respecter les règles DNS, c’est-à-dire :
Ne contenir que des lettres, chiffres et tirets
Ne pas commencer ni finir par un tiret
Avoir une longueur entre 1 et 63 caractères