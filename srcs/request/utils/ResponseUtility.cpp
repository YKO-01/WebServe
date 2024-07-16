/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtility.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:37:32 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/05 21:11:46 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseUtility.hpp"

std::string	ResponseUtility::getTime(std::time_t time)
{
    std::tm* timeInfo = std::gmtime(&time);
    if (timeInfo == nullptr) {
        std::cerr << "Error getting current time.\n";
        return "";
    }

    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);

    return std::string(buffer);
}

std::string	ResponseUtility::translateStatus(Status status)
{
	std::string	reason;

	switch (status)
	{
		case HTTP_CONTINUE:
			reason = "Continue";
			break ;
		case HTTP_SWITCHING_PROTOCOLS:
			reason = "Switching Protocols";
			break ;
		case HTTP_OK:
			reason = "OK";
			break ;
		case HTTP_CREATED:
			reason = "Created";
			break ;
		case HTTP_ACCEPTED:
			reason = "Accepted";
			break ;
		case HTTP_NO_CONTENT:
			reason = "No Content";
			break ;
		case HTTP_RESET_CONTENT:
			reason = "Reset Content";
			break ;
		case HTTP_PARTIAL_CONTENT:
			reason = "Partial Content";
			break ;
		case HTTP_MULTIPLE_CHOICES:
			reason = "Multiple Choices";
			break ;
		case HTTP_MOVED_PERMANENTLY:
			reason = "Moved Permanently";
			break ;
		case HTTP_FOUND:
			reason = "Found";
			break ;
		case HTTP_SEE_OTHER:
			reason = "See Other";
			break ;
		case HTTP_NOT_MODIFIED:
			reason = "Not Modified";
			break ;
		case HTTP_TEMPORARY_REDIRECT:
			reason = "Temporary Redirect";
			break ;
		case HTTP_BAD_REQUEST:
			reason = "Bad Request";
			break ;
		case HTTP_FORBIDDEN:
			reason = "Forbidden";
			break ;
		case HTTP_NOT_FOUND:
			reason = "Not Found";
			break ;
		case HTTP_METHOD_NOT_ALLOWED:
			reason = "Method Not Allowed";
			break ;
		case HTTP_NOT_ACCEPTABLE:
			reason = "Not Acceptable";
			break ;
		case HTTP_REQUEST_TIMEOUT:
			reason = "Request Timeout";
			break ;
		case HTTP_CONFLICT:
			reason = "Conflict";
			break ;
		case HTTP_GONE:
			reason = "Gone";
			break ;
		case HTTP_LENGTH_REQUIRED:
			reason = "Length Required";
			break ;
		case HTTP_REQUEST_TOO_LARGE:
			reason = "Content Too Large";
			break ;
		case HTTP_URI_TOO_LONG:
			reason = "URI Too Long";
			break ;
		case HTTP_UNSUPPORTED_MEDIATYPE:
			reason = "Unsupported Media Type";
			break ;
		case HTTP_SERVER_ERROR:
			reason = "Internal Server Error";
			break ;
		case HTTP_NOT_IMPLEMENTED:
			reason = "Not Implemented";
			break ;
		case HTTP_BAD_GATEWAY:
			reason = "Bad Gateway";
			break ;
		case HTTP_SERVICE_UNAVAILABLE:
			reason = "Service Unavailable";
			break ;
		case HTTP_NOT_SUPPORTED:
			reason = "HTTP Version Not Supported";
			break ;
		default:
			reason = "Not Recognized";
	}
	return (reason);
}
