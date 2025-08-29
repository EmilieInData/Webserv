/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Script.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:39:10 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/29 11:44:23 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "Utils.hpp"
#include "HttpRequest.hpp"
#include "PrintLog.hpp"

#define PIPE_READ 0
#define PIPE_WRITE 1
class Script
{
private:
	int			_statusCode;
	std::string _message;
	std::string _contentType;
	std::string _scriptOutput;
	std::string _scriptType;

	Script(Script const &src);
	Script &operator=(Script const &rhs);

public:
	Script();
	~Script();

	void		runScript(std::string const &cgiPath);
	void		setEnv(); // TODO set environment variables
	void		setScriptType(std::string const &cgiPath);
	int			getStatusCode() const;
	std::string getMessage() const;
	std::string getContentType() const;
};

#endif

/* script execution:
	check if request is script
	run from within HttpRequest
	pass parameters to execution function
		(maybe whole request as reference?)
	run script
		set environment variables
		check parameters
		run script
		get output
		set code
		set type
		set message
	save code and result 
	get the from within HttpRequest

	WHAT IF METHOD IS POST?
		send content of bodies? how?
	*/