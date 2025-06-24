/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/06/24 21:03:34 by esellier         ###   ########.fr       */
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
    serverName = "default";
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
        
void ServerConf::fillListens(std::vector<std::string>::iterator it)
{
    (void)it;
    std::cout <<"Test\n";
    return;   
}

// void ServerConf::fillServerName(std::string& value)
// {
    
//     return;   
// }

