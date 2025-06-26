/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConf.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:21:07 by esellier          #+#    #+#             */
/*   Updated: 2025/06/26 18:32:45 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ParsingConf.hpp"
#include "ServerConf.hpp"

ParsingConf::ParsingConf() {}

ParsingConf::~ParsingConf() {}

void	ParsingConf::print_tokens(std::vector<std::string>& buffer)
{
	for (size_t i = 0; i < buffer.size(); i++)
		std::cout << BLUE << buffer[i] << std::endl;
	return;
}

bool	ParsingConf::cleaning(char c)
{
	return ((c < 32 && c != 9) || c == 127); //je garde les espaces et tab pour couper les mots
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
					throw std::invalid_argument("Parsing error, miss an argument after 'location'\n");
				if (first[0] != '/')
					throw std::invalid_argument("Parsing error, wrong location's direction\n");
				return;
			}
			i++;
		}
		throw std::invalid_argument("Parsing error, miss an argument after 'location'\n");
	}
	return;
}

std::string&	ParsingConf::checkSemicolon(std::string& line)
{
	size_t j = line.find(';');
	if(j == std::string::npos)
		return line;
	for (size_t i = j + 1; i < line.length(); i++)  //si il y a autre chose derriere
	{
		if (line[i] != 9 && line[i] != 32 && line[i] != '}')
			throw std::invalid_argument("Parsing error, no argument accepted after the semicolon\n");
		if (line[i] == '}') //parenthese de fin de ligne acceptee uniquement 
		{
			i++;
			while (line[i])
			{
				if (line[i] != 9 && line[i] != 32)
					throw std::invalid_argument("Parsing error, no argument accepted after the brace\n");
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
	return line;
}

void	ParsingConf::doParsing(std::string line, std::vector<std::string>& buffer)
{
	line.erase(std::remove_if(line.begin(), line.end(), cleaning),line.end());
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] == '#')
		{
			line = line.substr(0, i); //enlever les commentaires
			break;
		}
	}
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
}

void	ParsingConf::fillStructs(std::vector<std::string>& buffer)
{
	size_t	i = 2;
	
	if (buffer.empty() || buffer[0] != "server")
		throw std::invalid_argument(" Parsing error, configuration files need to begin by a server\n");
	if (buffer[1].empty() || buffer[1] != "{")
		throw std::invalid_argument(" Parsing error, miss '{' after 'server' to opening the block\n");
	checkStructure(buffer); //parenthesis well closed, blocks well positionned
	
	servers.push_back(ServerConf());//creer le premier bloc server dans le contener
	std::vector<ServerConf>::iterator itServer;
	itServer = servers.begin();
	
	while(i < buffer.size()) //do a switch case ou un array(bonusCPP) pour que ce soit plus clean
	{
		if (buffer[i] == "listen") // NO IN LOCATION
		{
			i = itServer->fillListens(buffer, i + 1); //I quit args & ';' done here
			continue;
		}
		else if (buffer[i] == "server_name") //NO IN LOCATION
		{
			i = itServer->fillServerName(buffer, i + 1);
			continue;
		}	
		else if (buffer[i] == "autoindex")
		{
			i = itServer->fillAutoIndex(buffer, i + 1);
			continue;
		}

		// else if (buffer[i] == "root")

		// else if (buffer[i] == "index")

		// else if (buffer[i] == "client_max_body_size")

		// else if (buffer[i] == "return")

		// else if (buffer[i] == "error_page")

		// else if (buffer[i] == "location")

		// else if (buffer[i] == "{" || buffer[i] == "}")
		// 	i++;
		// else if (buffer[i] == "server")
		// 	//creer un nouveau server dans le vector et changer l'it de position
		//if (buffer[i] == ";" || buffer[i] == "{" || buffer[i] == "}")
		if (buffer[i] == "{" || buffer[i] == "}")
			i++;
		else
			throw std::invalid_argument(" Parsing error, invalid directives\n");
	}
	return;
}

void	ParsingConf::checkStructure(std::vector<std::string>& buffer)
{
	std::stack<std::string> stack;

	for (size_t i = 0; i < buffer.size(); i++)
	{
		//std::cout << BLUE << buffer[i] << std::endl;
		if (buffer[i] == "server")
		{
			if (buffer[i + 1].empty() || buffer[i + 1] != "{")
				throw std::invalid_argument(" Parsing error, miss '{' to opening server's block\n");
			stack.push(buffer[i]);
		}
		else if (buffer[i] == "location")
		{
			if (stack.empty() || stack.top() != "server")
				throw std::invalid_argument(" Parsing error, location need to be inside a server block\n");
			if (buffer[i + 1].empty() || buffer[i + 2].empty() || buffer[i + 2] != "{")
				throw std::invalid_argument(" Parsing error, miss '{' to opening location's block\n");
			stack.push(buffer[i]);
		}
		else if (buffer[i] == "{")
		{
			if ((!buffer[i - 1].empty() && buffer[i - 1] != "server") && (buffer[i - 2].empty() || buffer[i - 2] != "location"))
				throw std::invalid_argument(" Parsing error, wrong block's name\n");
		}
		else if (buffer[i] == "}")
		{
			if (stack.empty())
				throw std::invalid_argument(" Parsing error, trying to close a block not open\n");
			stack.pop();
		}
	}
	if (!stack.empty())
		throw std::invalid_argument(" Parsing error, a block is not closed\n");
	return;
}

//comparer avec nginx pour les messages d'erreurs
//checker si une ligne est si grande que la fonction me la retourne en deux fois ? 
