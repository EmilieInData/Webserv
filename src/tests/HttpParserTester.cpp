/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParserTester.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:22:15 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/21 12:11:21 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParserTester.hpp"
#include "HttpParser.hpp"
#include <iostream>
#include <string>

//Request Message

void	HttpParserTester::crWithoutLf() {
	try {
		std::string	http_mess( "POST /form HTTP/1.1\rHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    CR without LF accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  CR without LF in body don't accepted/ Test OK" << std::endl;
	}

	try {
		std::string	http_mess( "POST /form HTTP/1.1Host: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apel\rido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    CR without LF in body accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  CR without LF in body don't accepted / Test FAIL" << std::endl;
	}
}

void	HttpParserTester::emptyLinesBeforeReqLine() {
	try {
		std::string	http_mess( "\r\n\r\n\r\nPOST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    three empty lines (CRLF) before Request-Line accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  three CRLF before req-line not accepted / Test FAIL" << std::endl;
	}

	try {
		std::string	http_mess( "\r\nPOST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    one empty lines (CRLF) before Request-Line accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  one CRLF before req-line not accepted / Test FAIL" << std::endl;
	}
}

void	HttpParserTester::isspaceBeforeHeader() {
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\n\tContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    \\tab before header accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  \\tab (isspace) before header don't accepted / Test OK" << std::endl;
	}
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\n   Host: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    spaces before header accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  spaces (isspace) before header don't accepted / Test OK" << std::endl;
	}

}

void	HttpParserTester::parseHttpMessageTest() {
	crWithoutLf();
	emptyLinesBeforeReqLine();
	isspaceBeforeHeader();
}

//Request Line

void	HttpParserTester::parseRequestLineTest() {




}



