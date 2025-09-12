/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:59:53 by cle-tron          #+#    #+#             */
/*   Updated: 2025/09/12 11:51:46 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPARSER_HPP
#define HTTPARSER_HPP

#define E_204 "204 No Content"
#define E_400 "400 Bad Request"
#define E_403 "403 Forbidden"
#define E_404 "404 Not Found"
#define E_405 "405 Method Not Allowed"
#define E_408 "408 Request Timeout"
#define E_413 "413 Payload Too Large"
#define E_414 "414 URI Too Long"
#define E_421 "421 Misdirected Request"
#define E_501 "501 Not Implemented"

#include "LocationConf.hpp"
#include <string>
#include <vector>

class RequestLine;
class ServerData;

class HttpParser
{
private:
	HttpParser();
	HttpParser(HttpParser const &src);
	~HttpParser();

	HttpParser &operator=(HttpParser const &rhs);

	static const std::string valid_method[];
	static const int		 valid_method_count;
	static const std::string one_header[];
	static const int		 one_h_count;
	static const std::string many_header[];
	static const int		 many_h_count;

public:
	static std::vector<std::string> split(std::string const &str, char const delimiter);
	static std::vector<std::string> isspaceSplit(std::string const &str);
	static bool						isAsciiPrintable(std::string const &str);
	static bool						isUnreservedForUri(char c);
	static bool						isReservedForUri(char c);
	static bool						isHexChar(char c);
	static bool						isDNS(std::string s);
	static std::string				toLower(std::string const &str);
	static std::string				trimSpaceAndTab(std::string &str);
	static bool						isTokenChar(char c);

	static RequestLine		 parseRequestLine(std::string const &line);
	static void				 parseReqTarget(std::string &uri);
	static std::string		 parsePath(std::string const &uri);
	static std::string		 parseQuery(std::string const &uri);
	static std::string		 parseFragment(std::string const &uri);
	static bool				 notImplementedMethod(std::string const &method);
	static ServerData const &checkIfServerExist(std::vector<ServerData> const &servers, std::pair<int, std::string> incoming);
	static void				 checkIfPathExist(std::pair<std::string, std::string> const &path, LocationConf &blockLoc, std::string const &method);
	static void				 notAllowedMethod(std::map<std::string, LocationConf>::iterator loc, std::vector<std::string> const &serv_meth, std::string const &meth);

	static std::pair<std::string, std::string> parseHost(std::string const &str);
	static bool								   checkIfHostNameExistInServer(std::string &host, std::vector<std::string> const &serv);
	static std::pair<std::string, std::string> parseHeaderSyntaxis(std::string h);
	static bool								   recognizeHeaderName(std::string name);
	static bool								   oneValueHeader(std::string name);
	static bool								   manyValuesHeader(std::string name);
	static std::vector<std::string>			   pushValues(std::string n, std::string v);
	static void								   pushMoreValues(std::map<std::string, std::vector<std::string> >::iterator h, std::string v);
	static int								   parseContentLengthHeader(std::string const &v, std::size_t body_max);
	static std::string						   parseContentTypeBoundary(std::vector<std::string> const &v);
	static LocationConf const				  *findLocation(std::string path, std::map<std::string, LocationConf> const &loc);
};
#endif
