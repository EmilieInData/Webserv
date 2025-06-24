/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:35:03 by esellier          #+#    #+#             */
/*   Updated: 2025/06/24 21:10:48 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONF_HPP
#define LOCATIONCONF_HPP

#include "Utils.hpp"

class ServerConf;

class LocationConf
{
	public:
		LocationConf(ServerConf const& S);
		~LocationConf();
		LocationConf(LocationConf const& other);
		LocationConf&	operator=(LocationConf const& other);

	protected:
		bool    							autoindex;
		std::string							root;
		std::string							index;
		std::string							bodySize;
		std::string							returnDirective;
		std::map<unsigned int, std::string>	errorPage;

		std::string               			cgiPass; // tjr ds un bloc location
		std::vector<std::string>    		allowedMethods; // default= GET POST
};

#endif


// root->	            dossier racine pour cette location
// allowedMethods->     méthodes HTTP autorisées (GET, POST…)
// autoindex->	        booléen activant/désactivant l’autoindex
// cgiPass->	        chemin vers l’exécutable CGI
// clientMaxBodySize->	taille max acceptée du corps HTTP en octets
// returnDirective->	Pour return 301 URL; ou autre directive de retour



// class LocationConf
// {
// 	public:
// 		virtual ~ILocationConfig() = 0;

// 		void	fillLocationInstructions();

// 	protected://copier de server et redefini si info dans le bloc location
// 		ILocationConfig();
// 		ILocationConfig(ILocationConfig& other);
// 		ILocationConfig&  								operator=(ILocationConfig& other);
		
// 		std::map<std::string, std::vector<std::string>>	locationInstructions;
// };