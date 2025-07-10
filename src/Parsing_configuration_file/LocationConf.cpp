/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/07/10 18:48:27 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Utils.hpp"
#include "../../inc/LocationConf.hpp"
#include "../../inc/ServerConf.hpp"

LocationConf::LocationConf() {}

LocationConf::LocationConf(ServerConf const& S)
{
	_autoindex = S.getAutoindex();
	_root = S.getRoot();
	_index = S.getIndex();
	_bodySize = S.getBodySize();
	_returnDirective = S.getReturnDirective();
	_errorPage = S.getErrorPage();
	_allowedMethods = S.getAllowedMethods();
	_cgiPass[".py"] = "/usr/bin/python3";
	_cgiPass[".php"] = "/usr/bin/php-cgi";
}
  
LocationConf::~LocationConf() {} 
//detruire des trucs ici?

LocationConf::LocationConf(LocationConf const& other) : ABlockBase (other)
{
	*this = other;
}

LocationConf&	LocationConf::operator=(LocationConf const& other)
{
	if (this != &other)
	{
		ABlockBase::operator=(other);
		_cgiPass = other._cgiPass;
	}
	return *this;
}

bool    LocationConf::getAutoindex() const
{
	return _autoindex;
}

std::string const&	LocationConf::getKey() const
{
	return _key;	
}

void	LocationConf::setKey(std::string const value)
{
	_key = value;
	return;
}
std::map<std::string, std::string> const&	LocationConf::getCgiPass() const
{
	return _cgiPass;
}

size_t	LocationConf::fillCgiPass(std::vector<std::string>& buffer, size_t i)
{
	if (!checkFlag("cgiPass"))
		_cgiPass.clear();
	if (getKey() != "/cgi-bin/")
		throw std::invalid_argument(" Parsing error, 'cgi_pass' arguments"
		" are allowed only in '/cgi-bin/' location's block\n");
	if (i + 1 >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}" || buffer[i + 1].empty()
		|| buffer[i + 1] == "{" || buffer[i + 1] == "}" || buffer[i] == ";")
		throw std::invalid_argument(" Parsing error, miss 'cgi_pass' arguments\n");
	if ((buffer[i] != ".py" && buffer[i] != ".php") || (buffer[i + 1] != "/usr/bin/python3"
		&& buffer[i + 1] != "/usr/bin/php-cgi"))
		throw std::invalid_argument(" Parsing error, 'cgi_pass' argument invalid\n");
	if (_cgiPass.find(buffer[i]) != _cgiPass.end())
		throw std::invalid_argument(" Parsing error, 'cgi_pass' argument duplicated\n");
	_cgiPass[buffer[i]] = buffer[i + 1];
	if ((buffer[i] == ".py" && buffer[i + 1] != "/usr/bin/python3")
		|| (buffer[i] == ".php" && buffer[i + 1] != "/usr/bin/php-cgi"))
		throw std::invalid_argument(" Parsing error, 'cgi_pass' arguments don't match\n");
	if (i + 2 >= buffer.size() || buffer[i + 2] != ";")
		throw std::invalid_argument(" Parsing error, miss semicolon after 'cgi_pass' argument\n");
	return i + 3;
}
