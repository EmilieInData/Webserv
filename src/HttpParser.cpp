/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:59:58 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/20 14:48:57 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"
#include <iostream>
#include <cctype>



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


std::vector<std::string>	HttpParser::parseRequestLine( std::string const & line ) {
	std::vector<std::string>	tokens = isspaceSplit( line );

	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite = tokens.end();

	for ( it = tokens.begin(); it != ite; ++it ) 
		std::cout << *it << "|";
	
	//metodo mas largo que cuaquiera que implementa el servidor error: 501 (Not Implemented)
	// uri mas largo que 8000 octets ( aconsejado como minimo ) error:  414 (URI Too Long)

	return tokens;
}


void	HttpParser::parseHttpMessage( std::string const & message ) {

	std::vector<std::string> lines = crlfSplit( message );

	if ( !lines.back().empty()) throw std::invalid_argument( "Error 400: Bad Request" );

	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite = lines.end();
	std::string::const_iterator	s_it;
	std::string::const_iterator	s_ite;

//	int i = 0;

	for ( it = lines.begin(); it != ite; ++it ) {
		s_ite = (*it).end();
		for ( s_it = (*it).begin(); s_it != s_ite; ++s_it ) {
			if ( *s_it == '\r' ) throw std::invalid_argument( "Error 400: Bad Request" );
		}
	}


	for ( it = lines.begin(); it != ite; ++it ) 
		std::cout << *it << "|\n";

}







