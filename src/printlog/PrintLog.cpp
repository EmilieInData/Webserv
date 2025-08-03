/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintLog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:42:41 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/03 12:36:04 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrintLog.hpp"
#include "Graphics.hpp"

void printServersData(ServerManager const &serv)
{
	(void)serv;
	graTopLine();
	graTextLine(PROJ_TITLE);
	graSeparator();
	graTextLine(TEAM_LIST);
	graTime("Server Started");
	printServerNames(serv.getServersList()[0].getServerName());
	printServerListens(serv.getServersList()[0].getListens());
	graBottomLine();
	
	/* should print all the relevant
	information from server/config */
}

void	printServerNames(std::vector<std::string> names)
{
	graSeparator();
	graTextHeader("Server Names");
	
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = names.end();

	for (it = names.begin(); it != ite; it++)
		graTextElement(*it);
}

void	printServerListens(std::vector< std::pair<int, std::string> > listens)
{
	graSeparator();
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

// void printServersStatus(ServerManager const &serv)
// {
// 	/* while running at intervals
// 	it should print listening sockets,
// 	number of connections happened etc... */
// }

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