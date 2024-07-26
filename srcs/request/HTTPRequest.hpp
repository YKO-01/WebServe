/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:32:55 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/21 10:43:54 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "HTTPGet.hpp"
# include "HTTPPost.hpp"
# include "HTTPDelete.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"

class HTTPRequest
{
	private:
		Route route;
		HTTPParser* parser;
		HTTPResponse *response;

	public:
		HTTPRequest(HTTPParser* parser);
		void processRequest();
		IHTTPHandler* buildRequest();
		Route getMatchedLocation();
		String generateResponsePayload();
		String getResponse();
		~HTTPRequest();
};

#endif
