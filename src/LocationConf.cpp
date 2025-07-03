/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/07/03 16:50:16 by esellier         ###   ########.fr       */
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

LocationConf::LocationConf(LocationConf const& other) : BlockBase (other)
{
    *this = other;
}

LocationConf&	LocationConf::operator=(LocationConf const& other)
{
    if (this != &other)
    {
		BlockBase::operator=(other);
    	_cgiPass = other._cgiPass;
    }
    return *this;
}
