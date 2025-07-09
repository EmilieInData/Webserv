/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:35:03 by esellier          #+#    #+#             */
/*   Updated: 2025/07/09 18:33:04 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONF_HPP
#define LOCATIONCONF_HPP

#include "Utils.hpp"
#include "ABlockBase.hpp"

class ServerConf;

class LocationConf : public ABlockBase
{
	public:
		LocationConf();
		LocationConf(ServerConf const& S);
		~LocationConf();
		LocationConf(LocationConf const& other);
		LocationConf&	operator=(LocationConf const& other);

		bool			getAutoindex() const;
		// std::string	getCgiPass() const;
		// size_t		fillCgiPass(std::vector<std::string>& buffer, size_t i);

	private:
		std::string		_cgiPass; // tjr ds un bloc location
};

#endif
