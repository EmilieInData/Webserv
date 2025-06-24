/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/06/24 21:05:48 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONF_HPP
#define SERVERCONF_HPP

#include "Utils.hpp"
#include "LocationConf.hpp"

struct listen
{
	int			port;
	std::string	ip;
};

class LocationConf;

class ServerConf
{
	public:
		ServerConf();
		~ServerConf();
		ServerConf(ServerConf const& other);
		ServerConf&	operator=(ServerConf const& other);

		bool								getAutoindex() const;
		std::string							getRoot() const;
		std::string							getIndex() const;
		unsigned int						getBodySize() const;
		std::string							getReturnDirective() const;
		std::map<unsigned int, std::string>	getErrorPage() const;
		
		void	fillListens(std::vector<std::string>::iterator it);
		// void	fillServerName(std::vector<std::string>& value);
		// void	fillAutoIndex(std::vector<std::string>& value);
		// void	fillRoot(std::vector<std::string>& value);
		// void	fillIndex(std::vector<std::string>& value);
		// void	fillBodySize(std::vector<std::string>& value);
		// void	fillReturnDirectives(std::vector<std::string>& value);
		// void	fillErrorPage(std::vector<std::string>& value);
		// void	fillLocations(std::vector<std::string>& value);
	
	protected:
		//mettre une valeur par defaut quand je cree la class avec une fonction	
		bool    							autoindex;
		std::string							root;
		std::string							index;
		unsigned int						bodySize;
		std::string							returnDirective;
		std::map<unsigned int, std::string>	errorPage;
		
		//no in location
		std::vector<listen>					listens;
		std::string							serverName; //put to default if no exist
		std::map<std::string, LocationConf>	locations;
};

#endif

// class AServerConfig
// {
// 	public:
// 		virtual ~IServerConfig() = 0;

// 		void	fillListens();
// 		void	fillLocations();
// 		void	fillServerInstructions();
	
// 	protected:
// 		IServerConfig();
// 		IServerConfig(IServerConfig& other);
// 		IServerConfig&  						    	operator=(IServerConfig& other);

// 		std::vector<listen>								listens;
// 		std::string										serverName; //pas obligatoire
// 		std::map<std::string, ILocationConfig>			locations;
// 		std::map<std::string, std::vector<std::string>>	serverInstructions;
// };

//je decide que tous les infos server seront avant les blocs locations
//sinon ce sont des erreurs, plus simple pour le parsing