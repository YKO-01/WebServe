/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 21:36:44 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/07/09 19:07:24 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"
#include <unistd.h>
//__ Constructor _______________________________________________________________
//==============================================================================
Session::Session()
{

}

//__ Destructor ________________________________________________________________
//==============================================================================
Session::~Session()
{
}

//__ Generate Session __________________________________________________________
//==============================================================================
std::string Session::generateSessionID()
{
	srand(time(NULL) + rand());
	std::stringstream ss;
	for (int i = 0; i < 20; i++)
	{
		int r = rand() % 127;
		while((r < 48 || r > 57))
			r = rand() % 127;
		ss << char(r);
	}
	return (ss.str());
}

//-- Create Session ____________________________________________________________
//==============================================================================
std::string Session::createSession()
{
	std::string sessionID = generateSessionID();
	expirationTime = std::time(NULL);
	std::string data = "expirationTime:" + Utils::toString(expirationTime) + "\n";
	char *arg[] = {(char *)data.c_str(), NULL};

	setSession(sessionID, arg);
	setCookies(sessionID);
	return (sessionID);
}

//__ Get Path Session __________________________________________________________
//==============================================================================
std::string Session::getPathSession(const std::string _sessionID)
{
	return (_sessionID);
}

//__ Valid Session _____________________________________________________________
//==============================================================================
bool	Session::validSession(const std::string _sessionID)
{
	if (access(_sessionID.c_str(), F_OK) == 0)
	{
		std::ifstream file(getPathSession(_sessionID));
		if (file.is_open())
		{
			std::string line;
			std::getline(file, line);
			std::string expirationTime = line.substr(15);
			if (std::time(NULL) - Utils::toSize_t(expirationTime) > 86400)
			{
				removeSession(_sessionID);
				return (false);
			}
			return (true);
		}
		std::cout << "file not open" << std::endl;
	}
	return (false);
}

MapDataSession	Session::getDataSession(const std::string& _sessionID)
{
	MapDataSession dataSession;
	if (validSession(_sessionID) == false)
		return (dataSession);
	std::ifstream file(getPathSession(_sessionID));
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			size_t pos = line.find(":");
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			dataSession[key] = value;
		}
	}
	return (dataSession);
}

//__ Get Session Data __________________________________________________________
//==============================================================================
// std::string Session::getSession(const std::string& _sessionID)
// {
// 	if (validSession(_sessionID) == false)
// 		return (std::string(""));
// 	std::ifstream file(getPathSession(_sessionID));
// 	if (file.is_open())
// 	{
// 		std::stringstream data;
// 		data << file;
// 		return (data.str());
// 	}
// 	return (std::string(""));
// }

//__ Set Session Data __________________________________________________________
//==============================================================================
void	Session::setSession(const std::string& _sessionID, char **arg)
{
	std::ofstream file;
	file.open(_sessionID, std::ios_base::app);
	for (int i = 1; arg[i]; i++)
	{
		file << arg[i] << std::endl;
	}
	file.close();
}
// {
// 	std::cout << "session id: " << _sessionID << std::endl;
// 	std::ofstream file;
// 	file.open(_sessionID, std::ios_base::app);
// 	std::cout << data << std::endl;
// 	file << data;
// 	file.close();
// }

//__ Remove Session ____________________________________________________________
//==============================================================================
void	Session::removeSession(const std::string& _sessionID)
{
	std::remove(getPathSession(_sessionID).c_str());
}

//__ Set Cookies _______________________________________________________________
//==============================================================================
void	Session::setCookies(const std::string& _sessionID)
{
	std::time_t now = std::time(0);
	std::tm *ltm = std::localtime(&now);
	std::string expires = "expires=";
	expires += std::to_string(ltm->tm_mday + 1);
	expires += " ";
	expires += std::to_string(ltm->tm_hour);
	expires += ":";
	expires += std::to_string(ltm->tm_min);
	expires += ":";
	expires += std::to_string(ltm->tm_sec);
	expires += " GMT";
	std::cout << "Set-Cookie: sessionID=" << _sessionID << "; path=/; expires=" << expires << std::endl;
}

//__ Check First Arg Is Session ID _____________________________________________
//==============================================================================
bool	Session::checkFirstArgIsSessionID(MapDataSession dataSession)
{
	if (dataSession.find("sessionID") == dataSession.end())
		return (false);
	return (true);
}

//__ Parse Arg _________________________________________________________________
//==============================================================================
MapDataSession	Session::parseArg(char **arg)
{
	std::map<std::string, std::string> data;
	for (int i = 1; arg[i]; i++)
	{
		std::string str = arg[i];
		size_t pos = str.find("=");
		std::string key = str.substr(0, pos);
		std::string value = str.substr(pos + 1);
		data[key] = value;
	}
	return (data);
}