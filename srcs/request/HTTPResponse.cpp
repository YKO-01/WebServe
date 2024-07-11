/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:20:12 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/08 21:23:51 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
}

std::string&	HTTPResponse::operator[](const std::string& header)
{
	return (headers[header]);
}

std::string	HTTPResponse::generate(void)
{
	std::map<std::string, std::string>::iterator it;
	std::cout << "================================================================\n";
	response = "HTTP/1.1 " + std::to_string(status) + " " + ResponseUtility::translateStatus(status) + "\r\n"; 
	response += "Server: phantom/1.0.0\r\n";
	response += "Date: " + ResponseUtility::getTime(std::time(NULL)) + "\r\n";
	response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		response += it->first + " : " + it->second+ "\r\n";
	if (status != HTTP_OK)
		response += "Connection: close\r\n";
	else
		response += "Connection: keep-alive\r\n";
	response += "\r\n";
	response += body;

	return (response);
}

void	HTTPResponse::setStatus(const Status& status)
{
	this->status = status;
}

Status	HTTPResponse::getStatus(void) const
{
	return (status);
}

void	HTTPResponse::setBody(std::string body)
{
	this->body = body;
}

HTTPResponse::~HTTPResponse()
{
}
