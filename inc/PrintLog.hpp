/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintLog.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:34:55 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/03 20:35:22 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTLOG_HPP
#define PRINTLOG_HPP

#include "Utils.hpp"
#include "Graphics.hpp"
#include "ServerManager.hpp"


class	ServerManager;

void	printServerManager(ServerManager const &servMan);
void	printServersData(ServerData const &serv, size_t i);
void	printServersStatus(ServerManager &serv);
void	createLog();
void	logRequest(std::string const &request);
void	logResponse(std::string const &response);

void	printServerNames(std::vector<std::string> names);
void	printServerListens(std::vector< std::pair<int, std::string> > listens);
void	printServerLocations(ServerData const &serv);

#endif

/* max line length for print 80 char */
