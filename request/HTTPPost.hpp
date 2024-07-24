/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:38:46 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/23 11:43:15 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
# define HTTPPOST_HPP


# include "config/Cgi.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"
# include "IHTTPHandler.hpp"
# include "../utils/Utils.hpp"

class HTTPPost : public IHTTPHandler
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
		HTTPPost(HTTPParser *parser, HTTPResponse *response, Route route);
		virtual Status processResource();
		virtual Status processFile(String resource);
		virtual Status processDirectory(String directory);
		virtual String getResult(void) const;
		virtual Status getStatus(void) const;
		virtual std::map<String, String> initCGIEnv(void);
		void writeInFile(String filename, String content);
		Status uploadResource(String resource);
		String getFileName(String headers);
		~HTTPPost();
};

#endif
