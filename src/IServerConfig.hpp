/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/06/16 20:18:32 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERCONFIG_HPP
#define ISERVERCONFIG_HPP

#include "Common.hpp"

class ILocationConfig;

template<typename T>
class IServerConfig
{
	public:
		IServerConfig();
		virtual ~IServerConfig();
		IServerConfig(IServerConfig& other);
		IServerConfig&  operator=(IServerConfig& other);
	
	protected:
		T										port; //int ou adresse ip
		std::string								serverName; //pas obligatoire
		std::string								root;
		std::string								index;
		std::string								bodySize; //la definir et definir une taille max si quelqu'un met plus grand ds le fichier de configuration
		std::vector<std::string>				errorPage;
		std::map<std::string, ILocationConfig>	location;
};

#endif