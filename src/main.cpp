/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:34:31 by esellier          #+#    #+#             */
/*   Updated: 2025/06/17 18:55:48 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common.hpp"

int main(int ac, char** av)
{
	if (ac != 2)
		return (std::cerr << ERROR << PINK << "Error, one configuration file is required\n", 1);
		
	std::ifstream file(av[1]);
	if (!file)
		return (std::cerr << ERROR << PINK << "Error, file not open\n", 1);

	try
	{	
		std::string line;
		while (std::getline(file, line))//return false qund il n'y a plus rien a lire
		{
			do_parsing(line);
			//std::cout << line << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << ERROR << PINK << e.what() << '\n';
	}
	//checker arguments, parenthese, ecriture des mots clefs, apres la tokenisation
	return 0;
}
