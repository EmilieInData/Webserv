/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/01 11:40:35 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../utils/Utils.hpp"

Server::Server(): _createdTime(time(NULL))
{ 
	std::cout << utilsTimestamp() << "Server Created" << std::endl;
}

Server::Server(Server const &copy)
{
	*this = copy;
}

Server &Server::operator=(Server const &copy)
{
	(void)copy;
	return *this;
}

Server::~Server()
{
	std::cout << utilsTimestamp() << "Server destroyed" << std::endl;
}

time_t Server::servTimeGet() // delete??
{
	return this->_createdTime;
}

void Server::servRun()
{	
	std::cout << utilsTimestamp() << "Server started" << std::endl;
	
	while (true)
	{
		time_t loop = time(NULL);

		while ((difftime(time(NULL), loop)) <= 10)
			;
		
		std::cout << utilsTimestamp() <<  "Server running" << std::endl;
	}
}

//server run functions

