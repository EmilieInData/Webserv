/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConf.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:21:07 by esellier          #+#    #+#             */
/*   Updated: 2025/06/27 20:22:39 by esellier         ###   ########.fr       */
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
	line.erase(std::remove_if(line.begin(), line.end(), cleaning),line.end()); //enlever les char non imprimable
	for (size_t i = 0; i < line.length(); i++)
	{
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
}

void	ParsingConf::fillStructs(std::vector<std::string>& buffer)
{
	size_t						i = 2;
	std::vector<std::string>	blocks;
	
	if (buffer.empty() || buffer[0] != "server")
		throw std::invalid_argument(" Parsing error, configuration files need to begin by a server\n");
	if (buffer[1].empty() || buffer[1] != "{")
		throw std::invalid_argument(" Parsing error, miss '{' after 'server' to opening the block\n");
	checkStructure(buffer); //parenthesis well closed, blocks well positionned
	
	servers.push_back(ServerConf());//creer le premier bloc server dans le contener
	std::vector<ServerConf>::iterator itServer;
	itServer = servers.begin();
	blocks.push_back("server"); //mettre tout au dessus ds le while ?
	while(i < buffer.size()) //do a switch case ou un array(bonusCPP) pour que ce soit plus clean
	{
		std::cout << GREEN << " --> " << buffer[i] << RESET << std::endl;
		if (buffer[i] == "listen") // NO IN LOCATION
		{
			if (blocks.back() != "server")
				throw std::invalid_argument(" Parsing error, 'listen' directive"
					" allowed only in server block\n");	 
			i = itServer->fillListens(buffer, i + 1); //I quit args & ';' here
			continue;
		}
		else if (buffer[i] == "server_name") //NO IN LOCATION
		{
			if (blocks.back() != "server")
				throw std::invalid_argument(" Parsing error, 'server_name' directive"
					" allowed only in server block\n");	 
			i = itServer->fillServerName(buffer, i + 1);
			continue;
		}	
		else if (buffer[i] == "autoindex") //checker si dans loc ou server
		{
			if (blocks.empty())
				throw std::invalid_argument(" Parsing error, 'autoindex' directive"
					" allowed only in server or location block\n");	 
			//--> checker avec blocks si on est dans server ou location?
			i = itServer->fillAutoIndex(buffer, i + 1);
			continue;
		}
		// else if (buffer[i] == "root") //checker si dans loc ou server
		// {
		// 	if (blocks.empty())
		// 		throw std::invalid_argument(" Parsing error, 'root' directive"
		// 			" allowed only in server or location block\n");	 
		// 	i = itServer->fillRoot(buffer, i + 1);
		// 	continue;
		// }
		// else if (buffer[i] == "index")

		// else if (buffer[i] == "client_max_body_size")

		// else if (buffer[i] == "return")

		// else if (buffer[i] == "error_page")

		else if (buffer[i] == "location")
		{
			if (itServer->getLocations().find(buffer[i + 1]) != itServer->getLocations().end())
				throw std::invalid_argument(" Parsing error, this 'location'"
					" directive already exist\n");
			itServer->getLocations()[buffer[i + 1]] = LocationConf(*itServer);
			blocks.push_back("location");
			i = i + 2;
			continue;
		}
		// else if (buffer[i] == "server")
		//l'ajouter au vector blocks
		// 	//creer un nouveau server dans le vector et changer l'it de position
		//if (buffer[i] == ";" || buffer[i] == "{" || buffer[i] == "}")
		else if (buffer[i] == "{")
			i++;
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

void	ParsingConf::checkStructure(std::vector<std::string>& buffer)
{
	std::stack<std::string> stack;

	for (size_t i = 0; i < buffer.size(); i++)
	{
		//std::cout << BLUE << buffer[i] << std::endl;
		if (buffer[i] == "server")
		{
			if (buffer[i + 1].empty() || buffer[i + 1] != "{")
				throw std::invalid_argument(" Parsing error, miss '{'"
					" to opening server's block\n");
			stack.push(buffer[i]);
		}
		else if (buffer[i] == "location")
		{
			if (stack.empty() || stack.top() != "server")
				throw std::invalid_argument(" Parsing error, 'location' blocks "
					"allowed only in a server block\n");
			if (buffer[i + 1].empty() || buffer[i + 2].empty() || buffer[i + 2] != "{")
				throw std::invalid_argument(" Parsing error, miss '{' to"
					" opening location's block\n");
			stack.push(buffer[i]);
		}
		else if (buffer[i] == "{")
		{
			if ((!buffer[i - 1].empty() && buffer[i - 1] != "server")
				&& (buffer[i - 2].empty() || buffer[i - 2] != "location"))
				throw std::invalid_argument(" Parsing error, wrong block's name\n");
		}
		else if (buffer[i] == "}")
		{
			if (stack.empty())
				throw std::invalid_argument(" Parsing error, trying to close"
					" a block not open\n");
			stack.pop();
		}
	}
	if (!stack.empty())
		throw std::invalid_argument(" Parsing error, a block is not closed\n");
	return;
}

//comparer avec nginx pour les messages d'erreurs
//checker si une ligne est si grande que la fonction me la retourne en deux fois ? 
