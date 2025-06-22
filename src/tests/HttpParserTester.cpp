/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParserTester.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:22:15 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/22 14:40:10 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParserTester.hpp"
#include "HttpParser.hpp"
#include "RequestLine.hpp"
#include <iostream>
#include <string>

//Request Message

void	HttpParserTester::crWithoutLf() {
	try {
		std::string	http_mess( "POST /for\rm HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    CR without LF accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  CR without LF in body don't accepted/ Test OK" << std::endl;
	}

	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apel\rido=perez" );
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
		std::cout << "Valid message:    one empty line (CRLF) before Request-Line accepted / Test OK" << std::endl;
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

void	HttpParserTester::sfWithoutCrlf() {
	try {
		std::string	http_mess( "POST /form HTTP/1.1" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    string without CRLF / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  string without CRLF / Test OK" << std::endl;
	}
}

void	HttpParserTester::shouldHaveOneHost() {
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:    two hosts accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  two hosts don't accepted/ Test OK" << std::endl;
	}

	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apel\rido=perez" );
		HttpParser::parseHttpMessage( http_mess );
		std::cout << "Valid message:     message without host accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  message without host don't accepted / Test OK" << std::endl;
	}
}

void	HttpParserTester::parseHttpMessageTest() {
	
	std::cout << "_____________Http message tests_____________" << std::endl;

	crWithoutLf();
	emptyLinesBeforeReqLine();
	isspaceBeforeHeader();
	sfWithoutCrlf();
	shouldHaveOneHost();

	std::cout << std::endl;

}

//Request Line

void	HttpParserTester::shouldHaveTwoSpaces() {
	try {
		std::string	req_line( "POST /form HTTP/1.1" );
		RequestLine * req = HttpParser::parseRequestLine( req_line );
		delete req;
		std::cout << "Valid reqline:    two spaces accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  two spaces don't accepted / Test FAIL" << std::endl;
	}
	try {
		std::string	req_line( "POST /form  HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << "Valid reqline:    three spaces accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  three spaces don't accepted / Test OK" << std::endl;
	}
	try {
		std::string	req_line( "POST /formHTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << "Valid reqline:    one space accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  one space don't accepted / Test OK" << std::endl;
	}
}

void	HttpParserTester::shouldHaveThreeTokens() {
	try {
		std::string	req_line( "POST /form HTTP/1.1" );
		RequestLine * req = HttpParser::parseRequestLine( req_line );
		std::cout << "Valid req-line:   three tokens accepted / Test OK" << std::endl;
		delete req;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  three tokens don't accepted / Test FAIL" << std::endl;
	}
	try {
		std::string	req_line( "POST  /formHTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << "Valid req-line:   two tokens accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  two tokens don't accepted / Test OK" << std::endl;
	}
}

void	HttpParserTester::uriTooLong() {
	try {
		std::string	uri( 8001, 'x' );
		std::string req_line( "GET " + uri + " HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << "Valid req-line:   8001 octects accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ": 8001 octets don't accepted / Test OK" << std::endl;
	}

	try {
		std::string	uri( 8000, 'x' );
		RequestLine * req = HttpParser::parseRequestLine( "GET " + uri + " HTTP/1.1" );
		delete req;
		std::cout << "Valid req-line:   8000 octects accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ": 8000 octets don't accepted / Test FAIL" << std::endl;
	}
}

void	HttpParserTester::httpVersion() {
	try {
		std::string	req_line( "POST /form HTTP/1.1" );
		RequestLine * req = HttpParser::parseRequestLine( req_line );
		std::cout << "Valid http-vers:  HTTP/1.1 version accepted / Test OK" << std::endl;
		delete req;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  HTTP/1.1 version don't accepted / Test FAIL" << std::endl;
	}
	try {
		std::string	req_line( "POST  /form HTTP//1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << "Valid http-vers:  HTTP//1.1 accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  HTTP//1.1 don't accepted / Test OK" << std::endl;
	}
	try {
		std::string	req_line( "POST  /form HTTP/2.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << "Valid http-vers:   HTTP/2.1 accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << e.what() << ":  HTTP/2.1 don't accepted / Test OK" << std::endl;
	}
}






void	HttpParserTester::parseRequestLineTest() {

	std::cout << "_____________Request-line tests_____________" << std::endl;

	shouldHaveTwoSpaces();
	shouldHaveThreeTokens();
	uriTooLong();
	httpVersion();
 
	std::cout << std::endl << "_____________End tests_____________" << std::endl << std::endl;

}



