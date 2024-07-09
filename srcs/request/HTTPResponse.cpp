/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:20:12 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/09 17:42:18 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
}

void	HTTPResponse::push_body(std::string body)
{
	this->body = body;
}

void	HTTPResponse::push_header(std::string key, std::string value)
{
	this->headers[key] = value;
}

std::string	HTTPResponse::generate(void)
{
	return ("<html><body>No Contnet!</body></html>");
}

void	HTTPResponse::setStatusCode(const Status& code)
{
	this->status_code = code;
}

Status	HTTPResponse::getStatusCode(void) const
{
	return (status_code);
}

HTTPResponse::~HTTPResponse()
{
}
