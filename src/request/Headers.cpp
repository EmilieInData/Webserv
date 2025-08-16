/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:47:33 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/16 16:21:26 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers.hpp"
#include "HttpParser.hpp"
#include <string>
#include <iostream>

Headers::Headers() {}

Headers::Headers( Headers const & src ) { *this = src; }

Headers::~Headers() {}

Headers &	Headers::operator=( Headers const & rhs ) {
	if ( this != &rhs ) 
		this->header = rhs.header;
	return *this;
}

void	Headers::setHeader( std::string & str ) {
	std::pair<std::string, std::string>	tmp = HttpParser::parseHeaderSyntaxis( str );
		
	if ( !HttpParser::recognizeHeaderName( tmp.first )) return;
	
	std::map<std::string, std::vector<std::string> >::iterator	found = this->header.find( tmp.first );

	if ( found != this->header.end()) {
		HttpParser::pushMoreValues( found, tmp.second );
		return;
	}
		
	this->header[ tmp.first ] = HttpParser::pushValues( tmp.first, tmp.second );
}

std::map<std::string, std::vector<std::string> >::const_iterator	Headers::getHeader( std::string const & name ) const { 
	std::map<std::string, std::vector<std::string> >::const_iterator it = this->header.find( name ); 
	
	return it;
}

std::map<std::string, std::vector<std::string> >::const_iterator	Headers::getHeaderEnd() const { return this->header.end(); }

std::vector<std::string>	Headers::getHeaderValue(  std::string const & name ) const {
	std::map<std::string, std::vector<std::string> >::const_iterator it = this->header.find( name ); 

	if ( it == this->header.end())
		throw std::invalid_argument( "Error: header dont exist!" );

	return it->second;
}

std::string	Headers::getHeaderOnlyOneValue(  std::string const & name, int index ) const {
	std::map<std::string, std::vector<std::string> >::const_iterator it = this->header.find( name ); 

	if ( it == this->header.end())
		throw std::invalid_argument( "Error: header dont exist!" );
	return this->header.at( name ).at( index );
}

void	Headers::printHeader() const {
	std::map<std::string, std::vector<std::string> >::const_iterator	it, ite = this->header.end();
	std::vector<std::string>::const_iterator							it_v, ite_v;

	for ( it = this->header.begin(); it != ite; ++it ) {
		std::cout << "key: " << it->first << "	 values: ";
		ite_v = it->second.end();
		for ( it_v = it->second.begin(); it_v != ite_v; ++it_v )
			std::cout << *it_v << " ";
		std::cout << std::endl;
	}
}
