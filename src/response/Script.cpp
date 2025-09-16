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
#include "ServerManager.hpp"
#include <sys/time.h>

Script::Script() {}

Script::~Script() {}

bool Script::setScriptType(std::string const &cgiPath)
{
		size_t lastDot = cgiPath.find_last_of('.');
		if (lastDot == std::string::npos)
		{
				_statusCode = 501;
				return false;
		}
		_scriptType = cgiPath.substr(lastDot);
		if (_scriptType != ".py" && _scriptType != ".php")
		{
				printBoxError("Invalid script type");
				_statusCode = 501;
				return false;
		}
		return true;
}

void Script::runScript(HttpRequest &request, std::string const &interpreterPath,
					   ServerManager &server)
{
		_scriptOutput.clear();
		_outputBody.clear();
		_outputHeaders.clear();
		_cgiPath.clear();
		_scriptType.clear();
		_scriptOutput = "";

		std::string query = request.getQuery();
		_cgiPath = request.getFullPath().first + request.getFullPath().second;
		if (!setScriptType(_cgiPath))
				return;
		int pipeIn[2];
		int pipeOut[2];

		if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
		{
				printBoxError("Pipe Error");
				request.setStatusCode(E_500);
				_scriptOutput = "";
				return;
		}

		char **envServ = setEnv(request, server);

		pid_t child = fork();

		if (child < 0)
		{
				printBoxError("Fork error");
				close(pipeOut[PIPE_WRITE]);
				close(pipeOut[PIPE_READ]);
				close(pipeIn[PIPE_WRITE]);
				close(pipeIn[PIPE_READ]);
				request.setStatusCode(E_500);
				_scriptOutput = "";
				deleteArray(envServ);
				return;
		} else if (child == 0)
		{
				std::string scriptPath = _cgiPath;
				std::string scriptDir;
				std::string scriptFile;

				size_t lastSlash = scriptPath.find_last_of("/");
				if (lastSlash != std::string::npos)
				{
						scriptDir = scriptPath.substr(0, lastSlash);
						scriptFile = scriptPath.substr(lastSlash + 1);
						if (chdir(scriptDir.c_str()) != 0) // HERE 9
						{
								printBoxError("chdir failed");
								exit(1);
						}
				} else
				{
						scriptFile = scriptPath;
				}

				close(pipeIn[PIPE_WRITE]);
				dup2(pipeIn[PIPE_READ], STDIN_FILENO);
				close(pipeIn[PIPE_READ]);

				close(pipeOut[PIPE_READ]);
				dup2(pipeOut[PIPE_WRITE], STDOUT_FILENO);
				dup2(pipeOut[PIPE_WRITE], STDERR_FILENO);
				close(pipeOut[PIPE_WRITE]);

				char *argv[] = {const_cast<char *>(interpreterPath.c_str()),
								const_cast<char *>(scriptFile.c_str()), NULL};
				execve(interpreterPath.c_str(), argv, envServ);
				exit(1);
		} else
		{
				close(pipeIn[PIPE_READ]);
				close(pipeOut[PIPE_WRITE]);

				if (request.getHttpMethod() == "POST")
				{
						const std::string &requestBody = request.getRawBody();
						if (!requestBody.empty())
						{
								ssize_t written = write(pipeIn[PIPE_WRITE],
														requestBody.c_str(),
														requestBody.length());
								if (written == -1)
										printBoxError(
											"Error parent writing POST body");
								else if (written < static_cast<ssize_t>(
													   requestBody.length()))
										printBoxError("Partial write occurred");
						}
				}

				close(pipeIn[PIPE_WRITE]);

				struct timeval start_time, current_time;
				gettimeofday(&start_time, NULL);

				int status;
				bool timeCheck = false;

				while (true)
				{
						pid_t result = waitpid(child, &status, WNOHANG);

						if (result == child)
						{
								break;
						} else if (result == -1)
						{
								printBoxError("waitpid error");
								kill(child, SIGKILL);
								waitpid(child, NULL, 0);
								request.setStatusCode(E_500);
								deleteArray(envServ);
								return;
						}

						gettimeofday(&current_time, NULL);
						long elapsed =
							(current_time.tv_sec - start_time.tv_sec);

						if (elapsed >= SCRIPT_TIMEOUT)
						{
								timeCheck = true;
								break;
						}
				}

				if (timeCheck)
				{
						kill(child, SIGKILL);
						waitpid(child, &status, 0);
						printBoxError("Script timeout");
						_statusCode = 504;
				} else
				{
						char buffer[4096];
						ssize_t bytesRead;
						while ((bytesRead = read(pipeOut[PIPE_READ], buffer,
												 sizeof(buffer) - 1)) > 0)
						{
								buffer[bytesRead] = '\0';
								_scriptOutput += buffer;
						}
						if (bytesRead == -1)
								printBoxError("Read error occurred");

						if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
						{
								std::cerr << "CGI script " << _cgiPath
										  << " exited with code "
										  << WEXITSTATUS(status) << std::endl;
								_statusCode = 500;
						} else if (WIFSIGNALED(status))
						{
								std::cerr << "CGI script " << _cgiPath
										  << " killed by signal "
										  << WTERMSIG(status) << std::endl;
								_statusCode = 500;
						} else
						{
								std::cout << GREEN
										  << "CGI script output captured: "
										  << _scriptOutput.length() << " bytes"
										  << RESET << std::endl;
								_statusCode = 200;
								parseOutput();
						}
				}

				close(pipeOut[PIPE_READ]);
				deleteArray(envServ);
		}
}

std::string Script::getScriptOutput() const { return _scriptOutput; }

std::string Script::getContentType() const { return _contentType; }

char **Script::setEnv(HttpRequest const &request, ServerManager &serverManager)
{
		std::vector<std::string> envVars;

		envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
		envVars.push_back("REDIRECT_STATUS=200");
		envVars.push_back("SERVER_PROTOCOL=" + request.getHttpVersion());
		envVars.push_back("REQUEST_METHOD=" + request.getHttpMethod());
		envVars.push_back("SCRIPT_NAME=" + request.getFullPath().second);
		envVars.push_back("SCRIPT_FILENAME=" + _cgiPath);
		envVars.push_back("PATH_INFO=" + request.getFullPath().second);
		envVars.push_back("QUERY_STRING=" + request.getQuery());
		envVars.push_back("PATH_TRANSLATED=" + _cgiPath);
		envVars.push_back("REMOTE_ADDR=" + request.getAddrPort().second);
		envVars.push_back("SERVER_NAME=" + request.getHost().first);
		envVars.push_back("SERVER_PORT=" +
						  intToString(request.getAddrPort().first));

		Headers *reqHeaders = request.getReqHeaders();
		std::map<std::string, std::string> httpEnvVars;

		for (std::map<std::string, std::vector<std::string>>::const_iterator
				 it = reqHeaders->getHeaderBegin();
			 it != reqHeaders->getHeaderEnd(); it++)
		{
				std::string envEntry = "HTTP_" + it->first;
				std::transform(envEntry.begin(), envEntry.end(),
							   envEntry.begin(), ::toupper);
				std::replace(envEntry.begin(), envEntry.end(), '-', '_');

				if (!it->second.empty())
						httpEnvVars[envEntry] = it->second[0];
		}

		std::string scriptName = request.getFullPath().second;

		if (scriptName == "/cgi-bin/login.py")
		{
				std::string username =
					getQueryValue(request.getQuery(), "username");
				if (username.empty())
						username = "guest";
				std::string sessionId = serverManager.createSession(username);
				envVars.push_back("WEBSERV_SESSION_ID=" + sessionId);
		} else if (scriptName == "/cgi-bin/profile.py")
		{
				if (httpEnvVars.count("HTTP_COOKIE"))
				{
						const std::string &cookieValue =
							httpEnvVars["HTTP_COOKIE"];
						std::string sessionId =
							getCookieValue(cookieValue, "session_id");

						CookieData *session =
							serverManager.getSession(sessionId);
						if (session)
						{
								envVars.push_back("WEBSERV_USERNAME=" +
												  session->username);
						}
				}
		}

		if (reqHeaders->getHeader("content-length") !=
			reqHeaders->getHeaderEnd())
				envVars.push_back(
					"CONTENT_LENGTH=" +
					reqHeaders->getHeaderOnlyOneValue("content-length", 0));

		std::map<std::string, std::vector<std::string>>::const_iterator it_ct =
			reqHeaders->getHeader("content-type");
		if (it_ct != reqHeaders->getHeaderEnd())
		{
				const std::vector<std::string> &values = it_ct->second;
				std::string contentTypeValue = values[0];
				for (size_t i = 1; i < values.size(); ++i)
				{
						contentTypeValue += ";" + values[i];
				}
				envVars.push_back("CONTENT_TYPE=" + contentTypeValue);
		}

		for (std::map<std::string, std::string>::const_iterator it =
				 httpEnvVars.begin();
			 it != httpEnvVars.end(); ++it)
		{
				envVars.push_back(it->first + "=" + it->second);
		}

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
		size_t separatorPos = _scriptOutput.find("\r\n\r\n");
		size_t separatorLen = 4;

		if (separatorPos == std::string::npos)
		{
				separatorPos = _scriptOutput.find("\n\n");
				separatorLen = 2;
		}

		std::string headersPart;
		if (separatorPos != std::string::npos)
		{
				headersPart = _scriptOutput.substr(0, separatorPos);
				_outputBody = _scriptOutput.substr(separatorPos + separatorLen);
		} else
		{
				_outputBody = _scriptOutput;
				return;
		}

		std::stringstream ss(headersPart);
		std::string line;
		while (std::getline(ss, line))
		{
				if (!line.empty() && line[line.size() - 1] == '\r')
						line.erase(line.size() - 1);

				size_t colonPos = line.find(':');
				if (colonPos != std::string::npos)
				{
						std::string key = upperKey(line.substr(0, colonPos));
						std::string value = line.substr(colonPos + 1);

						size_t firstChar = value.find_first_not_of(" \t");
						if (firstChar != std::string::npos)
								value = value.substr(firstChar);

						_outputHeaders[key] = value;

						if (key == "STATUS")
						{
								std::istringstream statusStream(value);
								int statusCode;
								if (statusStream >> statusCode)
								{
										_statusCode = statusCode;
								}
						}
				}
		}
}

std::string Script::getOutputBody() const { return _outputBody; }

std::map<std::string, std::string> Script::getOutputHeaders() const
{
		return _outputHeaders;
}

int Script::getStatusCode() const { return _statusCode; }