/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:34:31 by esellier          #+#    #+#             */
/*   Updated: 2025/07/10 18:49:40 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"
#include "../inc/ParsingConf.hpp"

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
		while (std::getline(file, line))
			P.doParsing(line, buffer);
		P.print_tokens(buffer);
		P.fillStructs(buffer);
	}
	catch(const std::exception& e)
	{
		std::cerr << ERROR << PINK << e.what() << '\n' << RESET;
		return (1);
;	}
	//ici passer la ref de mon vector de server a la struct de fabio (la creer en premier)
	return 0;
}
