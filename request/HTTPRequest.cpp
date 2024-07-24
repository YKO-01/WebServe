/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 10:00:28 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/21 11:20:01 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(HTTPParser* parser) : parser(parser), response(new HTTPResponse())
{
}

void	HTTPRequest::processRequest()
{
	IHTTPHandler *builder = NULL;

	if (parser->getStatus() != HTTP_CONTINUE)
	{
		response->setStatus(parser->getStatus());
		response->setPayload(generateResponsePayload());
		return ;
	}

	route = getMatchedLocation();
	if (route.get_redirect().length())
	{
		(*response)["Location"] = route.get_redirect();
		response->setStatus(HTTP_MOVED_PERMANENTLY);
	}
	else if (std::find(route.get_methods().begin(), route.get_methods().end(), parser->getMethod()) == route.get_methods().end())
	{
		response->setStatus(HTTP_METHOD_NOT_ALLOWED);
	}
	else
	{
		builder = buildRequest();
		if (builder)
			response->setStatus(builder->processResource());
	}

	if (!builder || builder->getStatus() != HTTP_OK)
		response->setPayload(generateResponsePayload());
	delete builder;
}

IHTTPHandler*	HTTPRequest::buildRequest()
{
	IHTTPHandler	*builder;

	switch (parser->getMethod())
	{
		case GET:
			builder = new HTTPGet(parser, response, route);
			break ;
		case POST:
			builder = new HTTPPost(parser, response, route);
			break ;
		case DELETE:
			builder = new HTTPDelete(parser, response, route);
			break ;
		default:
			builder = NULL;
	}
	return (builder);
}

Route	HTTPRequest::getMatchedLocation()
{
	std::vector<Route>	routes;
	String	resource;
	Route	route;

	resource = parser->getUri().resource;
	routes = parser->getConfig().get_routes();
	for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); it++)
	{
		if (Utils::matchPathToRoute(resource, it->get_path()))
		{
			if (route.get_path().length() < it->get_path().length())
				route = *it;
		}
	}
	return (route);
}

String	HTTPRequest::generateResponsePayload()
{
	Status	status;
	String	result;
	String	code_message;
	std::map<int, String>	error_pages;
	std::map<int, String>::iterator	it;

	error_pages = parser->getConfig().get_error_pages();
	it = error_pages.find(response->getStatus());
	if (it != error_pages.end() && 0)
		result = Utils::readFile(it->second);
	else
	{
		status = response->getStatus();
		code_message = ResponseUtility::translateStatus(status);
		result = "<html>\r\n";
		result += "<head><title>";
		result += std::to_string(status) + " " + code_message;
		result += "</title></head>\r\n";
		result += "<body><center><h1>";
		result += std::to_string(status) + " " + code_message;
		result += "</h1></center>\r\n";
		result += "<hr><center>phantom/1.0.1</center></hr>\r\n";
		result += "</body>\r\n";
		result += "</html>\r\n";
	}
	return result;
}

String	HTTPRequest::getResponse()
{
	return (response->generate());
}

HTTPRequest::~HTTPRequest()
{
	delete response;
}
