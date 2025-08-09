/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:47:33 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/09 17:30:02 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers.hpp"
#include "HttpParser.hpp"
#include <string>
#include <iostream>

Headers::Headers( std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite  ) {
	//Parse sintaxis name tchar sin espacios con un ':'
	//Parse if name recognized by server 
	//Parse one value header
	//parse many values headers
	//pase specifics headers:
		//-content length only digit
		//cookie many values separated by ':'
		//user-agemt many values separated by space
	while ( it != ite && !(*it).empty()) {
			std::vector<std::string>	tmp = HttpParser::split( *it, ':' );
			std::vector<std::string>	value = HttpParser::split( tmp[1], ',' );

			header[ tmp[0] ] = value;

			++it;
		}

		//PRINTHEADERS
		std::map<std::string, std::vector<std::string> >::iterator	it_h, ite_h = header.end();
		std::vector<std::string>::iterator							it_v, ite_v;

		for ( it_h = header.begin(); it_h != ite_h; ++it_h ) {
			std::cout << "key: " << it_h->first << "	 values: ";
			ite_v = it_h->second.end();
			for ( it_v = it_h->second.begin(); it_v != ite_v; ++it_v )
				std::cout << *it_v << " ";
			std::cout << std::endl;
		}
}

Headers::Headers( Headers const & src ) { *this = src; }

Headers::~Headers() {}

Headers &	Headers::operator=( Headers const & rhs ) {
	if ( this != &rhs ) 
		this->header = rhs.header;
	return *this;
}

const std::string	Headers::one_header[] = { "host", "content-type", "content-length", "authorization", "user-agent", "cookie", "referer", "sec-fetch-dest", "sec-fetch-mode", "sec-fetch-site", "sec-fetch-user", "priority" };
const int			Headers::one_h_count = 12;
const std::string	Headers::many_header[] = { "accept", "accept-encoding", "accept-lenguage", "connection", "cache-control"};
const int			Headers::many_h_count = 5;
const std::string	Headers::all_headers[] = { "host", "content-type", "content-length", "authorization", "user-agent", "cookie", "referer", "sec-fetch-dest", "sec-fetch-mode", "sec-fetch-site", "sec-fetch-user", "priority", "accept", "accept-encoding", "accept-lenguage", "connection", "cache-control" };
const int			Headers::all_h_count = one_h_count + many_h_count;


std::map<std::string, std::vector<std::string> >::const_iterator	Headers::getHeader( std::string const & name ) const { 
	std::map<std::string, std::vector<std::string> >::const_iterator it = this->header.find( name ); 

	if ( it == this->header.end())
		throw std::invalid_argument( "Error: header dont exist!" );
	//std::cerr << "Be careful! " << title << " header dont exist!" << std::endl;

	return it;
}

std::vector<std::string>	Headers::getHeaderValue(  std::string const & name ) const {
	std::map<std::string, std::vector<std::string> >::const_iterator it = this->header.find( name ); 

	if ( it == this->header.end())
		throw std::invalid_argument( "Error: header dont exist!" );

	return it->second;
}


