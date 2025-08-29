/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Script.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:38:40 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/29 11:46:20 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Script.hpp"

Script::Script() {}

Script::~Script() {}

void	Script::setScriptType(std::string const &cgiPath)
{
	size_t		lastDot	   = cgiPath.find_last_of('.');
	_scriptType = cgiPath.substr(lastDot);
	std::cout << RED << "script type = " + _scriptType << RESET << std::endl; // DBG
	if (_scriptType != ".py" && _scriptType != ".php")
		{
			printBoxError("Invalid script type")
			exit(1);
			//TODO implement proper throw/error
		}
}

void	Script::runScript(std::string const &cgiPath)
{
	std::string query = _request->getQuery();
	std::cout << RED << std::string(__func__) + " " + query << RESET << std::endl; // DBG

	std::string content;

	int pipeIn[2];
	int pipeOut[2];

	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
	{
		//TODO what error number here?
		printBoxError("Pipe Error");
		return "";
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
		return "";
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
		std::string varMethod = "REQUEST_METHOD=" + _request->getHttpMethod();
		char	   *envQuery  = new char[varQuery.length() + 1];
		strcpy(envQuery, varQuery.c_str());

		char *envMethod = new char[varMethod.length() + 1];
		strcpy(envMethod, varMethod.c_str());

		char *envServ[] = {envQuery, envMethod, NULL};

		if (_scriptType == ".py")
			runPath = "/usr/bin/python3";
		else if (_scriptType == ".php")
			runPath = "/usr/bin/php";
		char *argv[] = {const_cast<char *>(runPath.c_str()), const_cast<char *>(cgiPath.c_str()),
						NULL};
		execve(runPath.c_str(), argv, envServ);

		std::cerr << "Execve failed for " << cgiPath << ": " << strerror(errno) << std::endl; // DBG
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
		std::string scriptOutput;
		char		buffer[4096];
		ssize_t		bytesRead;

		while ((bytesRead = read(pipeOut[PIPE_READ], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytesRead] = '\0';
			scriptOutput += buffer;
		}
		close(pipeOut[PIPE_READ]);

		int status;
		waitpid(child, &status, 0);

		// Check if child process exited with error
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			std::cerr << "CGI script " << cgiPath << " exited with code " << WEXITSTATUS(status)
					  << std::endl;
		}
		else if (WIFSIGNALED(status))
		{
			std::cerr << "CGI script " << cgiPath << " killed by signal " << WTERMSIG(status)
					  << std::endl;
		}
		else
		{
			content		 = _scriptOutput;
			_contentType = "text/html";
			std::cout << GREEN << "CGI script output captured: " << scriptOutput.length()
					  << " bytes" << RESET << std::endl;
		}
	}
	return content;
}