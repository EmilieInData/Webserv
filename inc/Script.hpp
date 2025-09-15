/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Script.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:39:10 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/15 12:53:31 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "HttpRequest.hpp"
#include "PrintLog.hpp"
#include "Utils.hpp"

#define PIPE_READ 0
#define PIPE_WRITE 1

class HttpRequest;

class Script
{
private:
	int								   _statusCode;
	std::string						   _outputBody;
	std::map<std::string, std::string> _outputHeaders;
	std::string						   _cgiPath;
	std::string						   _message;
	std::string						   _contentType;
	std::string						   _scriptOutput;
	std::string						   _scriptType;
	std::string						   _scriptName;

	Script(Script const &src);
	Script &operator=(Script const &rhs);

public:
	Script();
	~Script();

	void							   runScript(HttpRequest &request, std::string const &interpreterPath, ServerManager &server);
	char							 **setEnv(HttpRequest const &request, ServerManager &server); // TODO put back to char** after testing
	bool							   setScriptType(std::string const &cgiPath);
	std::string						   getMessage() const;
	std::string						   getContentType() const;
	std::string						   getScriptOutput() const;
	void							   parseOutput();
	std::string						   getOutputBody() const;
	std::map<std::string, std::string> getOutputHeaders() const;
	void							   setStatusCode(int statusCode);
	int								   getStatusCode() const;
};

#endif

/* TODO
script parsing
	separate headers and body
	create header fields */