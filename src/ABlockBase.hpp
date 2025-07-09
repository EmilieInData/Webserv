/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABlockBase.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/07/09 18:35:53 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABLOCKBASE_HPP
#define ABLOCKBASE_HPP

#include "Utils.hpp"

class ABlockBase
{
	public:
		ABlockBase();
		virtual ~ABlockBase();
		ABlockBase(ABlockBase const& other);
		ABlockBase&	operator=(ABlockBase const& other);

		virtual bool				getAutoindex() const = 0;
		std::string					getRoot() const;
		unsigned int				getBodySize() const;
		std::vector<std::string>	getReturnDirective() const;
		std::vector<std::string>	getIndex() const;
		std::vector<std::string>	getAllowedMethods() const;
		std::map<int, std::string>	getErrorPage() const;
		//flags

		bool						checkFlag(std::string const& value);
		size_t						fillAutoIndex(std::vector<std::string>& buffer, size_t i);
		size_t						fillRoot(std::vector<std::string>& buffer, size_t i);
		size_t						fillIndex(std::vector<std::string>& buffer, size_t i);
		size_t						fillBodySize(std::vector<std::string>& buffer, size_t i);
		size_t						fillReturnDirectives(std::vector<std::string>& buffer, size_t i);
		size_t						fillErrorPage(std::vector<std::string>& buffer, size_t i);
		size_t						fillAllowedMethods(std::vector<std::string>& buffer, size_t i);
		
	protected:
		bool    					_autoindex;
		std::string					_root;
		unsigned int				_bodySize;
		std::vector<std::string>	_flag;
		std::vector<std::string>	_index;
		std::vector<std::string>    _allowedMethods;
		std::vector<std::string> 	_returnDirective;
		std::map<int, std::string>	_errorPage;
};

#endif


// root->	            dossier racine pour cette location
// allowedMethods->     méthodes HTTP autorisées (GET, POST…)
// autoindex->	        booléen activant/désactivant l’autoindex
// cgiPass->	        chemin vers l’exécutable CGI
// clientMaxBodySize->	taille max acceptée du corps HTTP en octets
// returnDirective->	Pour return 301 URL; ou autre directive de retour