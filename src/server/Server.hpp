/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:54 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/06/30 11:37:23 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <ctime>



class Server
{
	private:
		time_t _startTime;
	
	public:
		Server();
		Server(Server const &copy);
		Server &operator=(Server const &copy);
		~Server();

		void serverRun();	 
};

#endif