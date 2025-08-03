/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/03 11:11:02 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Utils.hpp"
#include "ParsingConf.hpp"
#include "ServerData.hpp"
#include "HttpRequest.hpp"
#include "Response.hpp"
#include "PrintLog.hpp"

class ServerManager
{
	private:
		Response						_response;
		std::vector<ServerData>			_serverData;
		std::vector<int>				_socketFd;
		std::vector<struct sockaddr_in>	_servAddr;
		
		ServerManager();
		ServerManager(ServerManager const &copy);
		ServerManager &operator=(ServerManager const &copy);
		
	public:
		ServerManager(ParsingConf &parsData);
		~ServerManager();
	
		void						servSetup();
		void						servRun();
		void						servListen(std::pair<int, std::string> _listens);
		std::vector<ServerData>		getServersList() const;
		struct pollfd				*servPoll(size_t totalSocket);
		std::pair<int, std::string>	getSocketData(int socketFd);
		// TODO create servQuit() to stop all servers;

};

#endif
