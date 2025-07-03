/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:34:31 by esellier          #+#    #+#             */
/*   Updated: 2025/07/03 18:02:33 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ParsingConf.hpp"
#include "ServerConf.hpp"

int main(int ac, char** av)
{
	if (ac != 2)
		return (std::cerr << ERROR << PINK << "Error, one configuration file is required\n", 1);
		
	std::ifstream file(av[1]);
	if (!file)
		return (std::cerr << ERROR << PINK << "Error, file not open\n", 1);
	
	ParsingConf P;
	
	try
	{	
		static std::vector<std::string>	buffer;
		std::string line;
		while (std::getline(file, line))//return false when empty (end)
			P.doParsing(line, buffer);
		P.print_tokens(buffer);
		P.fillStructs(buffer);
	}
	catch(const std::exception& e)
	{
		std::cerr << ERROR << PINK << e.what() << '\n';
		return (1);
;	}
	//std::cout << PINK << P.servers[0].getLocations()["/"].getAutoindex() << std::endl;
	return 0;
}

//mettre un compteur de ligne pour preciser d'ou vient l'erreur ?