/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintLog.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:34:55 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/31 12:46:36 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTLOG_HPP
#define PRINTLOG_HPP

#include "Utils.hpp"
#include "ServerManager.hpp"

void	printServersData(ServerManager const &serv);
void	printServersStatus(ServerManager const &serv);
void	createLog();
void	logRequest(std::string const &request);
void	logResponse(std::string const &response);

#endif

/* max line length for print 80 char */
