/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/16 17:14:08 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "HttpRequest.hpp"
#include "ParsingConf.hpp"
#include "Response.hpp"
#include "Utils.hpp" // <-- ADD THIS INCLUDE
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <set>
#include <sys/socket.h>
#include <ctime>
#include <vector>

class ServerManager;

#define REQ_LINE_TIMEOUT 5

struct ClientConnection
{
	int							clientFd;
	struct sockaddr_in			clientAddr;
	socklen_t					clientLen;
	HttpRequest					req;
	Response				   *resp;
	std::pair<int, std::string> incoming;
	bool						requestComplete;
	time_t						lastActivityTime;

	ClientConnection(int fd, const struct sockaddr_in &addr, socklen_t len, const std::pair<int, std::string> &inc, ServerManager &sm);
	~ClientConnection();
};

class ServerManager
{
private:
	bool								   _running;
	int									   _reqCount;
	int									   _rspCount;
	std::vector<ServerData>				   _serverData;
	std::set<std::pair<int, std::string> > _uniqueListens;
	std::vector<int>					   _socketFd;
	std::vector<struct sockaddr_in>		   _servAddr;
	int									   _inputFd;
	Script								   _script;
	std::map<std::string, CookieData>	   _sessions;
	std::map<int, ClientConnection *>	   _clients;
	std::vector<struct pollfd>			   _polls;

public:
	ServerManager(ParsingConf &parsData);
	~ServerManager();

	void servSetup();
	void servRun();
	void servQuit();

	std::vector<ServerData>				   getServersList() const;
	Script								  &getScript();
	int									   getReqCount() const;
	int									   getRspCount() const;
	std::set<std::pair<int, std::string> > getUniqueListens();
	CookieData							  *getSession(const std::string &sessionId);
	std::string							   createSession(const std::string &username);
	void								   servListen(std::pair<int, std::string> _listens);
	void								   servInput();
	std::pair<int, std::string>			   getSocketData(int socketFd);

	void handleNewConnection(int listeningSocket);
	void handleClient(struct pollfd &pfd);
	void handleRead(int clientFd);
	void handleWrite(int clientFd);
	void closeConnection(int clientFd);
	void checkErrors();
};

#endif