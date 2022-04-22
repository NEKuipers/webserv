#include "CGIRunner.hpp"
#include "PipeException.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include "Response.hpp"

#define BUFFER_SIZE 10

static void UpdateEnv(const std::map<std::string, std::string>& ExtraEnv)
{
	for (std::map<std::string, std::string>::const_iterator It = ExtraEnv.begin(); It != ExtraEnv.end(); It++)
	{
		//std::cout << "env[" << It->first << "] = " << It->second << ";" << std::endl;
		setenv(It->first.c_str(), It->second.c_str(), 1);
	}
}

CGIRunner::CGIRunner(const std::string& PathName, const std::map<std::string, std::string>& ExtraEnv) : InputFD(-1), OutputFD(-1), CGIPid(-1)
{
	// pipes[0] = read
	int pipes_stdin[2];
	int pipes_stdout[2];
	if (pipe(pipes_stdin) == -1)
		throw PipeException();
	if (pipe(pipes_stdout) == -1)
	{
		close(pipes_stdin[0]);
		close(pipes_stdin[1]);
		throw PipeException();
	}

	const_cast<pid_t&>(CGIPid) = fork();
	if (CGIPid == 0)
	{
		UpdateEnv(ExtraEnv);

		// child
		dup2(pipes_stdin[0], STDIN_FILENO);
		dup2(pipes_stdout[1], STDOUT_FILENO);
		// Close all the pipes that we dont need anymore

		//close(pipes_stdin[0]);
		//close(pipes_stdin[1]);
		//close(pipes_stdout[0]);
		//close(pipes_stdout[1]);

		// Close every fd other than stdin/stdout/stderr
		for (int i = 3; i < FD_SETSIZE; i++)	// NOTE: Non terrible way to do this
			close(i);


		const char* Args[2];
		Args[0] = PathName.c_str();
		Args[1] = NULL;
		execv(PathName.c_str(), const_cast<char *const *>(Args));	// I mean, i COULD make a env string, but duplicates and there is no %100 to list all env variables, only getenv is in the standard, so just set it instead

		// TODO: Make it be a 500 response
		std::string Message = "Content-Type: text/html\n\n<!DOCTYPE html><html><p style=\"text-align:center;font-size:200%;\"><a href=\"/\">Webserv</a><br><br><b>500 Internal Server Error<br>CGI failed to run!</b><br><p style=\"line-height: 5000em;text-align:right\"><b>h</b></div></p></html>";
		std::cout << Message << std::endl;
		exit(1);
	}

	close(pipes_stdin[0]);
	close(pipes_stdout[1]);

	const_cast<int&>(InputFD) = pipes_stdin[1];
	const_cast<int&>(OutputFD) = pipes_stdout[0];
}

CGIRunner::~CGIRunner()
{
	close(InputFD);
	close(OutputFD);
}

std::ostream& operator<<(std::ostream& Stream, const CGIRunner& CGIRunner)
{
	Stream << "CGI Runner, InFD: " << CGIRunner.InputFD << " OutFD: " << CGIRunner.OutputFD;
	return Stream;
}
