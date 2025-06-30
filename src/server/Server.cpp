/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/06/30 15:13:50 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(): _createdTime(time(NULL))
{ 
	std::cout << "Server created at " << ctime(&_createdTime) << std::endl;
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
	std::cout << "Server destroyed" << std::endl;
}

time_t Server::servTimeGet()
{
	return this->_createdTime;
}

void Server::servRun()
{
	time_t startTime = time(NULL);
	
	std::cout << "Server started at " << ctime(&startTime) << std::endl;
	
	while (true)
	{
		time_t loop = time(NULL);

		while ((difftime(time(NULL), loop)) <= 10)
		{
			
		}
		std::cout << "timestamp" << std::endl;
	}
}

//server run functions

