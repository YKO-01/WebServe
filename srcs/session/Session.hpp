/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:34:39 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/07/09 18:13:10 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "ISession.hpp"
#include <ctime>
#include <unistd.h>
#include "../utils/Utils.hpp"

typedef std::map<std::string, std::string> MapDataSession;
// typedef std::map<std::string, DataSession> MapSessions;

class Session
{
	private:
		// DataSession dataSession;
		// MapSessions _sessions;
		size_t expirationTime;
		std::string generateSessionID();
		void setCookies(const std::string& _sessionID);
	//	std::string dirName;
	public:
		Session();
		~Session();
		std::string createSession();
		std::string	getPathSession(const std::string _sessionID);
		bool validSession(const std::string _sessionID);
		MapDataSession getDataSession(const std::string& _sessionID);
		void	setSession(const std::string& _sessionID, char **arg);
		void	removeSession(const std::string& _sessionID);

		MapDataSession parseArg(char **arg);

		bool checkFirstArgIsSessionID(MapDataSession dataSession);
};



#endif
