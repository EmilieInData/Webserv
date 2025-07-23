/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:53 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/23 11:42:03 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerManager.hpp"

ServerManager::ServerManager(ParsingConf &parsData) 
{
	_serverData = parsData.servers;
}

ServerManager::~ServerManager() {}

void ServerManager::servSetup()
{
	for (size_t i = 0; i < _serverData.size(); i++)
	{
		_servers.push_back(Server(_serverData[i]));
		_servers[i].socketInit(_serverData[i]);
	}
}

void ServerManager::servRun()
{
	for (size_t i = 0; i < _servers.size(); i++)
		_servers[i].servStart();
}