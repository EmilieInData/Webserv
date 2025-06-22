/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:59:58 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/22 14:30:09 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"
#include "RequestLine.hpp"
#include <iostream>
#include <cctype>
#include <cstring>

std::vector<std::string>	HttpParser::split( std::string const & str, char const delimiter ) {

	std::vector<std::string>	tokens;
	std::string					t;

	int	i = 0;
	int init = 0;
	int count = 0;

	std::string::const_iterator	it;
	std::string::const_iterator	ite = str.end();

	for ( it = str.begin(); it != ite; ++it ) {
		i++;
		count++;
		if ( *it == delimiter || it == ite - 1 ) {
			init = count - i;
			if ( it == ite - 1 && *it != delimiter ) i++;
			t = str.substr( init, i - 1);
			if ( !t.empty() ) tokens.push_back( t );
			i = 0;
		}
	}
	return tokens;
}

std::vector<std::string>	HttpParser::crlfSplit( std::string const & str ) {
	std::vector<std::string>	lines;
	size_t						start = 0, end;

    while ((end = str.find( "\r\n", start)) != std::string::npos) {
        lines.push_back(str.substr(start, end - start));
        start = end + 2;
    }
    return lines;
}

std::vector<std::string>	HttpParser::isspaceSplit( std::string const & str ) {

	std::vector<std::string>	tokens;
	std::string					t;

	int	i = 0;
	int init = 0;
	int count = 0;

	std::string::const_iterator	it;
	std::string::const_iterator	ite = str.end();

	for ( it = str.begin(); it != ite; ++it ) {
		i++;
		count++;
		if ( std::isspace( *it ) || it == ite - 1 ) {
			init = count - i;
			if ( it == ite - 1 && *it != std::isspace( *it )) i++;
			t = str.substr( init, i - 1);
			if ( !t.empty() ) tokens.push_back( t );
			i = 0;
		}
	}
	return tokens;
}

std::vector<std::string>	HttpParser::parseHttpMessage( std::string const & message ) {

	std::vector<std::string> lines = crlfSplit( message );

	if ( lines.empty() || !lines.back().empty()) throw std::invalid_argument( S_400 );

	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite = lines.end();
	std::string::const_iterator			s_it, s_ite;

	int header = 0, host = 0;

	for ( it = lines.begin(); it != ite; ++it ) {
		if ( header == 0 && (*it).empty()) continue;
		if ( header > 0 && !(*it).empty()) 
			if ( std::isspace( (*it)[0] )) throw std::invalid_argument( S_400 );
		s_ite = (*it).end();
		for ( s_it = (*it).begin(); s_it != s_ite; ++s_it )
			if ( *s_it == '\r' ) throw std::invalid_argument( S_400 );
		if ( strncmp( (*it).c_str(), "Host:", 5 ) == 0 )
			host++;
		header++;
	}
	if ( host != 1 ) throw std::invalid_argument( S_400 );

	return lines;
}

RequestLine *	HttpParser::parseRequestLine( std::string const & line ) {
	
	std::string::const_iterator	s_it, s_ite = line.end();
	int							spaces = 0;

	for ( s_it = line.begin(); s_it != s_ite; ++s_it )
		if ( *s_it == ' ' ) spaces++;
	if ( spaces != 2 ) throw std::invalid_argument( S_400 );

	std::vector<std::string>	tokens = split( line, ' ' );
	
	if ( tokens.size() != 3 ) throw std::invalid_argument( S_400 );

	if ( tokens[1].length() > 8000 ) throw std::invalid_argument( S_414 );

	if ( tokens[2] != "HTTP/1.1" ) throw std::invalid_argument( S_400 );

	return new RequestLine( tokens );
}




