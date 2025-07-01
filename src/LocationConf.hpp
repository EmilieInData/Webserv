/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:35:03 by esellier          #+#    #+#             */
/*   Updated: 2025/07/01 18:42:21 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONF_HPP
#define LOCATIONCONF_HPP

#include "Utils.hpp"

class ServerConf;

class LocationConf
{
	public:
		LocationConf();
		LocationConf(ServerConf const& S);
		~LocationConf();
		LocationConf(LocationConf const& other);
		LocationConf&	operator=(LocationConf const& other);

		bool	checkFlag(std::string const& value);
		size_t	fillAutoIndex(std::vector<std::string>& buffer, size_t i);
		size_t	fillRoot(std::vector<std::string>& buffer, size_t i);
		size_t	fillIndex(std::vector<std::string>& buffer, size_t i);
		
	protected:
		bool    							_autoindex;
		std::string							_root;
		std::string							_bodySize;
		std::string							_returnDirective;
		std::vector<std::string>			_index;
		std::map<unsigned int, std::string>	_errorPage;

		std::string               			_cgiPass; // tjr ds un bloc location
		std::vector<std::string>    		_allowedMethods; // default= GET POST

		std::vector<std::string>			_flag;
};

#endif


// root->	            dossier racine pour cette location
// allowedMethods->     méthodes HTTP autorisées (GET, POST…)
// autoindex->	        booléen activant/désactivant l’autoindex
// cgiPass->	        chemin vers l’exécutable CGI
// clientMaxBodySize->	taille max acceptée du corps HTTP en octets
// returnDirective->	Pour return 301 URL; ou autre directive de retour

