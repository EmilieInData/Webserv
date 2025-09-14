/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/14 11:27:54 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Graphics.hpp"
#include "HttpRequest.hpp"
#include "ParsingConf.hpp"
#include "Response.hpp"
#include "Utils.hpp"

class ServerManager;

class Client {
	public:
		int fd;
		struct sockaddr_in addr;
		socklen_t addrLen;
		HttpRequest request;
		Response response;
		std::string response_data;
		size_t bytes_sent;
		time_t last_action_time;

		Client(int client_fd, struct sockaddr_in client_addr,
			   ServerManager& manager, std::pair<int, std::string> incoming);
		~Client();
};

class ServerManager {
	public:
		ServerManager(ParsingConf& parsData);
		~ServerManager();

		void servSetup();
		void servRun();
		void servQuit();

		std::vector<ServerData> getServersList() const;
		Script& getScript();
		int getReqCount() const;
		int getRspCount() const;
		std::set<std::pair<int, std::string> > getUniqueListens();
		CookieData* getSession(const std::string& sessionId);
		std::string createSession(const std::string& username);
		std::pair<int, std::string> getSocketData(int socketFd);

	private:
		std::vector<ServerData> _serverData;
		std::set<std::pair<int, std::string> > _uniqueListens;
		std::vector<int> _socketFd;
		std::vector<struct sockaddr_in> _servAddr;
		bool _running;
		int _reqCount;
		int _rspCount;
		Script _script;
		std::map<std::string, CookieData> _sessions;

		std::vector<struct pollfd> _polls;
		std::map<int, Client*> _clients;

		void servListen(std::pair<int, std::string> _listens);
		void servPollSetup();
		void servInput();

		void handleNewConnection(int listener_fd);
		void handleClientRead(int client_fd);
		void handleClientWrite(int client_fd);
		void removeClient(int client_fd);
};

#endif