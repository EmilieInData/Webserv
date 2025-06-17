/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:34:31 by esellier          #+#    #+#             */
/*   Updated: 2025/06/16 20:37:23 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common.hpp"

int main(int ac, char** av)
{
	if (ac != 1)
	{
		std::cerr << ERROR << PINK << "Error, one configuration file is required\n";
		return (1);
	}
	
	std::ifstream file(av[1]);
	if (!file)
	{
		std::cerr << ERROR << PINK << "Error, files not open\n";
		return (1);
	}

	try
	{	
		std::string line;
		while (std::getline(file, line))
		{
			//do_parsing(line);
			std::cout << line << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
