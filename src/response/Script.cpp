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
	_scriptOutput.clear();
	_outputBody.clear();
	_outputHeaders.clear();
	_cgiPath.clear();
	_scriptType.clear();
	_scriptOutput = "";

	std::string query = request.getQuery();
	std::cout << RED << std::string(__func__) + " " + query << RESET << std::endl; // DBG
	_cgiPath = request.getFullPath().first + request.getFullPath().second;
	setScriptType(_cgiPath);
	int pipeIn[2];
	int pipeOut[2];

	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
	{
		//TODO what error number here?
		printBoxError("Pipe Error");
		_scriptOutput = "";
	}

	char **envServ = setEnv(request);

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
		deleteArray(envServ);
	}

	else if (child == 0)
	{
		std::string scriptPath = _cgiPath;
		std::string scriptDir;
		std::string scriptFile;

		size_t lastSlash = scriptPath.find_last_of("/");
		if (lastSlash != std::string::npos)
		{
			scriptDir  = scriptPath.substr(0, lastSlash);
			scriptFile = scriptPath.substr(lastSlash + 1);
			if (chdir(scriptDir.c_str()) != 0)
			{
				std::cerr << "Error: chdir failed for " << scriptDir << ": " << strerror(errno)
						  << std::endl;
				exit(1);
			}
		}
		else
		{
			scriptFile = scriptPath;
		}

		std::string runPath;
		close(pipeIn[PIPE_WRITE]);
		dup2(pipeIn[PIPE_READ], STDIN_FILENO);
		close(pipeIn[PIPE_READ]);

		// Redirect script's stdout to write to pipeOut
		close(pipeOut[PIPE_READ]);
		dup2(pipeOut[PIPE_WRITE], STDOUT_FILENO);
		close(pipeOut[PIPE_WRITE]);

		if (_scriptType == ".py")
			runPath = "/usr/bin/python3";
		else if (_scriptType == ".php")
			runPath = "/usr/bin/php-cgi";

		char *argv[] = {const_cast<char *>(runPath.c_str()), const_cast<char *>(scriptFile.c_str()),
						NULL};

		execve(runPath.c_str(), argv, envServ);

		std::cerr << "Execve failed for " << request.getFullPath().second << ": " << strerror(errno)
				  << std::endl; // DBG
		exit(1);
	}
	else
	{
		close(pipeIn[PIPE_READ]);
		close(pipeOut[PIPE_WRITE]);

		if (request.getHttpMethod() == "POST")
		{
			const std::string &requestBody = request.getRawBody();
			if (!requestBody.empty())
			{
				if (write(pipeIn[PIPE_WRITE], requestBody.c_str(), requestBody.length()) == -1)
					printBoxError("Error parent writing POST body");
			}
		}

		close(pipeIn[PIPE_WRITE]);

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
		deleteArray(envServ);

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
			std::cout << GREEN << "CGI script output captured: " << _scriptOutput.length()
					  << " bytes" << RESET << std::endl;
			printRaw(_scriptOutput);
			parseOutput();
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

char **Script::setEnv(HttpRequest const &request) // TODO make it return a char**
{
	std::vector<std::string> envVars;

	envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVars.push_back("REDIRECT_STATUS=200");
	envVars.push_back("SERVER_PROTOCOL=" + request.getHttpVersion());
	envVars.push_back("REQUEST_METHOD=" + request.getHttpMethod());
	envVars.push_back("SCRIPT_NAME=" + request.getFullPath().second);
	envVars.push_back("SCRIPT_FILENAME=" + _cgiPath);
	envVars.push_back("PATH_INFO=" +
					  request.getFullPath().second); // TODO here and above not clear, check subject.
	envVars.push_back("QUERY_STRING=" + request.getQuery());
	envVars.push_back("PATH_TRANSLATED=" + _cgiPath);
	envVars.push_back("REMOTE_ADDR=" + request.getAddrPort().second); // TODO get ip address
	envVars.push_back("SERVER_NAME=" + request.getHost().first);	  // TODO get server name
	envVars.push_back("SERVER_PORT=" + intToString(request.getAddrPort().first)); // TODO get port

	Headers *reqHeaders = request.getReqHeaders();

	std::map<std::string, std::vector<std::string> >::const_iterator it_ct = reqHeaders->getHeader("content-type");
	if (it_ct != reqHeaders->getHeaderEnd())
	{
		const std::vector<std::string>& values = it_ct->second;
		std::string contentTypeValue = values[0]; 
		for (size_t i = 1; i < values.size(); ++i)
		{
			contentTypeValue += ";" + values[i]; 
		}
		envVars.push_back("CONTENT_TYPE=" + contentTypeValue);
	}

if (reqHeaders->getHeader("content-length") != reqHeaders->getHeaderEnd())
    envVars.push_back("CONTENT_LENGTH=" + reqHeaders->getHeaderOnlyOneValue("content-length", 0));

	for (std::map<std::string, std::vector<std::string> >::const_iterator it =
			 reqHeaders->getHeaderBegin();
		 it != reqHeaders->getHeaderEnd(); it++)
	{
		std::string envEntry = "HTTP_" + it->first;
		std::transform(envEntry.begin(), envEntry.end(), envEntry.begin(), ::toupper);
		std::replace(envEntry.begin(), envEntry.end(), '-', '_');

		if (!it->second.empty())
			envVars.push_back(envEntry + "=" + it->second[0]);
	}
	// std::cout << RED << "[ENV variables]" << std::endl; // DBG to remove
	// for (size_t i = 0; i < envVars.size(); i++)
	// 	std::cout << envVars[i] << std::endl;
	// std::cout << RESET << std::endl;

	char **envp = new char *[envVars.size() + 1];
	for (size_t i = 0; i < envVars.size(); i++)
	{
		envp[i] = new char[envVars[i].length() + 1];
		strcpy(envp[i], envVars[i].c_str());
	}
	envp[envVars.size()] = NULL;

	return envp;
}

void Script::parseOutput()
{
	size_t clrfPos = _scriptOutput.find("\r\n\r\n");

	if (clrfPos == std::string::npos)
	{
		_outputBody = _scriptOutput;
		return;
	}

	std::string headers = _scriptOutput.substr(0, clrfPos);
	_outputBody			= _scriptOutput.substr(clrfPos + 4);

	std::stringstream streamHeaders(headers);
	std::string		  headerLine;
	while (std::getline(streamHeaders, headerLine))
	{
		if (!headerLine.empty() && headerLine[headerLine.size() - 1] == '\r')
			headerLine.erase(headerLine.size() - 1);

		size_t colon = headerLine.find(':');
		if (colon != std::string::npos)
		{
			std::string key	  = headerLine.substr(0, colon);
			std::string value = headerLine.substr(colon + 1);

			size_t first = value.find_first_not_of(" \t");
			if (first != std::string::npos)
				value = value.substr(first);

			_outputHeaders[upperKey(key)] = value;
		}
	}
}

std::string Script::getOutputBody() const
{
	return _outputBody;
}

std::map<std::string, std::string> Script::getOutputHeaders() const
{
	return _outputHeaders;
}