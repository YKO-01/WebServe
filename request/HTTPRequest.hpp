/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:32:55 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/08 16:26:11 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "IHTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPParser.hpp"
# include "HTTPGet.hpp"
# include "HTTPPost.hpp"
# include "HTTPDelete.hpp"

class HTTPRequest : public IHTTPRequest
{
	private:
		HTTPParser*	parser;
		HTTPResponse	*response;

		Route route;
		String	body;

	public:
		HTTPRequest(HTTPParser* parser);
		virtual void processRequest();
		IHTTPMethod*	buildRequest();
		Route	getMatchedLocation();
		String	generateResponseBody();
		String	getResponse();
		~HTTPRequest();
};

#endif
