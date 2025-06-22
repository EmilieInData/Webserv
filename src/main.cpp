/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:56:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/22 13:56:07 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpParserTester.hpp"

#include <iostream>

int	main() {

	HttpRequest();
	
	std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apel\rido=perez" );

	HttpRequest	req = HttpRequest( http_mess );

	std::cout << "method: " << req.getHttpMethod() << ", uri: " << req.getRequestUri();
	std::cout << ", version: " << req.getHttpVersion() << std::endl;


	return 0;
}
