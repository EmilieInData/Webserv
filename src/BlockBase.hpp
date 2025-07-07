/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockBase.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/07/07 19:28:56 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCKBASE_HPP
#define BLOCKBASE_HPP

#include "Utils.hpp"

class BlockBase
{
	public:
		BlockBase();
		~BlockBase();
		BlockBase(BlockBase const& other);
		BlockBase&	operator=(BlockBase const& other);

		bool									getAutoindex() const;
		std::string								getRoot() const;
		unsigned int							getBodySize() const;
		std::string								getReturnDirective() const;
		std::vector<std::string>				getIndex() const;
		std::map<unsigned int, std::string>		getErrorPage() const;
		std::vector<std::string>				getAllowedMethods() const;
		//flags

		bool	checkFlag(std::string const& value);
		size_t	fillAutoIndex(std::vector<std::string>& buffer, size_t i);
		size_t	fillRoot(std::vector<std::string>& buffer, size_t i);
		size_t	fillIndex(std::vector<std::string>& buffer, size_t i);
		size_t	fillBodySize(std::vector<std::string>& buffer, size_t i);
		// size_t	fillReturnDirectives(std::vector<std::string>& buffer, size_t i);
		// size_t	fillErrorPage(std::vector<std::string>& buffer, size_t i);
		size_t	fillAllowedMethods(std::vector<std::string>& buffer, size_t i);


	protected:
		bool    							_autoindex;
		std::string							_root;
		unsigned int						_bodySize;
		std::string							_returnDirective;
		std::vector<std::string>			_index;
		std::map<unsigned int, std::string>	_errorPage;
		std::vector<std::string>    		_allowedMethods;
		std::vector<std::string>			_flag;
};

#endif


// root->	            dossier racine pour cette location
// allowedMethods->     méthodes HTTP autorisées (GET, POST…)
// autoindex->	        booléen activant/désactivant l’autoindex
// cgiPass->	        chemin vers l’exécutable CGI
// clientMaxBodySize->	taille max acceptée du corps HTTP en octets
// returnDirective->	Pour return 301 URL; ou autre directive de retour