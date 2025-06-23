/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParserTester.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:22:22 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/22 16:30:28 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPARSERTESTER_HPP
#define	HTTPARSERTESTER_HPP

class	HttpParserTester	{
private:
	HttpParserTester();
	HttpParserTester( HttpParserTester const & src );
	~HttpParserTester();

	HttpParserTester &	operator=( HttpParserTester const & rhs );

	static void	crWithoutLf();
	static void	emptyLinesBeforeReqLine();
	static void	isspaceBeforeHeader();
	static void sfWithoutCrlf();
	static void shouldHaveOneHost();

	static void	shouldHaveTwoSpaces();
	static void shouldHaveThreeTokens();
	static void	uriTooLong();
	static void httpVersion();


	
public:
	static void	parseHttpMessageTest();
	static void	parseRequestLineTest();
};

#endif
