/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:32:59 by esellier          #+#    #+#             */
/*   Updated: 2025/08/25 18:04:01 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <sys/types.h>
#include <dirent.h>


#include "PrintLog.hpp"

class ServerManager;

#define ERROR "📍"
#define RESET "\033[0m"
#define BLUE "\033[38;5;147m"
#define PINK "\033[38;5;207m"
#define GREEN "\033[32m"
#define PURPLE "\033[38;5;62m"
#define GREY "\033[38;5;236m"
#define RED "\033[31m"

#define CLIENT_HEADER_TIMEOUT	5
#define CLIENT_BODY_TIMEOUT		5

// listen
bool		isInt(std::string const &value);
bool		isLocal(std::string const &value);
bool		isSocket(std::string const &value);
int			strToInt(std::string const &value);
bool		checkSocketAddress(std::string const &value);
std::string socketToIp(std::string const &value);
int			socketToPort(std::string const &value);

// server_name
bool checkDns(std::vector<std::string> &tmp);
bool checkLabel(std::string const &str);

// body_size
unsigned int strToSize(std::string const &value);

// error_page
std::map<int, std::pair<std::string, std::string> > defaultErrorPages();
bool												isErrorPage(std::string const &value);
bool												isHtmlAddress(std::string const &value);
void checkErrorPage(std::map<int, std::string> const &value);

// signals
void setupSignal();
void handleSignal(int signum);

// timestamp
const std::string timeStamp();

// use colors without affecting linelength
size_t visLen(const std::string &str);

// others
std::string intToString(const int &num);

// check file extension
bool isBinary(std::string location);
bool isFolder(std::string location);

#endif
