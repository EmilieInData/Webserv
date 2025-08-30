/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParserTester.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:22:22 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/30 10:15:59 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPARSERTESTER_HPP
#define	HTTPARSERTESTER_HPP

#define RED		"\033[31m"
#define GRE		"\033[32m"
#define RESET	"\033[0m"

#include "HttpRequest.hpp"

class	HttpRequest;

class	HttpParserTester	{
private:
	HttpParserTester();
	HttpParserTester( HttpParserTester const & src );
	~HttpParserTester();

	HttpParserTester &	operator=( HttpParserTester const & rhs );

		/*---------------Http Message-----------------*/
	static void	onlyASCII(ServerManager & s);
	static void	crWithoutLf( ServerManager & s );
	static void	emptyLinesBeforeReqLine( ServerManager & s );
	static void	isspaceBeforeHeader( ServerManager & s );
	static void shouldHaveOneHost( ServerManager & s );

	static void validHostSyntaxis();

	/*--------------Request Line------------------*/
	static void	shouldHaveTwoSpaces();
	static void shouldHaveThreeTokens();
	static void	uriTooLong();
	static void httpVersion();
	static void	implementedMethod();

	/*-------------------Uri----------------------*/
	static void	invalidCharUri();
	static void	invalidForm();
	static void	validPercentEncoded();
	static void	uriReconstruction();

	/*------------------Headers-------------------*/
	static void	parseHeaderSyntaxis();
	static void	pushHeaderValues();
	static void	pushMoreValues();

	
public:
	
	static void	run(ServerManager & s);

	static void	parseHttpMessageTest( ServerManager & s);
	static void	parseRequestLineTest();
	static void parseUriTest();
	static void	parseHostTest();
	static void	parseHeadersTest();
};

#endif
