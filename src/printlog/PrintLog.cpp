/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintLog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:42:41 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/04 11:16:22 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrintLog.hpp"
#include "Graphics.hpp"

void	printServerManager(ServerManager const &servMan)
{
	std::cout << BLUE << WEBSERV_BANNER << RESET << std::endl;
	graTopLine();
	graTime("Server Started");
	graEmptyLine();
	graTextHeader("# of servers");
	graTextElement(servMan.getServersList().size());
	
	for (size_t i = 0; i < servMan.getServersList().size(); i++)
		printServersData(servMan.getServersList()[i], i);
	graBottomLine();
}

void	printServersData(ServerData const &serv, size_t i)
{
	graSeparator();
	graTextHeader("Server number " + intToString(i + 1));
	printServerNames(serv.getServerName());
	printServerListens(serv.getListens());
	printServerLocations(serv);

	// TODO add all missing relevant server info (full locations data?)
}

void	printServerNames(std::vector<std::string> names)
{
	graTextHeader("Server Names");
	
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = names.end();

	for (it = names.begin(); it != ite; it++)
		graTextElement(*it);
}

void	printServerListens(std::vector< std::pair<int, std::string> > listens)
{
	graEmptyLine();
	graTextHeader("Listening Sockets");

	std::vector< std::pair<int, std::string> >::iterator it;
	std::vector< std::pair<int, std::string> >::iterator ite = listens.end();

	for (it = listens.begin(); it != ite; it++)
	{
		std::ostringstream num;
		num << it->first;
		graTextElement(it->second + ":" + num.str());
	}
}

void	printServerLocations(ServerData const &serv)
{
	graEmptyLine();
	graTextHeader("Root");
	graTextElement(serv.getRoot());
	graEmptyLine();
	graTextHeader("Locations");
	
	std::map<std::string, LocationConf>::iterator	it;
	std::map<std::string, LocationConf>::iterator ite = serv.getLocations().end();

	for (it = serv.getLocations().begin(); it != ite; it++)
	{
		graTextElement(it->second.getKey());
	}
}

void	printServersStatus(ServerManager &serv)
{
	// TODO make this function called from the command line while server is running
	graTopLine();
	graTime("Waiting for connection");
	graEmptyLine();
	graTextHeader("Requests received");
	graTextElement(intToString(serv.getReqCount()));
	graEmptyLine();
	graTextHeader("Responses sent");
	graTextElement(intToString(serv.getRspCount()));
	graEmptyLine();
	graTextHeader("Listening Sockets");
	
	const std::set<std::pair<int, std::string> >& listens = serv.getUniqueListens();
	std::set<std::pair<int, std::string> >::const_iterator it;
	std::set<std::pair<int, std::string> >::const_iterator ite = listens.end();
	
	for (it = listens.begin(); it != ite; it++)
		graTextElement(it->second + ":" + intToString(it->first));
	
	graBottomLine();
	/* while running at intervals
	it should print listening sockets,
	number of connections happened etc... */
}

void	printRequest(ServerManager &serv, int socketFd, std::string request, std::string fullPath)
{
	std::pair<int, std::string> incoming = serv.getSocketData(socketFd);
	graTopLine();
	graTime("Request received");
	graEmptyLine();
	graTextHeader("Request number");
	graEmptyLine();
	graTextElement(serv.getReqCount());
	graEmptyLine();
	graTextHeader("Received on");
	graTextElement(incoming.second + ":" + intToString(incoming.first));
	graEmptyLine();
	graTextHeader("Path for request");
	graTextElement(fullPath);
	graEmptyLine();
	graTextLine("Request content below.");
	graBottomLine();
	std::cout << request << std::endl;
	graTopLine();
	graTextLine("End of request");
	graBottomLine();
}

void	printResponse(ServerManager &serv, std::pair<int, std::string> incoming, std::string fullResponse, std::string fullPath)
{
	graTopLine();
	graTime("Response sent");
	graEmptyLine();
	graTextHeader("Response Number");
	graTextElement(intToString(serv.getRspCount()));
	graEmptyLine();
	graTextHeader("Request Number");
	graTextElement(intToString(serv.getReqCount()));
	graEmptyLine();
	graTextHeader("Socket received");
	graTextElement(incoming.second + ":" + intToString(incoming.first));
	graEmptyLine();
	graTextHeader("Response location");
	graTextElement(fullPath);
	graTextLine("Raw response below");
	graBottomLine();
	
	for (size_t i = 0; i < fullResponse.size(); i++)
	{
		if (fullResponse[i] == '\r') std::cout << "\\r";
		else if (fullResponse[i] == '\n') std::cout << "\\n\n";
		else std::cout << fullResponse[i];
	}
	
	graTopLine();
	graTextHeader("Response size");
	graTextElement(intToString(fullResponse.size()) + "bytes");
	graBottomLine();
}

void	printBoxMsg(std::string const &str)
{
	graTopLine();
	graTime(str);
	graBottomLine();
}

void	printBoxError(std::string const &str)
{
	graTopLine();
	graError(str);
	graBottomLine();
}

// void createLog()
// {
// 	/* creates a log file where
// 	requests and responses are stored 
// 	name should be [ssmmhhddmmyy]_log.text
// 	name/location to be stored in ServerManager*/
// }

// void logRequest(std::string const &request)
// {
// 	/* appends request content into
// 	log file */
// }

// void logResponse(std::string const &response)
// {
// 	/* appends raw response into
// 	log file */
// }