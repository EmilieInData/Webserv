/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintLog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:42:41 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/31 12:50:30 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrintLog.hpp"

void printServersData(ServerManager const &serv)
{
	/* should print all the relevant
	information from server/config */
}

void printServersStatus(ServerManager const &serv)
{
	/* while running at intervals
	it should print listening sockets,
	number of connections happened etc... */
}

void createLog()
{
	/* creates a log file where
	requests and responses are stored 
	name should be [ssmmhhddmmyy]_log.text*/
}

void logRequest(std::string const &request)
{
	/* appends request content into
	log file */
}

void logResponse(std::string const &response)
{
	/* appends raw response into
	log file */
}