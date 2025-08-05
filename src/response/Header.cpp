/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:04:28 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/05 16:32:00 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header(Response response): _response(response) {};

Header::~Header() {};

void	Header::setContentType()
{
	_contentType = _response.checkType();
}

void	Header::setProtocol()
{
	_protocol = "HTTP/1.1";
}