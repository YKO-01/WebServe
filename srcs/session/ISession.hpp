/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISession.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 21:03:36 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/07/09 17:55:04 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISESSION_HPP
#define ISESSION_HPP

#include <iostream>

class ISession
{
	public:
		virtual ~ISession();
		virtual std::string generateSessionID() = 0;
		virtual	std::string createSesssion() = 0;
		virtual std::string getPathSession(const std::string _sessionID) = 0;
		virtual bool validSession(const std::string _sessionID) = 0;
		virtual std::string getSession(const std::string& _sessionID) = 0;
		virtual void setSession(const std::string& _sessionID, char **arg) = 0;
		virtual	void	setCookies(const std::string& _sessionID) = 0;
};

#endif

