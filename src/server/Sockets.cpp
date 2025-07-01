/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:33:05 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/01 12:56:14 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server/Sockets.hpp"
#include "../../inc/utils/Utils.hpp"

Socket::Socket(): _socketFd(666)
{ 
	std::cout << utilsTimestamp() << "Socket Created" << std::endl;
}

Socket::Socket(Socket const &copy)
{
	*this = copy;
}

Socket &Socket::operator=(Socket const &copy)
{
	(void)copy;
	return *this;
}

Socket::~Socket()
{
	std::cout << utilsTimestamp() << "Socket destroyed" << std::endl;
}

int Socket::getSocketFd()
{
	return this->_socketFd;
}