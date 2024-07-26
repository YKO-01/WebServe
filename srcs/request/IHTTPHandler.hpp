/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 10:41:15 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/23 11:42:38 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPHANDLER_HPP
# define IHTTPHANDLER_HPP

# include "../../incs/http.h"

class IHTTPHandler
{
	public:
		virtual ~IHTTPHandler() {}
		virtual Status processResource() = 0;
		virtual Status processFile(String resource) = 0;
		virtual Status processDirectory(String resource) = 0;
		virtual Status getStatus(void) const = 0;
		virtual String getResult(void) const = 0;
		virtual	std::map<String, String> initCGIEnv(void) = 0;
};

#endif
