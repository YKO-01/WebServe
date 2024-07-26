/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:38:46 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/25 17:20:27 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
# define HTTPPOST_HPP


# include "../cgi/Cgi.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"
# include "IHTTPHandler.hpp"
# include "utils/Utils.hpp"

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
