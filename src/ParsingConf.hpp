/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConf.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/06/24 20:18:38 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGCONF_HPP
#define PARSINGCONF_HPP

#include "Utils.hpp"

class ServerConf;

class ParsingConf
{
	public:
		ParsingConf();
		~ParsingConf();

		std::vector<ServerConf>	servers;
		
		static bool     cleaning(char c);
		void    	    checkLocation(std::string& line);
		std::string&	checkSemicolon(std::string& line);
		void	        print_tokens(std::vector<std::string> buffer) const;
		void	        checkStructure(std::vector<std::string>& buffer);
		void	        doParsing(std::string line, std::vector<std::string>& buffer);
		void	        fillStructs(std::vector<std::string>& buffer);

	private:
		ParsingConf(ParsingConf const& other);
		ParsingConf&	operator=(ParsingConf const& other);
};

#endif
