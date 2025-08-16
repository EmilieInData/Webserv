/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:49:25 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/16 16:21:26 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HEADERS_HPP
#define	HEADERS_HPP

#include <string>
#include <vector>
#include <map>

class	Headers	{
private:
	std::map<std::string, std::vector<std::string> >	header;
		
public:
	Headers();
	Headers( Headers const & src );
	~Headers();
	
	Headers &	operator=( Headers const & rhs );

	std::map<std::string, std::vector<std::string> >::const_iterator	getHeader( std::string const & name ) const;
	std::map<std::string, std::vector<std::string> >::const_iterator	getHeaderEnd() const;
	std::vector<std::string>											getHeaderValue( std::string const & name ) const;
	std::string											getHeaderOnlyOneValue( std::string const & name, int index ) const; 
	void												printHeader() const;
	void												setHeader( std::string & str );
};

#endif
