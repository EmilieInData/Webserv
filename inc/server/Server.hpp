/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:54 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/01 16:20:24 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include "../utils/Utils.hpp"

class Server
{
	private:
		time_t _createdTime;
		int _socketFd;
		
	public:
		Server();
		Server(Server const &copy);
		Server &operator=(Server const &copy);
		~Server();

		void servSetup();
		void servRun();
		
		time_t servTimeGet();
};

#endif