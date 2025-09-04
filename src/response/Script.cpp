/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Script.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:38:40 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/04 13:21:23by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Script.hpp"

Script::Script() {}

Script::~Script() {}

void Script::setScriptType(std::string const &cgiPath)
{
	size_t lastDot = cgiPath.find_last_of('.');
	_scriptType	   = cgiPath.substr(lastDot);
	std::cout << RED << "script type = " + _scriptType << RESET << std::endl; // DBG
	if (_scriptType != ".py" && _scriptType != ".php")
	{
		printBoxError("Invalid script type");
		exit(1);
		//TODO implement proper throw/error
	}
}

void Script::runScript(HttpRequest const &request)
{
	_scriptOutput	  = "";
	std::string query = request.getQuery();
	std::cout << RED << std::string(__func__) + " " + query << RESET << std::endl; // DB
	_cgiPath = request.getFullPath().first + request.getFullPath().second;
	setScriptType(_cgiPath);
	setEnv(request);
	int pipeIn[2];
	int pipeOut[2];

	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
	{
		//TODO what error number here?
		printBoxError("Pipe Error");
		_scriptOutput = "";
	}

	pid_t child = fork();

	if (child < 0)
	{
		//TODO also here, what error?
		printBoxError("Fork error");
		close(pipeOut[PIPE_WRITE]);
		close(pipeOut[PIPE_READ]);
		close(pipeIn[PIPE_WRITE]);
		close(pipeIn[PIPE_READ]);
		_scriptOutput = "";
	}

	else if (child == 0)
	{
		std::string runPath;
		close(pipeIn[PIPE_WRITE]);
		dup2(pipeIn[PIPE_READ], STDIN_FILENO);
		close(pipeIn[PIPE_READ]);

		// Redirect script's stdout to write to pipeOut
		close(pipeOut[PIPE_READ]);
		dup2(pipeOut[PIPE_WRITE], STDOUT_FILENO);
		close(pipeOut[PIPE_WRITE]);

		/* TODO wrap env creation
		into its owh function*/

		std::string varQuery  = "QUERY_STRING=" + query;
		std::string varMethod = "REQUEST_METHOD=" + request.getHttpMethod();
		char	   *envQuery  = new char[varQuery.length() + 1];
		strcpy(envQuery, varQuery.c_str());

		char *envMethod = new char[varMethod.length() + 1];
		strcpy(envMethod, varMethod.c_str());

		char *envServ[] = {envQuery, envMethod, NULL};

		if (_scriptType == ".py")
			runPath = "/usr/bin/python3";
		else if (_scriptType == ".php")
			runPath = "/usr/bin/php";
		char *argv[] = {const_cast<char *>(runPath.c_str()), const_cast<char *>(_cgiPath.c_str()),
						NULL};
		execve(runPath.c_str(), argv, envServ);

		std::cerr << "Execve failed for " << request.getFullPath().second << ": " << strerror(errno)
				  << std::endl; // DBG
		delete[] envQuery;
		exit(1);
	}
	else
	{
		close(pipeIn[PIPE_READ]);
		close(pipeOut[PIPE_WRITE]);

		if (!query.empty())
		{
			if ((write(pipeIn[PIPE_WRITE], query.c_str(), query.length())) == -1)
				printBoxError("Error parent writing");
		}
		close(pipeIn[PIPE_WRITE]);

		// Read the script output from pipeOut
		char	buffer[4096];
		ssize_t bytesRead;

		while ((bytesRead = read(pipeOut[PIPE_READ], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytesRead] = '\0';
			_scriptOutput += buffer;
		}
		close(pipeOut[PIPE_READ]);

		int status;
		waitpid(child, &status, 0);

		// Check if child process exited with error
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			std::cerr << "CGI script " << _cgiPath << " exited with code " << WEXITSTATUS(status)
					  << std::endl;
		}
		else if (WIFSIGNALED(status))
		{
			std::cerr << "CGI script " << _cgiPath << " killed by signal " << WTERMSIG(status)
					  << std::endl;
		}
		else
		{
			_contentType = "text/html"; // TODO check for other outputs formats?
			std::cout << GREEN << "CGI script output captured: " << _scriptOutput.length()
					  << " bytes" << RESET << std::endl;
		}
	}
}

std::string Script::getScriptOutput() const
{
	return _scriptOutput;
}

std::string Script::getContentType() const
{
	return _contentType;
}

void Script::setEnv(HttpRequest const &request) // TODO make it return a char**
{
	std::vector<std::string> envVars;

	envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVars.push_back("SERVER_PROTOCOL=" + request.getHttpVersion());
	envVars.push_back("REQUEST_METHOD=" + request.getHttpMethod());
	envVars.push_back("SCRIPT_NAME=" + request.getFullPath().second);
	envVars.push_back("PATH_INFO=" + _cgiPath); // TODO here and above not clear, check subject.
	envVars.push_back("QUERY_STRING=" + request.getQuery());
	envVars.push_back("PATH_TRANSLATED=" + _cgiPath);
	envVars.push_back("REMOTE_ADDR=" + request.getRequestUri()); // TODO get ip address
	envVars.push_back("SERVER_NAME=" + request.getRequestUri()); // TODO get server name
	envVars.push_back("SERVER_PORT=" + request.getRequestUri()); // TODO get port
	
	// TODO get headers for HTTP_ variables
	Headers *reqHeaders = request.getReqHeaders();
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = reqHeaders->getHeaderBegin(); it != reqHeaders->getHeaderEnd(); it++)
	{
		std::string envEntry = "HTTP_" + it->first;
		std::transform(envEntry.begin(), envEntry.end(), envEntry.begin(), ::toupper);
		std::replace(envEntry.begin(), envEntry.end(), '-', '_');
		
		if (!it->second.empty())
			envVars.push_back(envEntry + "=" + it->second[0]);
	}	
	std::cout << RED << "[ENV variables]" << std::endl; // DBG to remove
	for (size_t i = 0; i < envVars.size(); i++)
		std::cout << envVars[i] << std::endl;
	std::cout << RESET << std::endl;
}