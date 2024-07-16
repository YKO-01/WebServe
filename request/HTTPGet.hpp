/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:24:44 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/08 21:26:40 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
# define HTTPGET_HPP

# include <fstream>
# include <sstream>

# include "IHTTPMethod.hpp"
# include "../utils/Utils.hpp"

class HTTPGet : public IHTTPMethod
{
	private:
		HTTPParser	*parser;

		Route target;
		Status status;
		String result;
		String	resource;
		String	absolute_resource;

		HTTPResponse	*response;

	public:
		HTTPGet(HTTPParser *parser, HTTPResponse *response, Route route);

		virtual Status	processResource();

		virtual Status	processFile(String resource);
		virtual Status	processDirectory(String resource);
		virtual	String	getResult(void) const;
		virtual	Status	getStatus(void) const;
		virtual	std::map<String, String>	getCgiEnv(void);

		String	readFile(String resource);
		String getAutoIndex(String resource);
		String	getHyperlink(String& link);
		String	getIndexEntry(String directory, String resource);

		~HTTPGet();
};

#endif
