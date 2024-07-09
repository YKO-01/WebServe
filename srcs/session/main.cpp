/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:37:11 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/07/09 19:58:41 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"
#include <unistd.h>

using namespace std;
int main(int ac, char **av, char **env)
{
	(void)env;
	Session _session;
	if (ac < 2)
		return (0);

	chdir("sessions");
	MapDataSession dataSession;

	dataSession = _session.parseArg(av);
	if (_session.checkFirstArgIsSessionID(dataSession) == true)
	{
		std::string sessionID = dataSession.find("sessionID")->second;
		if (_session.validSession(sessionID) == false)
		{
			std::cout << "session not valid" << std::endl;
			return (1);
		}
		_session.setSession(sessionID, dataSession);
		return (0);
	}
	std::string sessionID = _session.createSession();
	dataSession["sessionID"] = sessionID;
	std::cout << sessionID << std::endl;
	_session.setSession(sessionID, dataSession);
	return (0);
}
