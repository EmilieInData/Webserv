/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockBase.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/07/01 19:50:33 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "BlockBase.hpp"

BlockBase::BlockBase()
{
	_autoindex = false;
	_root = "/var/www/html";
	_index.push_back("index.html");
	_bodySize = 1048576;
	// _returnDirective = ""; //To define ??
	_errorPage[404] = "/404.html"; //to fill
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
}
  
BlockBase::~BlockBase() {} 
//detruire des trucs ici?

BlockBase::BlockBase(BlockBase const& other)
{
	*this = other;
}
BlockBase&	BlockBase::operator=(BlockBase const& other)
{
	if (this != &other)
	{
		this->_autoindex = other._autoindex;
		this->_root = other._root;
		this->_index = other._index;
		this->_bodySize = other._bodySize;
		this->_returnDirective = other._returnDirective;
		this->_errorPage = other._errorPage;
		this->_allowedMethods = other._allowedMethods;
	}
	return *this;
}

bool    BlockBase::getAutoindex() const
{
	return _autoindex;
}

std::string BlockBase::getRoot() const
{
	return _root;
}
std::vector<std::string> BlockBase::getIndex() const
{
	return _index;
}
		
unsigned int BlockBase::getBodySize() const
{
	return _bodySize;
}
		
std::vector<std::string> BlockBase::getReturnDirective() const
{
	return _returnDirective;
}

std::map<unsigned int, std::string> BlockBase::getErrorPage() const
{
	return _errorPage;
}

std::vector<std::string>	BlockBase::getAllowedMethods() const
{
	return _allowedMethods;
}

bool	BlockBase::checkFlag(std::string const& value)
{
	for(size_t i = 0; i < _flag.size(); i++)
	{
		if (_flag[i] == value)
			return true;
	}
	_flag.push_back(value); //mettre le flag de la directive
	return false;
}

size_t	BlockBase::fillAutoIndex(std::vector<std::string>& buffer, size_t i)
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

size_t	BlockBase::fillRoot(std::vector<std::string>& buffer, size_t i)
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

size_t	BlockBase::fillIndex(std::vector<std::string>& buffer, size_t i)
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
    return (i + 1);
}

size_t	BlockBase::fillBodySize(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("bodySize"))
		throw std::invalid_argument(" Parsing error, only one 'client_max_body_size'"
			" directive allowed by server blocks\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}")
		throw std::invalid_argument(" Parsing error, miss 'client_max_body_size' arguments\n");
	if (i + 1 >= buffer.size() || buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'client_max_body_size' directive\n");
	_bodySize = strToSize(buffer[i]);
	if (_bodySize > 1048576)
		throw std::invalid_argument(" Parsing error, 'client_max_body_size'"
			" max allowed is 1 048 576 bits\n");
	return i + 2;
}

size_t	BlockBase::fillAllowedMethods(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("allowMethods"))
		throw std::invalid_argument(" Parsing error, only one 'allow_methods'"
			" directive allowed by server blocks\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}")
		throw std::invalid_argument(" Parsing error, miss 'allow_methods' arguments\n");
	_allowedMethods.clear(); // vider l'arg/les args car redefini
	for (; i < buffer.size(); i++)
	{
		if (buffer[i] == "GET" || buffer[i] == "POST" || buffer[i] == "DELETE")
		{
			for(size_t j = 0; j < _allowedMethods.size(); j++) //checker doublons mm si ok nginx
			{
				if (_allowedMethods[j] == buffer[i])
					throw std::invalid_argument(" Parsing error, 'allow_methods' duplicated\n");
			}
			_allowedMethods.push_back(buffer[i]);
		}
		else if (buffer[i] == ";")
			break;
		else
			throw std::invalid_argument(" Parsing error 501, not implemented\n"
				"'allow_methods' argument is not correct\n");
	}
	return i + 1;
}

size_t	BlockBase::fillReturnDirectives(std::vector<std::string>& buffer, size_t i)
{
	int	num;

	if (checkFlag("return"))
		throw std::invalid_argument(" Parsing error, only one 'return'"
			" directive allowed by server blocks\n"); 
	if (i + 1 >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}" || buffer[i + 1].empty()
		|| buffer[i + 1] == "{" || buffer[i + 1] == "}")
		throw std::invalid_argument(" Parsing error, miss 'return' directive arguments\n");
	_returnDirective.clear(); // vider l'arg/les args car redefini
	if (isInt(buffer[i]))
		num = strToInt(buffer[i]);
	if (!isInt(buffer[i]) || num < 100 || num > 599)
			throw std::invalid_argument(" Parsing error, 'return' directive"
				" argument is not correct\n");
	_returnDirective.push_back(buffer[i]);
	if (buffer[i + 1] != ";")
	{
		_returnDirective.push_back(buffer[i + 1]);
		if (i + 2 >= buffer.size() || buffer[i + 2].empty() || buffer[i + 2] != ";")
			throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'return' directive arguments\n");
		return i + 3;
	}
	return i + 2;
}
