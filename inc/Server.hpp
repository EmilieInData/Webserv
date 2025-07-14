/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:54 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/14 18:22:50 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include "ServerConf.hpp"
#include "ParsingConf.hpp"

class Server
{
	private:
		Server();
	
		time_t 												_createdTime;
		int 												_socketFd;
		int 												_clientFd;
		struct sockaddr_in 									_servAddr;
		struct sockaddr_in 									_clientAddr;
		struct pollfd 										_pollFd[1];
		std::vector<ServerConf>&							_serversList;
		std::map<int, std::pair<std::string, std::string> >	_defaultErrorPages;
				
	public:
		Server(ParsingConf& P);
		Server(Server const &copy);
		Server &operator=(Server const &copy);
		~Server();

		void 														servRun();
		void 														servSetup();
		std::vector<ServerConf> const&								getServersList() const;
		std::map<int, std::pair<std::string, std::string> > const&	getDefaultErrorPages() const;
		
		time_t	servTimeGet();
};

#endif
