/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/20 15:17:03 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

HttpRequest::HttpRequest() { 
//	std::cout << "requestLine: ";

//	std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\n\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
//	http_mess.push_back(static_cast<char>(0x20));
//	std::cout << http_mess << std::endl;

	HttpParserTester::parseHttpMessageTest();
/*	try {
		HttpParser::parseHttpMessage( http_mess );
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << std::endl;
	}*/
//	HttpParser::parseRequestLine( "GET/user/create?userId=john&password=1234&name=john park HTTP/1.1" );

}

HttpRequest::~HttpRequest() {}


	

