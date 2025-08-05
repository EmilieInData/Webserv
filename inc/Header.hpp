/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:04:30 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/05 15:52:31 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include "Response.hpp"

/* TODO building a basic header class
to compose the right one to put at
the beginning of the response*/

class Header
{
private:
	Response _response;

	Header(Header const &copy);
	Header &operator=(Header const &copy);
	Header();

public:

};

#endif