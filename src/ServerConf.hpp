/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/06/27 20:18:36 by esellier         ###   ########.fr       */
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

		bool									getAutoindex() const;
		std::string								getRoot() const;
		std::string								getIndex() const;
		unsigned int							getBodySize() const;
		std::string								getReturnDirective() const;
		std::map<unsigned int, std::string>		getErrorPage() const;
		//std::map<std::string, LocationConf>		getLocations()const;
		std::map<std::string, LocationConf>&	getLocations();

		bool	checkFlag(std::string const& value);
		
		size_t	fillListens(std::vector<std::string>& buffer, size_t i);
		size_t	fillServerName(std::vector<std::string>& buffer, size_t i);
		size_t	fillAutoIndex(std::vector<std::string>& buffer, size_t i);
		size_t	fillRoot(std::vector<std::string>& buffer, size_t i);
		// size_t	fillIndex(std::vector<std::string>& buffer, size_t i);
		// size_t	fillBodySize(std::vector<std::string>& buffer, size_t i);
		// size_t	fillReturnDirectives(std::vector<std::string>& buffer, size_t i);
		// size_t	fillErrorPage(std::vector<std::string>& buffer, size_t i);
		// size_t	fillLocations(std::vector<std::string>& buffer, size_t i);

	protected:
		bool    							autoindex;
		std::string							root;
		std::string							index;
		unsigned int						bodySize;
		std::string							returnDirective;
		std::map<unsigned int, std::string>	errorPage;
		
		//no in location
		std::vector<listen>					listens;
		std::vector<std::string>			serverName;
		std::map<std::string, LocationConf>	locations;

		std::vector<std::string>			flag;
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