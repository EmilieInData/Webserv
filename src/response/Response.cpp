/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/08 12:30:06 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "HttpRequest.hpp"

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

Response::Response(HttpRequest const &request) : _request(&request)
{

	_clientFd	   = -1;
	_response	   = "";
	_location	   = "";
	_method		   = "";
	_contentType   = "";
	_contentLength = "";
}

Response::~Response() {}

Response::Response(Response const &copy)
	: _clientFd(copy._clientFd), _response(copy._response), _location(copy._location),
	  _method(copy._method), _contentLength(copy._contentLength)
{
	_output << copy._output.str();
}

void Response::setResponse(std::string response)
{
	_response = response;
}

void Response::setContent(std::pair<std::string, std::string> fullPath, std::string method)
{
	if (fullPath.second == "/" || fullPath.second.empty())
		_location = fullPath.first + "/index.html"; // TODO check AutoIndex
	else
		_location = fullPath.first + fullPath.second;

	_method = method;
}

void Response::setClientFd(int clientFd)
{
	_clientFd = clientFd;
}

std::string Response::prepFile()
{
	// Check if it's a binary file (image)

	if (isBinary(_location))
	{
		std::ifstream file(_location.c_str(), std::ios::binary);
		if (!file.is_open())
			return "";

		std::ostringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	else
	{
		std::ifstream page(_location.c_str());
		if (!page.is_open())
			return "";

		std::ostringstream pageContent;
		pageContent << page.rdbuf();
		page.close();
		return pageContent.str();
	}
}

void Response::runScript(std::string const &cgiPath)
{
	std::string query = _request->getQuery();
	std::cout << RED << std::string(__func__) + " " + query << RESET << std::endl; // DBG

	int pipeIn[2];
	int pipeOut[2];

	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
	{
		//TODO what error number here?
		printBoxError("Pipe Error");
		return;
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
		return;
	}

	/* TODO to implement more types of script,
	create option for selecting the kind of 
	script detected (here for example it's
	python)*/

	else if (child == 0)
	{
		close(pipeIn[PIPE_WRITE]);
		dup2(pipeIn[PIPE_READ], STDIN_FILENO);
		close(pipeIn[PIPE_READ]);

		// Redirect script's stdout to write to pipeOut
		close(pipeOut[PIPE_READ]);
		dup2(pipeOut[PIPE_WRITE], STDOUT_FILENO);
		close(pipeOut[PIPE_WRITE]);

		std::string envVar = "QUERY_STRING=" +
							 query; // TODO should pass everything the script might need in form of variable
		char *envChar = new char[envVar.length() + 1];
		strcpy(envChar, envVar.c_str());

		char *envServ[] = {envChar, NULL};
		char *argv[] = {const_cast<char *>("/usr/bin/python3"), const_cast<char *>(cgiPath.c_str()),
						NULL};
		execve("/usr/bin/python3", argv, envServ);

		std::cerr << "Execve failed for " << cgiPath << ": " << strerror(errno) << std::endl; // DBG
		delete[] envChar;
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
			// Script executed successfully, set the response with the output
			_response = scriptOutput;
			std::cout << GREEN << "CGI script output captured: " << scriptOutput.length()
					  << " bytes" << RESET << std::endl;
		}
	}
}

std::string Response::checkType()
{
	std::string extension;
	size_t		dotPos = _location.find_last_of('.');
	if (dotPos != std::string::npos)
		extension = _location.substr(dotPos);

	if (extension == ".html" || extension == ".htm")
		return "text/html";
	else if (extension == ".jpg" || extension == ".jpeg")
		return "image/jpeg";
	else if (extension == ".png")
		return "image/png";
	else if (extension == ".gif")
		return "image/gif";
	else if (extension == ".css")
		return "text/css";
	else if (extension == ".js")
		return "text/javascript";
	else if (extension == ".py")
		return "text/x-python";
	else
		return "application/octet-stream";
}

void Response::prepResponse()
{
	std::string		   content = prepFile();
	std::ostringstream output;
	output << content.length();
	_contentLength = output.str();
	_contentType   = checkType();

	Header header(*this);
	if (_contentType == "text/x-python")
	{
		runScript(_location);
	}
	/* TODO the response here is hardcoded
	and for now it's just for testing */
	else
		_response = header.getHeader() + content;
}

void Response::printRawResponse()
{
	std::cout << PINK << "[RAW RESPONSE]" << RESET << std::endl;
	for (size_t i = 0; i < _response.size(); i++)
	{
		if (_response[i] == '\r')
			std::cout << "\\r";
		else if (_response[i] == '\n')
			std::cout << "\\n\n";
		else
			std::cout << _response[i];
	}
	std::cout << PINK << "\n[END OF RESPONSE]" << RESET << std::endl;
}

void Response::sendResponse()
{
	prepResponse();

	size_t		totalSent	= 0;
	size_t		totalSize	= _response.size();
	const char *data		= _response.c_str();
	int			attempts	= 0;
	const int	maxAttempts = 100;

	while (totalSent < totalSize && attempts < maxAttempts)
	{

		ssize_t sent = send(_clientFd, data + totalSent, totalSize - totalSent, 0);
		if (sent > 0)
			totalSent += sent;
		else if (sent == 0)
		{
			std::cerr << "Connection closed by client during send" << std::endl;
			break;
		}
		else
		{
			usleep(1000);
			attempts++;
		}
	}

	if (totalSent != totalSize)
		printBoxError("Warning, send incomplete");
}

std::string Response::getType()
{
	return _contentType;
}

std::string Response::getLength()
{
	return _contentLength;
}

std::string Response::getResponse()
{
	return _response;
}
