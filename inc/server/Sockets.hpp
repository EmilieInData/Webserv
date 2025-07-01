/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:33:09 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/01 11:42:40 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
#define SOCKETS_HPP

class Socket
{
	private:
		int _socketFd;
	
	public:
		Socket();
		Socket(Socket const &copy);
		Socket &operator=(Socket const &copy);
		~Socket();
	
		int	getSocketFd();
};

#endif