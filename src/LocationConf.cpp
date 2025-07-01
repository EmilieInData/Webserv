/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/07/01 19:07:00 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "LocationConf.hpp"
#include "ServerConf.hpp"

LocationConf::LocationConf() {}

LocationConf::LocationConf(ServerConf const& S)
{
    _autoindex = S.getAutoindex();
	_root = S.getRoot();
	_index = S.getIndex();
    _bodySize = S.getBodySize();
    _returnDirective = S.getReturnDirective();
	_errorPage = S.getErrorPage();
    _cgiPass = "";
    _allowedMethods.push_back("GET");
    _allowedMethods.push_back("POST");
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
        _autoindex = other._autoindex;
        _root = other._root;
        _index = other._index;
        _bodySize = other._bodySize;
        _returnDirective = other._returnDirective;
    	_errorPage = other._errorPage;
    	_cgiPass = other._cgiPass;
        _allowedMethods = other._allowedMethods;
    }
    return *this;
}

bool	LocationConf::checkFlag(std::string const& value)
{
	for(size_t i = 0; i < _flag.size(); i++)
	{
		if (_flag[i] == value)
			return true;
	}
	_flag.push_back(value); //mettre le flag de la directive
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
			" directive allowed by location block\n"); 
	if (buffer[i] == "on")
		_autoindex = true;
	else
		_autoindex = false;
	return (i + 2);
}

size_t	LocationConf::fillRoot(std::vector<std::string>& buffer, size_t i)
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
			" directive allowed by location block\n"); 
	_root = buffer[i];
	return (i + 2);
}

size_t	LocationConf::fillIndex(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("index"))
		throw std::invalid_argument(" Parsing error, only one 'index'"
			" directive allowed by location blocks\n"); 
	if (i >= buffer.size() || buffer[i].empty() || i + 1 >= buffer.size() || buffer[i + 1] == ";")
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
	for (size_t i = 0; i < _index.size(); i++)
		std::cout << PURPLE << _index[i] << std::endl;
    return (i + 1);   
}
