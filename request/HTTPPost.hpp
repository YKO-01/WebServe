/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:38:46 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/06 12:59:18 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
# define HTTPPOST_HPP

# include <sstream>
# include <fstream>

# include "IHTTPMethod.hpp"
# include "../utils/Utils.hpp"

class HTTPPost : public IHTTPMethod
{
	private:
		HTTPParser	*parser;

		Route	target;
		Status	status;
		String	result;
		String	resource;
		String	absolute_resource;

		HTTPResponse	*response;

	public:
		HTTPPost(HTTPParser *parser, HTTPResponse *response, Route route);
		virtual Status processResource();

		virtual Status  processFile(String resource);
		virtual Status  processDirectory(String directory);
		virtual String	getResult(void) const;
		virtual Status  getStatus(void) const;
		virtual	std::map<String, String>	getCgiEnv(void);

		Status	uploadResource(String resource);
		String	getFileName(String headers);
		void	writeInFile(String filename, String content);
		~HTTPPost();
};

#endif
