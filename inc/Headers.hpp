/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:49:25 by cle-tron          #+#    #+#             */
/*   Updated: 2025/09/16 19:38:10 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <map>
#include <string>
#include <vector>

class Headers
{
	private:
		std::map<std::string, std::vector<std::string> > header;

	public:
		Headers();
		Headers(Headers const &src);
		~Headers();

		Headers &operator=(Headers const &rhs);

		size_t getHeaderSize() const;
		std::map<std::string, std::vector<std::string> >::const_iterator
		getHeader(std::string const &name) const;
		std::map<std::string, std::vector<std::string> >::const_iterator getHeaderBegin() const;
		std::map<std::string, std::vector<std::string> >::const_iterator getHeaderEnd() const;
		std::vector<std::string> getHeaderValue(std::string const &name) const;
		std::vector<std::string> &getHeaderValue(std::string const &name);
		std::string getHeaderOnlyOneValue(std::string const &name, int index) const;
		void printHeader() const;
		void setHeader(std::string &str);
		void setManyValuesHeader(std::string name);
};

#endif
