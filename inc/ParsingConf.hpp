/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConf.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/07/14 17:24:18 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGCONF_HPP
#define PARSINGCONF_HPP

#include "Utils.hpp"
#include "ABlockBase.hpp"
#include "LocationConf.hpp"

class ServerConf;

class ParsingConf
{
	public:
		ParsingConf();
		~ParsingConf();

		std::vector<ServerConf>	servers;
		
		std::vector<ServerConf>&	getServers();
		
		void		checkLocation(std::string& line);
		void		checkSemicolon(std::string& line);
		void		checkParenthesis(std::string& line);
		static void	print_tokens(std::vector<std::string>& buffer);
		void	  	print_structure();
		void	    checkStructure(std::vector<std::string>& buffer);
		void		doParsing(std::string line, std::vector<std::string>& buffer);
		size_t		fillServers(std::vector<std::string>& buffer,
								size_t& i, std::vector<ABlockBase*>& blocks,
								std::vector<ServerConf>::iterator& itServer);
		size_t		fillLocations(std::vector<std::string>& buffer,
								size_t& i, std::vector<ABlockBase*>& blocks,
								std::vector<ServerConf>::iterator& itServer,
								std::map<std::string, LocationConf>::iterator& itLocation);
		void	 	fillStructs(std::vector<std::string>& buffer);

	private:
		ParsingConf(ParsingConf const& other);
		ParsingConf&	operator=(ParsingConf const& other);
};

#endif
