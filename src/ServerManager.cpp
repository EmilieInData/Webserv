/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:53 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/23 17:58:50 by fdi-cecc         ###   ########.fr       */
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
		_serverData[i].serverInit();
}

void ServerManager::servRun()
{
	for (size_t i = 0; i < _serverData.size(); i++)
		_serverData[i].servStart();
}