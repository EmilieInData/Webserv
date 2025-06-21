/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:59:53 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/21 12:11:21 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPARSER_HPP
#define	HTTPARSER_HPP

#define 400 "400 Bad Request"
#define 414 "URI Too Long"
#define 501 "Not Implemented"


#include <string>
#include <vector>

class	HttpParser	{
private:
	HttpParser();
	HttpParser( HttpParser const & src );
	~HttpParser();

	HttpParser &	operator=( HttpParser const & rhs );

public:
	static std::vector<std::string>	split( std::string const & str, char const delimiter );
	static std::vector<std::string>	crlfSplit( std::string const & str );
	static std::vector<std::string>	isspaceSplit( std::string const & str );
	static std::vector<std::string>	parseRequestLine( std::string const & line );
	static void						parseHttpMessage( std::string const & str );
};

#endif
