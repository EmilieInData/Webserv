/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:49:25 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/14 14:24:28 by cle-tron         ###   ########.fr       */
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
	
/*	static const std::string	one_header[];
	static const int			one_h_count;
	static const std::string	many_header[];
	static const int			many_h_count;
	static const std::string	all_headers[];
	static const int			all_h_count;
*/
	Headers();

public:
	Headers( std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite );
	Headers( Headers const & src );
	~Headers();
	
	Headers &	operator=( Headers const & rhs );

	std::map<std::string, std::vector<std::string> >::const_iterator	getHeader( std::string const & name ) const;
	std::map<std::string, std::vector<std::string> >::const_iterator	getHeaderEnd() const;
	std::vector<std::string>											getHeaderValue( std::string const & name ) const;
	std::string											getHeaderOnlyOneValue( std::string const & name, int index ) const; 
	void												printHeader() const;
};

#endif
