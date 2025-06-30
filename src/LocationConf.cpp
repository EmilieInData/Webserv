/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/06/30 19:05:35 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "LocationConf.hpp"
#include "ServerConf.hpp"

LocationConf::LocationConf() {}

LocationConf::LocationConf(ServerConf const& S)
{
    autoindex = S.getAutoindex();
	root = S.getRoot();
	index = S.getIndex();
    bodySize = S.getBodySize();
    returnDirective = S.getReturnDirective();
	errorPage = S.getErrorPage();
    cgiPass = "";
    allowedMethods.push_back("GET");
    allowedMethods.push_back("POST");
}
  
LocationConf::~LocationConf() {} 
//detruire des trucs ici?

LocationConf::LocationConf(LocationConf const& other)
{
    *this = other;
}
LocationConf&	LocationConf::operator=(LocationConf const& other)
{
    if (this != &other)
    {
        autoindex = other.autoindex;
        root = other.root;
        index = other.index;
        bodySize = other.bodySize;
        returnDirective = other.returnDirective;
        errorPage = other.errorPage;
        cgiPass = other.cgiPass;
        allowedMethods = other.allowedMethods;
    }
    return *this;
}

bool	LocationConf::checkFlag(std::string const& value)
{
	for(size_t i = 0; i < flag.size(); i++)
	{
		if (flag[i] == value)
			return true;
	}
	flag.push_back(value); //mettre le flag de la directive
	return false;
}

size_t	LocationConf::fillAutoIndex(std::vector<std::string>& buffer, size_t i)
{
	if ( i >= buffer.size() || buffer[i].empty())
		throw std::invalid_argument(" Parsing error, miss 'autoindex' argument\n");
	if (buffer[i] != "on" && buffer[i] != "off")
		throw std::invalid_argument(" Parsing error, 'autoindex' allow only"
			" 'on' or 'off' arguments\n");
	if (buffer[i + 1].empty() || buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error, 'autoindex' allow only"
			" one argument\n");
	if (checkFlag("autoindex"))
		throw std::invalid_argument(" Parsing error, only one 'autoindex'"
			" directive allowed by server block\n"); 
	if (buffer[i] == "on")
		autoindex = true;
	else
		autoindex = false;
	return (i + 2);
}
