/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:32:59 by esellier          #+#    #+#             */
/*   Updated: 2025/06/18 19:11:41 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HPP
#define COMMON_HPP

#include <map>
#include <string>
#include <vector>
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

bool	    cleaning(char c);
std::string	firstToken(std::string& line);
void	    print_tokens(std::vector<std::string> buffer);
void	    doParsing(std::string line, std::vector<std::string>& buffer);

#endif
