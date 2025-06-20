/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParserTester.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:22:15 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/20 15:19:24 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParserTester.hpp"
#include "HttpParser.hpp"
#include <iostream>
#include <string>

void	HttpParserTester::crWithoutLf() {
	try {
		std::string	http_mess( "POST /form HTTP/1.1\rHost: www.ejemplo.com\r\n\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message: CR without LF accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << "Invalid message: CR without LF / Test OK" << std::endl;
	}

	try {
		std::string	http_mess( "POST /form HTTP/1.1Host: www.ejemplo.com\r\n\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan\r&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message: CR without LF in body accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << "Invalid message: CR without LF / Test FAIL" << std::endl;
	}

}

void	HttpParserTester::parseHttpMessageTest() {
	crWithoutLf();
}







