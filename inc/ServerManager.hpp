/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/04 12:54:04 by fdi-cecc         ###   ########.fr       */
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
		bool									_running;
		std::string								_input;
		Response								_response;
		std::vector<ServerData>					_serverData;
		std::vector<int>						_socketFd;
		std::vector<struct sockaddr_in>			_servAddr;
		int										_reqCount;
		int										_rspCount;
		std::set<std::pair<int, std::string> >	_uniqueListens;
		
		ServerManager();
		ServerManager(ServerManager const &copy);
		ServerManager &operator=(ServerManager const &copy);
		
	public:
		ServerManager(ParsingConf &parsData);
		~ServerManager();
	
		void									servSetup();
		void									servRun();
		void									servListen(std::pair<int, std::string> _listens);
		std::vector<ServerData>					getServersList() const;
		struct pollfd							*servPoll(size_t totalSocket);
		std::pair<int, std::string>				getSocketData(int socketFd);
		std::set<std::pair<int, std::string> >	getUniqueListens();
		int										getReqCount() const;
		int										getRspCount() const;
		void									servQuit();
		// TODO create servQuit() to stop all servers;

};

#endif
