/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintLog.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:34:55 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/16 18:07:07 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTLOG_HPP
#define PRINTLOG_HPP

#include "Graphics.hpp"

class ServerManager;
class ServerData;

void printServerManager(ServerManager const &servMan);
void printServersData(ServerData const &serv, size_t i);
void printServersStatus(ServerManager &serv);
void printRequest(ServerManager &serv, int socketFd, std::string request,
				  std::string fullPath, std::string method);
void printResponse(ServerManager &serv, std::pair<int, std::string> incoming,
				   std::string fullResponse, std::string fullPath);
void printBoxMsg(std::string const &str);
void printBoxError(std::string const &str);
void printServerNames(std::vector<std::string> names);
void printServerListens(std::vector<std::pair<int, std::string>> listens);
void printServerLocations(ServerData const &serv);
void printRaw(std::string const &fullResponse);

#endif
