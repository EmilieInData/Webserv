/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:54 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/22 16:22:47 by fdi-cecc         ###   ########.fr       */
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
		int 												_socketFd;
		int 												_clientFd;
		struct sockaddr_in 									_servAddr;
		struct sockaddr_in 									_clientAddr;
		struct pollfd 										_pollFd[1];
		std::vector<ServerData>&							_serversList;
		std::map<int, std::pair<std::string, std::string> >	_defaultErrorPages;
				
	public:
		Server(ServerData& servData);
		Server(Server const &copy);
		Server &operator=(Server const &copy);
		~Server();
		
		void														servInit(ServerData &servData);
		void 														servStart();
		std::vector<ServerData> const&								getServersList() const;
		std::map<int, std::pair<std::string, std::string> > const&	getDefaultErrorPages() const;
		
		time_t	servTimeGet();
};

#endif
