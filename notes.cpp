#include "Response.hpp"
#include <fstream>
#include <sstream>
#include <sys/wait.h> // For waitpid
#include <unistd.h>	  // For pipe, fork, dup2, execve, close, write, read
#include <vector>

// Define constants for pipe array indices for better readability.
#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

/**
 * @brief Helper function to build the environment array for execve.
 * @param env_vars A vector of strings, where each string is "KEY=VALUE".
 * @return A dynamically allocated, NULL-terminated array of char pointers.
 * The caller is responsible for deleting the outer vector pointer.
 */
static char **build_environment(const std::vector<std::string> &env_vars)
{
	std::vector<char *> *envp_vector = new std::vector<char *>();
	for (size_t i = 0; i < env_vars.size(); ++i)
	{
		envp_vector->push_back(const_cast<char *>(env_vars[i].c_str()));
	}
	envp_vector->push_back(NULL); // The array must be NULL-terminated.
	return envp_vector->data();
}

void Response::RunScript(const std::string &cgi_path)
{
	int pipe_in[2];	 // Pipe for server to write to script's stdin
	int pipe_out[2]; // Pipe for server to read from script's stdout

	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
	{
		_statusCode = 500; // Internal Server Error
		return;
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		_statusCode = 500;
		return;
	}

	if (pid == 0)
	{ // --- Child Process (The Script) ---
		// Redirect stdin to read from pipe_in
		close(pipe_in[PIPE_WRITE_END]);
		dup2(pipe_in[PIPE_READ_END], STDIN_FILENO);
		close(pipe_in[PIPE_READ_END]);

		// Redirect stdout to write to pipe_out
		close(pipe_out[PIPE_READ_END]);
		dup2(pipe_out[PIPE_WRITE_END], STDOUT_FILENO);
		close(pipe_out[PIPE_WRITE_END]);

		// Build the CGI environment
		std::vector<std::string> cgi_env;
		cgi_env.push_back("REQUEST_METHOD=" + _request.getMethod());
		cgi_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
		if (_request.getMethod() == "POST")
		{
			std::stringstream ss;
			ss << _request.getBody().length();
			cgi_env.push_back("CONTENT_LENGTH=" + ss.str());
			// You can get Content-Type from request headers
			cgi_env.push_back("CONTENT_TYPE=application/x-www-form-urlencoded");
		}

		char **envp	  = build_environment(cgi_env);
		char  *argv[] = {const_cast<char *>(cgi_path.c_str()), NULL};

		execve(cgi_path.c_str(), argv, envp);

		// If execve fails, the child must exit to prevent a fork bomb.
		std::cerr << "Execve failed" << std::endl;
		delete envp; // Clean up on error
		exit(1);
	}
	else
	{ // --- Parent Process (The Server) ---
		close(pipe_in[PIPE_READ_END]);
		close(pipe_out[PIPE_WRITE_END]);

		// Write the request body to the script's stdin.
		if (!_request.getBody().empty())
		{
			if (write(pipe_in[PIPE_WRITE_END], _request.getBody().c_str(),
					  _request.getBody().length()) == -1)
			{
				_statusCode = 500;
			}
		}
		close(pipe_in[PIPE_WRITE_END]); // Send EOF to script

		// Read the script's output from its stdout.
		char		buffer[4096];
		ssize_t		bytes_read;
		std::string cgi_output;
		while ((bytes_read = read(pipe_out[PIPE_READ_END], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			cgi_output.append(buffer, bytes_read);
		}
		close(pipe_out[PIPE_READ_END]);

		waitpid(pid, NULL, 0);

		// TODO: A full implementation must parse CGI headers from the output.
		// For now, we assume the entire output is the body.
		_body		= cgi_output;
		_statusCode = 200;
		std::stringstream ss;
		ss << _body.length();
		_headers["Content-Length"] = ss.str();
		_headers["Content-Type"]   = "text/html"; // Default, should be parsed
	}
}
