/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:32:59 by esellier          #+#    #+#             */
/*   Updated: 2025/08/04 15:45:29 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <ctime>
#include <stack>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <csignal>
#include <set>
#include <sys/time.h>
#include <iomanip>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "PrintLog.hpp"

class	ServerManager;

#define ERROR	"📍"
#define RESET   "\033[0m"
#define BLUE	"\033[38;5;147m"
#define PINK    "\033[38;5;207m"
#define GREEN  	"\033[32m"
#define PURPLE  "\033[38;5;62m"
#define GREY    "\033[38;5;236m"
#define RED		"\033[31m"

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
std::map<int, std::pair<std::string, std::string> > defaultErrorPages();
bool                                                isErrorPage(std::string const& value);
bool                                                isHtmlAddress(std::string const& value);
void                                                checkErrorPage(std::map<int, std::string> const& value);

//signals
void	handleSignal(int signum);

//timestamp
const std::string   timeStamp();

//use colors without affecting linelength
size_t visLen(const std::string &str);

//others
std::string	intToString(const int &num);

//check file extension
bool	isBinary(std::string location);

#endif
