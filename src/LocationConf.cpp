/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/07/09 18:34:56 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "LocationConf.hpp"
#include "ServerConf.hpp"

LocationConf::LocationConf() {}

LocationConf::LocationConf(ServerConf const& S)
{
    _autoindex = S.getAutoindex();
	_root = S.getRoot();
	_index = S.getIndex();
    _bodySize = S.getBodySize();
    _returnDirective = S.getReturnDirective();
	_errorPage = S.getErrorPage();
	_allowedMethods = S.getAllowedMethods();
    _cgiPass = "";
}
  
LocationConf::~LocationConf() {} 
//detruire des trucs ici?

LocationConf::LocationConf(LocationConf const& other) : ABlockBase (other)
{
    *this = other;
}

LocationConf&	LocationConf::operator=(LocationConf const& other)
{
    if (this != &other)
    {
		ABlockBase::operator=(other);
    	_cgiPass = other._cgiPass;
    }
    return *this;
}

bool    LocationConf::getAutoindex() const
{
	return _autoindex;
}
