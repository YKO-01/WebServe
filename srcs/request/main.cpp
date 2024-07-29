#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <sys/select.h> // select()
#define MAX 8000
#define PORT 8080
#define SA struct sockaddr
#include "HTTPRequest.hpp"

char buffer[MAX] = {0};

int main(int ac, char **av)
{
    Config configs;
    int i;
    HTTPParser *parser;

    int sockfd, connfd, len, opt = 1;
    struct sockaddr_in servaddr, cli;
    fd_set readfds;
    int max_sd, sd, activity, new_socket, valread;
    std::vector<int> client_sockets;

    (void)ac;
    configs.parssConfigs((char **)av);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        perror("setsockopt");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli);
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    max_sd = sockfd;

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        for (auto &client_socket : client_sockets)
        {
            FD_SET(client_socket, &readfds);
            if (client_socket > max_sd)
                max_sd = client_socket;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &readfds))
        {
            new_socket = accept(sockfd, (SA *)&cli, (socklen_t *)&len);
            if (new_socket < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            else
            {
                //printf("Server accepted the client...\n");
                client_sockets.push_back(new_socket);
            }
        }

        for (auto it = client_sockets.begin(); it != client_sockets.end();)
        {
            sd = *it;
            if (FD_ISSET(sd, &readfds))
            {
                if ((valread = read(sd, buffer, MAX)) == 0)
                {
                    close(sd);
                    it = client_sockets.erase(it);
                    continue;
                }
                else
                {
                    std::string s(buffer);
                    bzero(buffer, sizeof(buffer));
                    std::string request, body;
                    std::size_t pos = s.find("\r\n\r\n");
                    if (pos != std::string::npos)
                    {
                        request = s.substr(0, pos + 4);
                        body = s.substr(pos + 4, s.length());
                    }
                    else
                        request = s;
					//write(1, request.c_str(), request.length());
                    parser = new HTTPParser(request);
					parser->setBody(body);
                    parser->setConfig(configs.get_allInfo()[0]);
                    HTTPRequest req(parser);
                    req.processRequest();
					write(sd, req.getResponse().c_str(), req.getResponse().length());
					//write(1, "-----------------------------------------\n", 42);
                    delete parser;
                }
            }
            ++it;
        }
    }
    close(sockfd);
    return 0;
}

