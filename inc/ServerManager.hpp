/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/22 18:06:38 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Utils.hpp"
#include "Server.hpp"
#include "ParsingConf.hpp"
#include "ServerData.hpp"

class ServerManager
{
	private:
		std::vector<Server>		_servers;
		std::vector<ServerData>	_serverData;
		
		ServerManager();
		ServerManager(ServerManager const &copy);
		ServerManager &operator=(ServerManager const &copy);
		
	public:
		ServerManager(ParsingConf &parsData);
		~ServerManager();
	
	void	servSetup();
	void	servRun();
	// create servQuit() to stop all servers;
};

#endif
