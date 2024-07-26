#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string>
#include<fstream>
#include<vector>
#include<cstddef>
#include<sstream>
#include "../config/Config.hpp"
#include "Cgi.hpp"
#include "../request/utils/Utils.hpp"

CGI::CGI(std::map<std::string, std::string> env) : env(env)
{
}

std::string	CGI::getCgiOutput() const
{
	return (output);
}

char** CGI::set_env()
{
    char **env = new char*[this->env.size() + 1];
    size_t i = 0;
    for (std::map<std::string, std::string>::iterator it = this->env.begin(); it != this->env.end(); ++it)
    {
        std::string env_var = it->first + "=" + it->second;
        env[i] = new char[env_var.size() + 1];
        std::copy(env_var.begin(), env_var.end(), env[i]);
        env[i][env_var.size()] = '\0';
        i++;
    }
    env[i] = NULL;
    return env;
}

std::string	CGI::get_cgi_extension(const std::string &file)
{
    std::string extension;
    std::string formats[] = {"php", "sh", "cpp", "py"};
    size_t found = file.find_last_of(".");
    if (found != std::string::npos)
    {
        extension = file.substr(found + 1);
        for (size_t i = 0; i < formats->size() ; i++)
        {
            if (extension == formats[i])
                return extension;
        }
    }
    return "";
}

std::string split_equal(const std::string &str)
{
    size_t pos = str.find("=");
    if (pos != std::string::npos)
        return str.substr(pos + 1);
    return "";
}

std::vector<std::pair<std::string, std::string> > CGI::get_cgi_heahers()
{
	return (headers);
}

void CGI::exec_cpp(const std::string &path, char **env)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Failed to fork." << std::endl;
        return;
    }
    else if (pid == 0)
    {
        const char* path_cpp = "/usr/bin/c++";
        const char* args[] = { "c++", path.c_str(), "-o", "a.out", NULL };
        execve(path_cpp, (char* const*)args, env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "Error: failed to compile" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

bool	CGI::exec_cgi()
{
	std::pair<String, String> header;
	std::pair<Iterator, String> temp;
    std::string path = env["PATH_INFO"] + env["SCRIPT_NAME"];

    int pipefd[2];
    if (pipe(pipefd) == -1)
        return (false);
    char **env = set_env();
    pid_t pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (false);
    }
    else if (pid == 0)
    {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        std::string ext = get_cgi_extension(path);
        if (ext == "cpp")
        {
            exec_cpp(path, env);
            char *args[] = { (char *)"./a.out", NULL };
            execve(args[0], args, env);
        }
        else
        {
            char *args[] = { (char *)path.c_str(), NULL };
            execve(args[0], args, env);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipefd[1]);
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';
            output += buffer;
        }
        close(pipefd[0]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            return false;
    }
	temp = Utils::parseToken(output.begin(), output.end(), "\n");
	while (!temp.second.empty())
	{
		if (temp.second.find(":") != std::string::npos)
		{
			header.first = temp.second.substr(0, temp.second.find(":"));
			header.second = temp.second.substr(temp.second.find(":") + 2, temp.second.length());
		}
		headers.push_back(header);
		header.first.clear();
		header.second.clear();
		temp = Utils::parseToken(temp.first, output.end(), "\n");
	}
	output = String(temp.first, output.end());
    return true;
}
