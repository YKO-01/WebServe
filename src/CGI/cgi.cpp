#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string>
#include<fstream>
#include<vector>
#include<cstddef>
#include<sstream>
// #include "../config/Config.hpp"
#include"../../incs/Cgi.hpp"
// #include "../CGI/Cgi.hpp"
void CGI::set_output(const std::string &output)
{
    this->outpute = output;

}
CGI::CGI(std::map<std::string, std::string> envi)
{
    this->_env = envi;
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
// bool CGI::exec_cgi()
// {
//     std::string output;
//     _env["PATH_INFO"] = "/Users/hkasbaou/Desktop/WebServeTeam/src/CGI";
//      _env["SCRIPT_NAME"] = "/script.php";
//     std::string path = _env["PATH_INFO"] + _env["SCRIPT_NAME"];
//     int pipefd[2];
//     if (pipe(pipefd) == -1)
//     {
//         return false;
//     }
//     char **env = set_env();
//     pid_t pid = fork();
//     if (pid == -1)
//     {
//         close(pipefd[0]);
//         close(pipefd[1]);
//         return false;
//     }
//     else if (pid == 0)
//     {
//         close(pipefd[0]);
//         if (dup2(pipefd[1], STDOUT_FILENO) == -1)
//         {
//             close(pipefd[1]);
//             exit(EXIT_FAILURE);
//         }
//         close(pipefd[1]);
//         std::string ext = check_extension_file(path);
//         if (ext == "cpp")
//         {
//             exec_cpp(path, env);
//             char *args[] = { (char *)"./a.out", NULL };
//             execve(args[0], args, env);
//         }
//         else
//         {
//             char *args[] = { (char *)path.c_str(), NULL };
//             execve(args[0], args, env);
//         }
//         // perror("execve");
//         exit(EXIT_FAILURE);
//     }
//     else
//     {
//         time_t start_time = time(NULL);
//         std::cout << "start time::" <<difftime(time(NULL), start_time) << std::endl;
//         const int timeout = 1; 
//         close(pipefd[1]);
//         char buffer[1024];
//         int bytesRead;

//         fd_set readfds;
//         FD_ZERO(&readfds);
//         FD_SET(pipefd[0], &readfds);

//         struct timeval tv;
//         tv.tv_sec = timeout;
//         tv.tv_usec = 0;
//         int result = select(pipefd[0] + 1, &readfds, NULL, NULL, &tv);
//         std::cout << "trdult:" << res << std::endl;
//         while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
//         {
//             buffer[bytesRead] = '\0';
//             output += buffer;
//             if (difftime(time(NULL), start_time) >= timeout)
//             {
//                 std::cerr << "Execution timeout" << std::endl;
//                 kill(pid, SIGKILL);
//                 close(pipefd[0]);
//                 return false;
//             }
//         }
//         close(pipefd[0]);
//         int status;
//         waitpid(pid, &status, 0);
//         if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
//         {
//             return false;
//         }
//     }
//     set_output(output);
//     std::cout << "output::" << output << std::endl;
//     return true;
// }

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#include <ctime>

bool CGI::exec_cgi() {
    std::string output;
    _env["PATH_INFO"] = "/Users/hkasbaou/Desktop/WebServeTeam/src/CGI";
    _env["SCRIPT_NAME"] = "/script.php";
    std::string path = _env["PATH_INFO"] + _env["SCRIPT_NAME"];
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return false;
    }
    char **env = set_env();
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    } else if (pid == 0) {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }
        std::string ext = check_extension_file(path);
        if (ext == "cpp") {
            exec_cpp(path, env);
            char *args[] = {(char *)"./a.out", NULL};
            execve(args[0], args, env);
        } else {
            char *args[] = {(char *)path.c_str(), NULL};
            execve(args[0], args, env);
        }
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    } else {
        time_t start_time = time(NULL);
        const int timeout = 4;
        close(pipefd[1]);

        fd_set readfds;
        // FD_ZERO(&readfds);
        // FD_SET(pipefd[0], &readfds);

        struct timeval tv;
        // tv.tv_sec = timeout;
        // tv.tv_usec = 0;

        char buffer[1024];
        int bytesRead;
        while (true) {
            FD_ZERO(&readfds);
            FD_SET(pipefd[0], &readfds);
            tv.tv_sec = timeout;
            tv.tv_usec = 0;

            int result = select(pipefd[0] + 1, &readfds, NULL, NULL, &tv);
            if (result == -1) {
                close(pipefd[0]);
                return false;
            } else if (result == 0) {
                std::cerr << "Execution timeout" << std::endl;
                kill(pid, SIGKILL);
                close(pipefd[0]);
                waitpid(pid, NULL, 0); // Clean up the zombie process
                return false;
            } else {
                if (FD_ISSET(pipefd[0], &readfds)) {
                    bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1);
                    if (bytesRead > 0) {
                        buffer[bytesRead] = '\0';
                        output += buffer;
                    } else {
                        break; // No more data to read
                    }
                }
            }
        }

        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            return false;
        }
    }

    set_output(output);
    std::cout << "output::" << output << std::endl;
    return true;
}

std::string CGI::get_output()
{
    return this->outpute;
}
int main(int ac, char **av)
{
    std::map<std::string, std::string> env;
    CGI cgi(env);
    cgi.exec_cgi();
}