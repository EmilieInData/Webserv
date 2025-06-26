/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:32:59 by esellier          #+#    #+#             */
/*   Updated: 2025/06/26 17:46:05 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

#define ERROR	"📍"
#define RESET   "\033[0m"
#define BLUE	"\033[38;5;147m"
#define PINK    "\033[38;5;207m"
#define GREEN  	"\033[38;5;194m"
#define PURPLE  "\033[38;5;62m"

//listen
bool		isInt(std::string const& value);
bool		isIp(std::string const& value);
bool		isSocket(std::string const& value);
int			strToInt(std::string const& value);
bool		checkIpAddress(std::string const& value);
bool		checkSocketAddress(std::string const& value);
std::string	socketToIp(std::string const& value);
int			socketToPort(std::string const& value);

//server_name
bool	checkDns(std::vector<std::string>& tmp);
bool	checkLabel(std::string const& str);

#endif
