/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:32:59 by esellier          #+#    #+#             */
/*   Updated: 2025/07/10 18:17:55 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
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
bool            isInt(std::string const& value);
bool            isLocal(std::string const& value);
bool            isSocket(std::string const& value);
int           	strToInt(std::string const& value);
bool            checkSocketAddress(std::string const& value);
std::string     socketToIp(std::string const& value);
int	            socketToPort(std::string const& value);

//server_name
bool            checkDns(std::vector<std::string>& tmp);
bool       	    checkLabel(std::string const& str);

//body_size
unsigned int    strToSize(std::string const& value);

//error_page
std::map<int, std::pair<std::string, std::string>>  defaultErrorPages();
bool                                                isErrorPage(std::string const& value);
bool                                                isHtmlAddress(std::string const& value);
void                                                checkErrorPage(std::map<int, std::string> const& value);

#endif
