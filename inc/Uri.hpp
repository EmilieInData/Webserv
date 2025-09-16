/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:30:53 by cle-tron          #+#    #+#             */
/*   Updated: 2025/09/16 18:23:14 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
#define URI_HPP

#include <string>
#include <vector>

class Uri
{
	private:
		std::string uri;
		std::string scheme;
		std::string authority;
		std::string path;
		std::string query;
		std::string fragment;

		Uri();

	public:
		Uri(std::string const &req_target, std::string const &host);
		Uri(Uri const &src);
		~Uri();

		Uri &operator=(Uri const &rhs);

		std::string getUri() const;
		std::string getScheme() const;
		std::string getAuthority() const;
		std::string getPath() const;
		std::string getQuery() const;
		std::string getFragment() const;
};

#endif
