/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ILocationConfig.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:35:03 by esellier          #+#    #+#             */
/*   Updated: 2025/06/16 20:17:36 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ILOCATIONCONFIG_HPP
#define ILOCATIONCONFIG_HPP

#include "Common.hpp"

class ILocationConfig
{
	public:
		std::string					path;
		std::string					root;
		std::string                 cgiPass;
		unsigned long				bodySize;
		bool    					autoindex;
		std::vector<std::string>    allowedMethods;
		std::string					returnDirective;
		
		ILocationConfig();
		virtual ~ILocationConfig();
		ILocationConfig(ILocationConfig& other);
		
		ILocationConfig&  			operator=(ILocationConfig& other);
};

#endif


// path->	            chemin de la location, obligatoire
// root->	            dossier racine pour cette location
// allowedMethods->    méthodes HTTP autorisées (GET, POST…)
// autoindex->	        booléen activant/désactivant l’autoindex
// cgiPass->	        chemin vers l’exécutable CGI
// clientMaxBodySize->	taille max acceptée du corps HTTP en octets
// returnDirective->	Pour return 301 URL; ou autre directive de retour
