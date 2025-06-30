/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/06/30 11:36:38 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(): _startTime(time(NULL))
{ 
	std::cout << "Server created at " << ctime(&_startTime) << std::endl;
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
//server run functions

