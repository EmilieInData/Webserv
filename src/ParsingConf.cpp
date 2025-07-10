/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConf.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:21:07 by esellier          #+#    #+#             */
/*   Updated: 2025/07/10 17:23:40 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ParsingConf.hpp"
#include "ServerConf.hpp"
#include "LocationConf.hpp"

ParsingConf::ParsingConf() {}

ParsingConf::~ParsingConf() {}

void	ParsingConf::print_tokens(std::vector<std::string>& buffer)
{
	for (size_t i = 0; i < buffer.size(); i++)
		std::cout << BLUE << buffer[i] << std::endl;
	return;
}

void	ParsingConf::checkLocation(std::string& line)
{
	size_t		i = 0;
	size_t			j;
	std::string first;
	
	while(i < line.length())
	{
		if (line[i] != 9 && line[i] != 32)
		{
			j = i;
			while (i < line.length() && line[i] != 9 && line[i] != 32)
				i++;
			first = line.substr(j, i - j);
			break;
		}
		i++;
	}
	if(first == "location")
	{
		while (i < line.length())
		{
			if (line[i] != 9 && line[i] != 32)
			{
				j = i;
				while (i < line.length() && line[i] != 9 && line[i] != 32)
					i++;
				first = line.substr(j, i - j);
				if  (first == "{" || first == "}" || first == ";" )
					throw std::invalid_argument("Parsing error, miss an argument"
						" after 'location'\n");
				if (first[0] != '/')
					throw std::invalid_argument("Parsing error, wrong location's"
						" direction\n");
				return;
			}
			i++;
		}
		throw std::invalid_argument("Parsing error, miss an argument after 'location'\n");
	}
	return;
}

void	ParsingConf::checkSemicolon(std::string& line)
{
	size_t j = line.find(';');
	if(j == std::string::npos)
		return;
	for (size_t i = j + 1; i < line.length(); i++)  //si il y a autre chose derriere
	{
		if (line[i] != 9 && line[i] != 32 && line[i] != '}')
			throw std::invalid_argument("Parsing error, no argument accepted"
				" after the semicolon\n");
		if (line[i] == '}') //parenthese de fin de ligne acceptee uniquement 
		{
			i++;
			while (line[i])
			{
				if (line[i] != 9 && line[i] != 32)
					throw std::invalid_argument("Parsing error, no argument"
						" accepted after the brace\n");
				i++;
			}
			break;
		}
	}
	size_t i = 0;
	while (line[i] && line[i] < 33)
		i++;
	if (line[i] == ';') //pas de semicolon en premier token
		throw std::invalid_argument("Parsing error, need argument before the semicolon\n");
	line.insert(j, " ");//mettre un espace avant le ";" pour le garder ds un autre token
	return;
}

void	ParsingConf::checkParenthesis(std::string& line)
{
	size_t i = line.find('{');
	size_t j = line.find('}');
	
	if(i == std::string::npos && j == std::string::npos)
		return ;
	for (size_t i = 0; i < line.length(); i++)
	{
		if ((line[i] == '{' || line[i] == '}') && (i + 1 < line.length()
			&& (line[i + 1] == '{' || line[i + 1] == '}')))
			throw std::invalid_argument("Parsing error, parenthesis cannot be"
			 	" followed by another parenthesis\n");
	}
}

void	ParsingConf::doParsing(std::string line, std::vector<std::string>& buffer)
{
	for (size_t i = 0; i < line.length(); i++)
	{
		if ((line[i] < 32 && line[i] != 9 && line[i] != 10) || line[i] == 127) //je garde les espaces et tab pour couper les mots
			throw std::invalid_argument("Parsing error, invalid no printable character\n");
		if (line[i] == '#')
		{
			line = line.substr(0, i); //enlever les commentaires
			break;
		}
	}
	checkParenthesis(line);
	checkLocation(line);
	checkSemicolon(line);
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != 9 && line[i] != 32) // enlever les espaces
		{
			size_t j = i;
			while (i < line.length() && line[i] != 9 && line[i] != 32)
				i++;
			buffer.push_back (line.substr(j, i - j)); //transformer en token
		}
	}
	for (size_t i = 0; i + 1 < buffer.size(); i++)
	{
		if (buffer[i] == "{" && !buffer[i + 1].empty() && buffer[i + 1] == "}")
			throw std::invalid_argument("Parsing error, server & locations's"
				" blocks cannot be empty\n");
	}
}

void	ParsingConf::checkStructure(std::vector<std::string>& buffer)
{
	std::vector<std::string> stack;

	for (size_t i = 0; i < buffer.size(); i++)
	{
		if (buffer[i] == "server")
		{
			if (i + 1 >= buffer.size() || buffer[i + 1] != "{")
				throw std::invalid_argument(" Parsing error, miss '{'"
					" to opening server's block\n");
			stack.push_back(buffer[i]);
		}
		else if (buffer[i] == "location")
		{
			if (stack.empty() || stack.back() != "server")
				throw std::invalid_argument(" Parsing error, 'location' blocks "
					"allowed only in a server block\n");
			if (i + 2 >= buffer.size() || buffer[i + 2] != "{")
				throw std::invalid_argument(" Parsing error, miss '{' to"
					" opening location's block\n");
			stack.push_back(buffer[i]);
		}
		else if (buffer[i] == "{")
		{
			if ((i > 0 && buffer[i - 1] != "server")
				&& (i > 1 && buffer[i - 2] != "location"))
				throw std::invalid_argument(" Parsing error, wrong block's"
					" name before parenthesis\n");
		}
		else if (buffer[i] == "}")
		{
			if (stack.empty())
			{
				throw std::invalid_argument(" Parsing error, trying to close"
					" a block not open\n");
			}
			stack.pop_back();
		}
	}
	if (!stack.empty())
		throw std::invalid_argument(" Parsing error, a block is not closed\n");
	return;
}

void	ParsingConf::fillStructs(std::vector<std::string>& buffer)
{
	size_t											i = 0;
	std::vector<ABlockBase*>						blocks;
	std::vector<ServerConf>::iterator 				itServer;
	std::map<std::string, LocationConf>::iterator	itLocation;
	
	if (buffer.empty() || buffer[0] != "server")
		throw std::invalid_argument(" Parsing error, configuration files need to begin by a server\n");
	checkStructure(buffer); //parenthesis well closed, blocks well positionned
	while(i < buffer.size())
	{
		std::cout << GREEN << " --> " << buffer[i] << RESET << std::endl; // to borrow
		if (buffer[i] == "server")
		{
			if ( i + 1 >= buffer.size() || buffer[i + 1] != "{")
				throw std::invalid_argument(" Parsing error, miss '{' after 'server' to opening the block\n");
			servers.push_back(ServerConf());//creer les blocs server dans le contener
			itServer = servers.end() - 1;
			blocks.push_back(&(*itServer));
			i = i + 2;
			continue;
		}
		else if (buffer[i] == "location")
		{
			if (itServer->getLocations().find(buffer[i + 1]) != itServer->getLocations().end())
				throw std::invalid_argument(" Parsing error, this 'location'"
					" directive already exist\n");
			itServer->getLocations()[buffer[i + 1]] = LocationConf(*itServer);
			itLocation = itServer->getItLocations(buffer[i + 1]);
			itLocation->second.setKey(buffer[i + 1]);
			blocks.push_back(&itLocation->second);
			i = i + 3;
			continue;
		}
		else if (blocks.empty())
			throw std::invalid_argument(" Parsing error, directives allowed only in a block\n");	
		else if (buffer[i] == "listen") // NO IN LOCATION
		{
			if (!blocks.empty() && blocks.back() != &(*itServer))
				throw std::invalid_argument(" Parsing error, 'listen' directive"
					" allowed only in server block\n");	 
			i = itServer->fillListens(buffer, i + 1);
			continue;
		}
		else if (buffer[i] == "server_name") //NO IN LOCATION
		{
			if (!blocks.empty() && blocks.back() != &(*itServer))
				throw std::invalid_argument(" Parsing error, 'server_name' directive"
					" allowed only in server block\n");	 
			i = itServer->fillServerName(buffer, i + 1);
			continue;
		}	
		else if (buffer[i] == "autoindex")
			i = blocks.back()->fillAutoIndex(buffer, i + 1);
		else if (buffer[i] == "root")
			i = blocks.back()->fillRoot(buffer, i + 1);
		else if (buffer[i] == "index")
			i = blocks.back()->fillIndex(buffer, i + 1);
		else if (buffer[i] == "client_max_body_size")
			i = blocks.back()->fillBodySize(buffer, i + 1);
		else if (buffer[i] == "return")
				i = blocks.back()->fillReturnDirectives(buffer, i + 1);
		else if (buffer[i] == "error_page")
			i = blocks.back()->fillErrorPage(buffer, i + 1);
		else if (buffer[i] == "allow_methods")
			i = blocks.back()->fillAllowedMethods(buffer, i + 1);
		else if (buffer[i] == "cgi_pass")
		{
			if (!blocks.empty() && blocks.back() != &(itLocation->second))
				throw std::invalid_argument(" Parsing error, 'cgi_pass' directive"
					"  allowed only in a location block\n");
			i = itLocation->second.fillCgiPass(buffer, i + 1);
			continue;
		}
		else if (buffer[i] == "}")
		{
			blocks.pop_back(); //supprimer le block ds le vector pour le 'fermer'
			i++;
		}
		else
			throw std::invalid_argument(" Parsing error, invalid directives\n");
	}
	return;
}
