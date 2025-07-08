/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:35:03 by esellier          #+#    #+#             */
/*   Updated: 2025/07/08 12:45:01 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONF_HPP
#define LOCATIONCONF_HPP

#include "Utils.hpp"
#include "BlockBase.hpp"

class ServerConf;

class LocationConf : public BlockBase
{
	public:
		LocationConf();
		LocationConf(ServerConf const& S);
		~LocationConf();
		LocationConf(LocationConf const& other);
		LocationConf&	operator=(LocationConf const& other);

		// std::string	getCgiPass() const;
		// size_t		fillCgiPass(std::vector<std::string>& buffer, size_t i);

	private:
			std::string		_cgiPass; // tjr ds un bloc location
			
		// bool    							_autoindex;
		// std::string							_root;
		// std::string							_bodySize;
		// std::string							_returnDirective;
		// std::vector<std::string>			_index;
		// std::map<unsigned int, std::string>	_errorPage;
		// std::vector<std::string>    		_allowedMethods; // default= GET POST
};

#endif
