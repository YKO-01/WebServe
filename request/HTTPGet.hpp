/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:24:44 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/23 11:43:02 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
# define HTTPGET_HPP


# include "config/Cgi.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"
# include "IHTTPHandler.hpp"
# include "../utils/Utils.hpp"

class HTTPGet : public IHTTPHandler
{
	private:
		HTTPParser *parser;
		HTTPResponse *response;
		Route target;
		Status status;
		String result;
		String resource;
		String absolute_resource;

	public:
		HTTPGet(HTTPParser *parser, HTTPResponse *response, Route route);
		virtual Status processResource();
		virtual Status processFile(String resource);
		virtual Status processDirectory(String resource);
		virtual String getResult(void) const;
		virtual Status getStatus(void) const;
		virtual	std::map<String, String> initCGIEnv(void);
		String readFile(String resource);
		String getAutoIndex(String resource);
		String getHyperlink(String& link);
		String getIndexEntry(String directory, String resource);
		~HTTPGet();
};

#endif
