/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/06/24 21:02:23 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "LocationConf.hpp"
#include "ServerConf.hpp"

LocationConf::LocationConf(ServerConf const& S)
{
    autoindex = S.getAutoindex();
	root = S.getRoot();
	index = S.getIndex();
    bodySize = S.getBodySize();
    returnDirective = S.getReturnDirective();
	errorPage = S.getErrorPage();
    cgiPass = "";
    allowedMethods.push_back("GET");
    allowedMethods.push_back("POST");
}
  
LocationConf::~LocationConf() {} 
//detruire des trucs ici?

LocationConf::LocationConf(LocationConf const& other)
{
    *this = other;
}
LocationConf&	LocationConf::operator=(LocationConf const& other)
{
    if (this != &other)
    {
        autoindex = other.autoindex;
        root = other.root;
        index = other.index;
        bodySize = other.bodySize;
        returnDirective = other.returnDirective;
        errorPage = other.errorPage;
        cgiPass = other.cgiPass;
        allowedMethods = other.allowedMethods;
    }
    return *this;
}
