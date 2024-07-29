/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/07/29 22:29:10 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPServer.hpp"
#include <netdb.h>
#include <string>

// __ Constructor & Destructor _________________________________________________
// =============================================================================
TCPServer::TCPServer():serverSD(-1), fdMax(-1)
{
	FD_ZERO(&FDs);
}

TCPServer::TCPServer(Config &configs):serverSD(-1), fdMax(-1), configs(configs.get_allInfo())
{
	FD_ZERO(&FDs);
}

TCPServer::~TCPServer()
{
	size_t i;
	std::cout << "server is closed" << std::endl;
	for (i = 0; i < serverSockets.size(); i++)
		close(serverSockets[i]);
}

// __ Init Socket  _____________________________________________________________
// =============================================================================
bool	TCPServer::initSocket()
{
	size_t i = -1;
	int count = 0;
	while (++i < configs.size())
	{
		// create a socket for the server
		struct addrinfo hints, *res;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		getaddrinfo(configs[i].get_host().c_str(), std::to_string(configs[i].get_port()).c_str(), &hints, &res);
		serverSD = socket(res->ai_family, res->ai_socktype, 0);
		if (serverSD < 0)
		{
			std::cout << "failed to creation a socket" << std::endl;
			continue;
		}
		// set socket option
		int	option = TRUE; 	
		setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));  
		// bind this socket to a specific port number
		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = ((sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
		serverAddress.sin_port = htons(configs[i].get_port());
		if (bind(serverSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		{
			std::cout << "binding failed" << std::endl;
			close(serverSD);
			continue ;
		}
		// listen to the client connection request
		if (listen(serverSD, SOMAXCONN) < 0)
		{
			std::cout << "failed to listening" << std::endl;
			close(serverSD);
			continue ;
		}
		FD_SET(serverSD, &FDs);
    	fdMax = serverSD;
		serverSockets.push_back(serverSD);
		count++;
	}
	if (count)
		return (true);
	return (false);
}

// __ Accept Connection ________________________________________________________
// =============================================================================
bool TCPServer::acceptConnection(int serverSD, fd_set *FDSRead)
{
	(void) FDSRead;
	struct sockaddr_in conClientAdd;
	int fdClient;
	socklen_t clientAddLength = sizeof(conClientAdd);

	memset(&conClientAdd, 0, sizeof(conClientAdd));
	fdClient = accept(serverSD, (struct sockaddr*)&conClientAdd, &clientAddLength);
	if (fdClient < 0)
		return (false);
	if (!TCPUtils::setNonBlocking(fdClient))
	{
		close(fdClient);
		return (false);
	}
	struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
	if (setsockopt(fdClient, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0) {
        std::cerr << "Error setting timeout\n";
        exit(1);
    }
    if (setsockopt(fdClient, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) < 0) {
        std::cerr << "Error setting timeout\n";
        exit(1);
    }
	FD_SET(fdClient, &FDs);
	if (fdMax < fdClient)
		fdMax = fdClient;
	clients[fdClient] = Client();
	std::cout << GREEN << BOLD << "client with id : " << fdClient << " is connected" << RESET << std::endl;
	return (true);
}

// __ Exist Socket _____________________________________________________________
// =============================================================================
int		TCPServer::existSocket(int sock)
{
	size_t i;

	for (i = 0; i < serverSockets.size(); i++)
	{
		if (sock == serverSockets[i])
			return (serverSockets[i]);
	}
	return (0);
}

void printVector(std::vector<std::string> vec)
{
	for (size_t i = 0; i < vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << std::endl;
}

// __ Get Config Client ________________________________________________________
// =============================================================================
Config	&TCPServer::getConfigClient(int sock)
{
	struct sockaddr_in localAddr;
	socklen_t addLen = sizeof(localAddr);

	getsockname(sock, (struct sockaddr*)&localAddr, &addLen);
	int port = ntohs(localAddr.sin_port);
	long host = ntohl(localAddr.sin_addr.s_addr);
	String hostname = (*clients[sock].getHTTPParser())["host"];
	hostname = hostname.substr(0, hostname.find(":"));
	std::vector<Config>::iterator it = configs.begin();
	while (it != configs.end())
	{
		if(it->get_host() == "localhost")
			it->set_host("127.0.0.1");
		if (host == TCPUtils::stringToLong(it->get_host()) && port == it->get_port())
		{
			size_t i = -1;
			while (++i < it->get_server_names().size())
			{
				if (hostname == it->get_server_names()[i] || hostname == "localhost")
					return (*it);
			}
		}
		it++;
	}
	return (configs[0]);
}

// __ run server  ______________________________________________________________
// =============================================================================
void	TCPServer::runServer()
{
	fd_set FDSRead, FDSWrite;
	struct timeval timeout;
	int fdNum;
	int	i, j;
	
	FD_ZERO(&FDSWrite);
	FD_ZERO(&FDSRead);
	timeout.tv_sec = 5;
    timeout.tv_usec = 0;
	while (1)
	{
		FDSRead = FDs;
		fdNum = select(fdMax + 1, &FDSRead, &FDSWrite, 0, &timeout);
		if (fdNum >= 0)
		{
			for(i = 0; i < (fdMax + 1); i++)
			{
				if (clients[i].isKeepAlive == true)
				 	if (!handleTimeOut(i, &FDSRead, &FDSWrite))
						continue;
				if (FD_ISSET(i, &FDSRead) && (j = existSocket(i)))
				{
					if (!acceptConnection(j, &FDSRead))
						continue;
				}
				else
				{
					if (FD_ISSET(i, &FDSRead))
					{
						readRoutine(i, &FDSRead, &FDSWrite);
						if (clients[i].getReadNum() == 0 && clients[i].isHeader)
						{
							initClient(i);
							if (!clients[i].getHTTPParser())
								clients[i].setHTTPParser(new HTTPParser(""));
							clients[i].getHTTPParser()->setConfig(getConfigClient(i));
							clients[i].getHTTPParser()->setBody(clients[i].getRequest());
							clients[i].httpRequest = new HTTPRequest(clients[i].getHTTPParser());
							clients[i].httpRequest->processRequest();
						}
					}
					else if (FD_ISSET(i, &FDSWrite) && i != existSocket(i))
					{
						if (clients[i].getReadNum() == 0)
							sendRoutine(i, &FDSWrite, &FDSRead);
					}
				}
			}
		}
	}
}

// __ Init Client ______________________________________________________________
// =============================================================================
void	TCPServer::initClient(int sock)
{
	clients[sock].setIsChunked(0);
	clients[sock].isHeader = false;
	clients[sock].isBody = false;
}

// __ Read Routine _____________________________________________________________
// =============================================================================
void		TCPServer::readRoutine(int sock, fd_set *FDSRead, fd_set *FDSWrite)
{
	char buffer[BUFFER_SIZE];
	int	bytesNum = 0;

	(void) FDSRead;
	memset(buffer, 0, BUFFER_SIZE);
	// clients[sock].lastActivity = time(NULL);
	if ((bytesNum = recv(sock, buffer, BUFFER_SIZE, 0)) == 0)
	{
		std::cout << RED << "client with id : " << sock << " is disconnected by client" << RESET << std::endl;
		clients[sock].setReadNum(-1);
		destroyConnection(sock);
		return ;
	}
	if (bytesNum < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			FD_CLR(sock, &FDs);
			close(sock);
		}
	}
	clients[sock].setReadNum(bytesNum);
	clients[sock].setRequest(clients[sock].getRequest().append(buffer, bytesNum));
	if (!clients[sock].isHeader)
	{
		std::cout << YELLOW << clients[sock].getRequest() << RESET << std::endl;
		size_t pos = clients[sock].getRequest().find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			clients[sock].setHTTPParser(new HTTPParser(clients[sock].getRequest().substr(0, pos)));
			clients[sock].setRequest(clients[sock].getRequest().substr(pos + 4,  clients[sock].getRequest().size()));
			mapHeaders = clients[sock].getHTTPParser()->getHeaders();
			clients[sock].setIsChunked(mapHeaders.find("transfer-encoding") != mapHeaders.end());
			clients[sock].isHeader = true;
			clients[sock].isBody = true;
		}	
	}
	if (clients[sock].isBody)
	{
		if (clients[sock].getIsChunked())
		{
			std::string chunk = TCPUtils::parseChunkedBody(clients[sock].getRequest());
			(clients[sock].body).append(chunk);
		}
		else
			clients[sock].body = clients[sock].getRequest();
	}
	if (bytesNum < BUFFER_SIZE)
	{
		clients[sock].setReadNum(0);
		FD_CLR(sock, &FDs);
		FD_SET(sock, FDSWrite);
	}	
}

// __ Send Routine _____________________________________________________________
// =============================================================================
void	TCPServer::sendRoutine(int sock, fd_set *FDSWrite, fd_set *FDSRead)
{
	(void) FDSRead;
  	std::ostringstream response;
	response << clients[sock].httpRequest->getResponse();
	std::string str = response.str();
	int bytesSend = 0;
	
	if (clients[sock].getSendNum() && clients[sock].getSendNum() < str.size())
		str = str.substr(clients[sock].getSendNum(), str.size());
	size_t size = str.size();
	if (size > BUFFER_SIZE)
		size = BUFFER_SIZE;
	// clients[sock].lastActivity = time(NULL);
	if ((bytesSend = send(sock, str.c_str(), size + 1, 0)) < 0)
	{
		std::cout << std::strerror(errno) << std::endl;
		FD_CLR(sock, FDSWrite);
		destroyConnection(sock);
		return ;
	}
	if (clients[sock].getSendNum())
	   	clients[sock].setSendNum(clients[sock].getSendNum() + bytesSend);
	else
	   	clients[sock].setSendNum(bytesSend);
	if (bytesSend == 0 || bytesSend < BUFFER_SIZE)
	{
		clients[sock].setSendNum(0);
		FD_CLR(sock, FDSWrite);
		if (clients[sock].getHTTPParser()->getConnectionType() == HTTP_KEEPALIVE_ON)
		{
			std::cout << GREEN << "client with id : " << sock << " is keep alive" << RESET << std::endl;
			delete clients[sock].getHTTPParser();
			delete clients[sock].httpRequest;
			clients[sock].isKeepAlive = true;
			clients[sock].setRequest("");
			clients[sock].setIsChunked(0);
			clients[sock].isHeader = false;
			clients[sock].isBody = false;
			clients[sock].body = "";
			clients[sock].setReadNum(-1);
			clients[sock].setSendNum(0);
			clients[sock].setHTTPParser(NULL);
			clients[sock].httpRequest = NULL;
			clients[sock].lastActivity = time(NULL);
			FD_SET(sock, FDSRead);
			FD_SET(sock, &FDs);
			return ;
		}
		destroyConnection(sock);
	}
}
// __ Handle Time Out __________________________________________________________
// =============================================================================
bool	TCPServer::handleTimeOut(int sock, fd_set *FDSRead, fd_set *FDSWrite)
{
	// (void)FDSRead;
	// (void)FDSWrite;
	if (time(NULL) - clients[sock].lastActivity > 10)
	{
		std::cout << RED << BOLD << "client with id : " << sock << " is disconnected by timeout" << RESET << std::endl;
		if (FD_ISSET(sock, FDSRead))
		{
			FD_CLR(sock, FDSRead);
			FD_CLR(sock, &FDs);
			FD_SET(sock, FDSWrite);
		}
		if (FD_ISSET(sock, FDSWrite))
			FD_CLR(sock, FDSWrite);
		clients.erase(sock);
		close(sock);
		fdMax -= 1;
		return (false);
	}
	return (true);
}

// __ Destroy Connection _______________________________________________________
// =============================================================================
void	TCPServer::destroyConnection(int sock)
{
	std::cout << RED << BOLD << "client with id : " << sock << " is disconnected" << RESET << std::endl;
	if (clients[sock].httpRequest)
		delete clients[sock].httpRequest;
	if (clients[sock].getHTTPParser())
		delete clients[sock].getHTTPParser();
	close(sock);
	FD_CLR(sock, &FDs);
	clients.erase(sock);
	//fdMax -= 1;
}