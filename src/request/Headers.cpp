/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:47:33 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/13 18:04:31 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers.hpp"
#include "HttpParser.hpp"
#include <string>
#include <iostream>

Headers::Headers( std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite  ) {
	//pase specifics headers:
		//-content length only digit
		//cookie many values separated by ':'
		//user-agemt many values separated by space
	while ( it != ite && !(*it).empty()) {
			std::pair<std::string, std::string>	tmp = HttpParser::parseHeaderSyntaxis( *it );
		
		if ( !HttpParser::recognizeHeaderName( tmp.first )) {
			++it;
			continue;
		}

	//	std::cout << "HEADERNAME: " << tmp.first << std::endl;
		
		std::map<std::string, std::vector<std::string> >::iterator	found = header.find( tmp.first );

		

		if ( found != header.end()) {
			HttpParser::pushMoreValues( found, tmp.second );
			++it;
			continue;
		}
		
		header[ tmp.first ] = HttpParser::pushValues( tmp.first, tmp.second );
		
		++it;
	}

	printHeader();

	//PRINTHEADERS
/*	std::map<std::string, std::vector<std::string> >::iterator	it_h, ite_h = header.end();
	std::vector<std::string>::iterator							it_v, ite_v;

	for ( it_h = header.begin(); it_h != ite_h; ++it_h ) {
		std::cout << "key: " << it_h->first << "	 values: ";
		ite_v = it_h->second.end();
		for ( it_v = it_h->second.begin(); it_v != ite_v; ++it_v )
			std::cout << *it_v << " ";
		std::cout << std::endl;
	}*/
}

Headers::Headers( Headers const & src ) { *this = src; }

Headers::~Headers() {}

Headers &	Headers::operator=( Headers const & rhs ) {
	if ( this != &rhs ) 
		this->header = rhs.header;
	return *this;
}




std::map<std::string, std::vector<std::string> >::const_iterator	Headers::getHeader( std::string const & name ) const { 
	std::map<std::string, std::vector<std::string> >::const_iterator it = this->header.find( name ); 

	if ( it == this->header.end())
		throw std::invalid_argument( "Error: header dont exist!" );

	return it;
}

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
//	if ( !this->header.at( name ).at( index ))
//		throw std::invalid_argument( "Error: value dont exist!" );
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
