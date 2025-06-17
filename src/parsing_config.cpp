/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:21:07 by esellier          #+#    #+#             */
/*   Updated: 2025/06/17 20:34:37 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common.hpp"

void	print_tokens(std::vector<std::string> buffer)
{
	for (size_t i = 0; i < buffer.size(); i++)
		std::cout << BLUE << buffer[i] << std::endl;
	return;
}

bool	cleaning(char c)
{
	return (c < 32 && c != 9); //je garde les espaces et tab pour couper les mots
}

std::string&	semicolon(std::string& line)
{
	size_t			j;
	std::string	first;
	
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != 9 && line[i] != 32)
		{
			j = i;
			while (i < line.length() && line[i] != 9 && line[i] != 32)
				i++;
			first = line.substr(j, i - j);
			break;
		}
	}
	if (first.empty())
		return line;
	j = line.find(';');
	if (j == std::string::npos && !first.empty()) // si il n'y a pas
	{ // si server ou location, pas avant parenthese et pas juste apres
		if (first == "server" || first == "location" || first == "{" || first == "}")
			return line;
		throw std::invalid_argument("Parsing error, miss semicolon at the end of the line\n");

	}
	for (size_t i = j + 1; i < line.length(); i++)  //si il y a autre chose derriere
	{
		if (line[i] != 9 && line[i] != 32 && line[i] != '}')
			throw std::invalid_argument("Parsing error, no argument accepted after the semicolon\n");
		if (line[i] == '}')
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
	// si server ou location, pas avant parenthese et pas juste apres
	// if (first == "server" || first == "location" || first == "{" || first == "}")
	// {
	// 	size_t tmp = line.find('{');
	// 	size_t tmp2 = line.find('}');

	// 	//if ((tmp != std::string::npos && tmp < j ++ un arg entre les deux ) || (tmp2 != std::string::npos && j < tmp2));
		
	// 	//throw std::invalid_argument("Parsing error, semicolon not alloweed in bloc declaration\n");
		
	// }
	line.insert(j, " ");
	return line;
}

// Si la ligne commence par un mot-clé de bloc (server, location, etc.)
// Et contient un point-virgule :
// avant la première { → ❌ erreur
// juste après la { (comme {;) → ❌ erreur

void	do_parsing(std::string line)
{
	static std::vector<std::string>	buffer;

	line.erase(std::remove_if(line.begin(), line.end(), cleaning),line.end());
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] == '#' || (line[i] == '/' && line[i + 1] && line[i + 1] == '/'))
		{
			line = line.substr(0, i); //enlever les commentaires
			break;
		}
	}
	semicolon(line);
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != 9 && line[i] != 32) // enlever les espaces
		{
			size_t j = i;
			while (i < line.length() && line[i] != 9 && line[i] != 32)
				i++;
			buffer.push_back (line.substr(j, i - j)); //transformer en token
			std::cout << BLUE << line.substr(j, i - j) << std::endl;
		}
	}
	//print_tokens(buffer);
}

	//parser ligne par ligne en checkant si il y a un point virgule
	// a la fin, ou une parenthese , a part si c est le debut d un bloc
	//ou faire par bloc avec un buffer static qui attend une parenthese ?

	//00_j'enleve les caracteres invisibles etc en gardant aue les espaces et tab..
	//01_je coupe les mots et les gardes dans l'array static
	
	//throw exceptions if error

	//comparer avec nginx pour les messages d'erreurs
	
	//checker si une ligne est si grande que la fonction me la retourne en deux fois ? 