/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:54 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/23 14:45:45 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include "ServerData.hpp"
#include "ParsingConf.hpp"

class Server
{
	private:
		Server();
		
		time_t 												_createdTime;
		std::vector<int>									_socketFd; // convert into vector for multiple sockets
		// int 												_clientFd; // moved into servStart
		std::vector<struct sockaddr_in>						_servAddr;
		// struct sockaddr_in 									_clientAddr; // moved into servStart
		// struct pollfd 										_pollFd[1]; // moving it into each servStart function
		std::vector<ServerData>&							_serversList;
		std::map<int, std::pair<std::string, std::string> >	_defaultErrorPages;
		
	public:
		Server(ServerData& servData);
		Server(Server const &copy);
		Server &operator=(Server const &copy);
		~Server();
		
		void														serverInit(ServerData &servData);
		void														servListen(std::pair<int, std::string> _listens);
		void 														servStart();
		std::vector<ServerData> const&								getServersList() const;
		// std::map<int, std::pair<std::string, std::string> > const&	getDefaultErrorPages() const;
		
		time_t	servTimeGet();
};

#endif
