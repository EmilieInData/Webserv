/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graphics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:22:41 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/31 15:14:23 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Graphics.hpp"

void	graSimpleLine(char const &c)
{
	const size_t	padLen = std::strlen(PAD);
	const size_t	restLen = LINELENGTH - (padLen * 2);

	std::string line(restLen, c);
	std::cout << PAD << line << PAD << std::endl;
}

void graSeparator(char const &c)
{
	const size_t	leftLen = std::strlen(LEFT_SIDE);
	const size_t	rightLen = std::strlen(RIGHT_SIDE);
	const size_t	restLen = LINELENGTH - leftLen - rightLen;
	
	std::cout << LEFT_SIDE << std::string(restLen, c) << RIGHT_SIDE << std::endl;
}

void graTextLine(std::string const &str)
{
	const size_t	leftLen = std::strlen(LEFT_SIDE);
	const size_t	rightLen = std::strlen(RIGHT_SIDE);
	const size_t	strLen = std::strlen(str.c_str());
	const size_t	restLen = LINELENGTH - strLen - leftLen - rightLen;
	
	std::cout << LEFT_SIDE << str << std::string(restLen, ' ') << RIGHT_SIDE << std::endl;
}
