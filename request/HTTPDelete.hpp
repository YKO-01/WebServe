/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPDelete.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:38:46 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/23 11:43:30 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETE_HPP
# define HTTPDELETE_HPP


# include "config/Cgi.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"
# include "IHTTPHandler.hpp"
# include "../utils/Utils.hpp"

class HTTPDelete : public IHTTPHandler
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
		HTTPDelete(HTTPParser *parser, HTTPResponse *response, Route route);
		virtual Status processResource();
		virtual Status processFile(String resource);
		virtual Status processDirectory(String resource);
		virtual String getResult(void) const;
		virtual Status getStatus(void) const;
		virtual std::map<String, String> initCGIEnv(void);
		bool deleteDirectory(String directory);
		~HTTPDelete();
};

#endif
