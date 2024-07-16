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
#include "../CGI/Cgi.hpp"

CGI::CGI(std::map<std::string, std::string> envi)
{
    this->_env = envi;
    _env["SERVER_NAME"] = "server";
    _env["SERVER_PORT"] = "8081";
    _env["REQUEST_METHOD"] = "GET";
    _env["PATH_INFO"] = "/Users/ayakoubi/Desktop/webserv/src/CGI";
    _env["SCRIPT_NAME"] = "/script.sh";
    _env["QUERY_STRING"] = "name=value1&password=value2";
    _env["CONTENT_TYPE"] = "text/html;";
    _env["CONTENT_LENGTH"] = "1";

}

char** CGI::set_env()
{
    char **env = new char*[_env.size() + 1];
    size_t i = 0;
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it)
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
std::string CGI::check_extension_file(const std::string &file)
{
    std::string extension;
    std::string formats[] = {"php", "sh", "cpp", "python"};
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

void CGI::exec_cpp(const std::string &path, char **env)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        // std::cerr << "Failed to fork." << std::endl;
        return;
    }
    else if (pid == 0)
    {
        const char* path_cpp = "/usr/bin/c++";
        const char* args[] = { "c++", path.c_str(), "-o", "a.out", NULL };
        execve(path_cpp, (char* const*)args, env);
        // perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            // std::cerr << "Error: failed to compile" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
bool CGI::exec_cgi()
{
    std::string output;
    std::string path = _env["PATH_INFO"] + _env["SCRIPT_NAME"];

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        return false;
    }
    char **env = set_env();
    pid_t pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
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
        std::string ext = check_extension_file(path);
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
        // perror("execve");
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
        {
            return false;
        }
    }
    this->output = output; 
    // std::cout << "output::" << output << std::endl;
    return true;
}

std::string CGI::getCgiOutput()
{
    return this->output;
}
// int main(int ac, char **av)
// {
//     std::map<std::string, std::string> env;
//     CGI cgi(env);
//     // cgi.set_env();
//     cgi.exec_cgi();
// }