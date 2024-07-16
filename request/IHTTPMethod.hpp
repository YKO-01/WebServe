/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPMethod.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 14:23:56 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/06 16:56:42 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPMETHOD_HPP
# define IHTTPMETHOD_HPP

# include <iostream>

# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>

# include "../../incs/http.h"
# include "config/Config.hpp"
# include "config/Cgi.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"

typedef std::string	String;

class IHTTPMethod
{
	public:
		IHTTPMethod() {}
		virtual Status	processResource() = 0;
		virtual Status	processFile(String resource) = 0;
		virtual Status	processDirectory(String resource) = 0;
		virtual Status	getStatus(void) const = 0;
		virtual std::string getResult(void) const = 0;
		virtual	std::map<std::string, std::string>	getCgiEnv(void) = 0;
};

#endif
