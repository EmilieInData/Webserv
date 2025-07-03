/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/07/03 16:44:46 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONF_HPP
#define SERVERCONF_HPP

#include "Utils.hpp"
#include "LocationConf.hpp"
#include "BlockBase.hpp"

struct listen
{
	int			port;
	std::string	ip;
};

class LocationConf;

class ServerConf : public BlockBase
{
	public:
		ServerConf();
		~ServerConf();
		ServerConf(ServerConf const& other);
		ServerConf&	operator=(ServerConf const& other);

		std::map<std::string, LocationConf>&			getLocations();
		std::map<std::string, LocationConf>:: iterator	getItLocations(std::string const& key);
		size_t											fillListens(std::vector<std::string>& buffer, size_t i);
		size_t											fillServerName(std::vector<std::string>& buffer, size_t i);

	private:
		std::vector<listen>					_listens;
		std::vector<std::string>			_serverName;
		std::map<std::string, LocationConf>	_locations;

		// bool    							_autoindex;
		// std::string							_root;
		// unsigned int						_bodySize;
		// std::string							_returnDirective;
		// std::vector<std::string>			_index;
		// std::map<unsigned int, std::string>	_errorPage;
		// std::vector<std::string>    		_allowedMethods; // default= GET POST
};

#endif

//je decide que tous les infos server seront avant les blocs locations
//sinon ce sont des erreurs, plus simple pour le parsing