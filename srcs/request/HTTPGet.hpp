/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:24:44 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/29 21:45:21 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
# define HTTPGET_HPP


# include "../cgi/Cgi.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"
# include "IHTTPHandler.hpp"
# include "utils/Utils.hpp"

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
