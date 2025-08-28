/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/08/26 18:28:58 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Utils.hpp"
#include "../../inc/LocationConf.hpp"
#include "../../inc/ServerData.hpp"
#include "../../inc/ABlockBase.hpp"

LocationConf::LocationConf() {}

LocationConf::LocationConf(ServerData const& S)
{
	_autoindex = S.getAutoindex();
	_root = S.getRoot();
	_index = S.getIndex();
	_bodySize = S.getBodySize();
	_returnDirective = S.getReturnDirective();
	_errorPage = S.getErrorPage();
	_allowedMethods = S.getAllowedMethods();
	_cgiPass = S.getCgiPass();
}
  
LocationConf::~LocationConf() {} 

LocationConf::LocationConf(LocationConf const& other) : ABlockBase (other)
{
	*this = other;
}

LocationConf&	LocationConf::operator=(LocationConf const& other)
{
	if (this != &other)
	{
		ABlockBase::operator=(other);
		this->_key = other._key;
	}
	return *this;
}

bool    LocationConf::getAutoindex() const
{
	return _autoindex;
}

std::string const&	LocationConf::getKey() const
{
	return _key;	
}

void	LocationConf::setKey(std::string const value)
{
	_key = value;
	return;
}
