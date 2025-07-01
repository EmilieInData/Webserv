/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:55:07 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/01 12:55:36 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/utils/Utils.hpp"

const std::string utilsTimestamp()
{
	std::string timeStamp;
	char buffer[64];
	time_t timeNow = time(NULL);
	struct tm *timeData = localtime(&timeNow);
	
	strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", timeData);
	timeStamp = "\033[35m" + std::string(buffer) + "\033[0m";
	
	return timeStamp;
}


// vector<std::string>	HttpParser::split( std::string & const str, std::string & const delimiter ) {}

