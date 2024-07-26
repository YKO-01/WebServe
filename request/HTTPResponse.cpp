/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:20:12 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/26 16:50:04 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
}

String&	HTTPResponse::operator[](const String& header)
{
	for (std::vector<std::pair<std::string, std::string> >::iterator it = headers.begin(); it != headers.end(); ++it) {
		if (it->first == header) {
			return it->second;
		}
	}
	headers.push_back(std::make_pair(header, ""));
	return headers.back().second;
}

std::vector<std::pair<String, String> >& HTTPResponse::operator+=(const std::vector<std::pair<String, String> >& headers)
{
    this->headers.insert(this->headers.begin(), headers.begin(), headers.end());
    return (this->headers);
}

String	HTTPResponse::generate(void)
{
	std::map<String, String>::iterator it;

	response = "HTTP/1.1 " + std::to_string(status) + " " + ResponseUtility::translateStatus(status) + "\r\n"; 
	response += "Server: phantom/1.0.0\r\n";
	response += "Date: " + ResponseUtility::getTime(std::time(NULL)) + "\r\n";
	response += "Content-Length: " + std::to_string(payload.length()) + "\r\n";
	for (std::vector<std::pair<String, String> >::iterator it = headers.begin(); it != headers.end(); it++)
		response += it->first + ": " + it->second+ "\r\n";
	response += "\r\n";
	response += payload;
	return (response);
}

Status	HTTPResponse::getStatus(void) const
{
	return (status);
}

void	HTTPResponse::setStatus(const Status& status)
{
	this->status = status;
}

void	HTTPResponse::setPayload(String payload)
{
	this->payload = payload;
}

HTTPResponse::~HTTPResponse()
{
}
