/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 10:00:28 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/29 18:20:12 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPParser.hpp"

HTTPParser::HTTPParser(String request) : encoding(HTTP_ENCODE_LENGTH), connection(HTTP_KEEPALIVE_ON)
{
	Header header;
	Iterator it;

	it = Utils::findToken(request.begin(), request.end(), "\r\n", false);
	status = parseStatusLine(request.begin(), it);
	if (status == HTTP_CONTINUE)
		status = parseHeaders(it, request.end());
}

String HTTPParser::operator[](const String& header)
{
	std::map<String, String>::iterator it;

	it = headers.find(header);
	if (it != headers.end())
		return (it->second);
	return "";
}

Status	HTTPParser::parseHeaders(Iterator& begin, const Iterator end)
{
	Iterator it;
	Header header;

	while (begin != end)
	{
		it = begin + 2;
		if (it == end)
			break ;
		begin = Utils::findToken(it, end, "\r\n", false);
		if (it == begin)
			break ;
		header = parseHeaderField(it, begin);
		if (header.first.empty())
			return (HTTP_BAD_REQUEST);
		headers[header.first] = header.second;
	}
	if ((*this)["host"].empty())
		return (HTTP_BAD_REQUEST);
	if (headers.find("transfer-encoding") != headers.end())
	{
		if (headers.find("content-length") != headers.end())
			return (HTTP_BAD_REQUEST);
		if ((*this)["transfer-encoding"] != "chunked")
			return (HTTP_NOT_IMPLEMENTED);
		else
			encoding = HTTP_ENCODE_CHUNKED;
	}
	else
	{
		if ((*this)["content-length"].find_first_not_of("0123456789") != String::npos)
			return (HTTP_BAD_REQUEST);
		encoding = HTTP_ENCODE_LENGTH;
	}
	if (!(*this)["connection"].compare("close"))
		connection = HTTP_KEEPALIVE_OFF;
	else
		connection = HTTP_KEEPALIVE_ON;
	if (body.length() > config.get_client_body_size())
		return (HTTP_REQUEST_TOO_LARGE);
	return (HTTP_CONTINUE);
}

Status	HTTPParser::parseStatusLine(const Iterator begin, const Iterator end)
{
	Status	status;
	Iterator it;

	try
	{
		it = begin;
		method = parseMethod(it, Utils::findToken(it, end, " ", false));
		it = Utils::ltrimString(it, end);
		uri = parseUri(it, Utils::findToken(it, end, " ", false));
		it = Utils::ltrimString(it, end);
		version = parseVersion(it, Utils::findToken(it, end, " ", false));
		status = HTTP_CONTINUE;
	}
	catch (const HTTPMethodNotAllowed& e)
	{
		status = HTTP_METHOD_NOT_ALLOWED;
	}
	catch (const HTTPVersionNotSupported& e)
	{
		status = HTTP_NOT_SUPPORTED;
	}
	catch (const std::exception& e)
	{
		status = HTTP_BAD_REQUEST;
	}
	return (status);
}

Method	HTTPParser::parseMethod(Iterator& begin, const Iterator end)
{
	String	method;

	method = String(begin, end);
	begin = end;
	if (!Utils::isValidMethod(method))
		throw HTTPBadMethod();
	if (!method.compare("GET"))
		return (GET);
	else if (!method.compare("POST"))
		return (POST);
	else if (!method.compare("DELETE"))
		return (DELETE);
	else
		throw HTTPMethodNotAllowed();
}

Uri	HTTPParser::parseUri(Iterator& begin, const Iterator end)
{
	Uri	uri;
	String authority;
	std::pair<Iterator, String> temp;

	if (!Utils::isValidUriCharacters(String(begin, end)))
		throw HTTPBadUri();
	if (std::distance(begin, end) >= HTTP_MAX_URI)
		throw HTTPUriOverflow();
	temp = Utils::parseToken(begin, end, "://");
	if (temp.first != end)
	{
		uri.scheme = temp.second;
		begin = temp.first;
		temp = Utils::parseToken(begin, end, "/", false);
		if (temp.first != end)
		{
			authority = temp.second;
			begin = temp.first;
			temp = Utils::parseToken(authority.begin(), authority.end(), ":", false);
			if (temp.first != authority.end())
			{
				uri.host = temp.second;
				uri.port = String(temp.first + 1, authority.end());
			}
			else
				uri.host = authority;
		}
		else
		{
			uri.host = String(begin, end);
			begin = temp.first;
		}
	}
	if (Utils::locateToken(begin, end, "?") <= Utils::locateToken(begin, end, "#"))
	{
		temp = Utils::parseToken(begin, end, "?");
		if (temp.first != end)
		{
			uri.resource = temp.second;
			begin = temp.first;
			temp = Utils::parseToken(begin, end, "#");
			if (temp.first != end)
			{
				uri.query = temp.second;
				uri.fragment = String(temp.first, end);
			}
			else
				uri.query = String(begin, end);
		}
		else
			uri.resource = String(begin, end);
	}
	else
	{
		temp = Utils::parseToken(begin, end, "#");
		if (temp.first != end)
		{
			uri.resource = temp.second;
			uri.fragment = String(temp.first, end);
		}
		else
			uri.resource = String(begin, end);
	}
	begin = end;
	if (!Utils::isValidUriScheme(uri.scheme))
		throw HTTPUriBadScheme();
	if (uri.scheme.empty() && (!uri.host.empty() || !uri.port.empty()))
		throw HTTPUriBadHost();
	if (!uri.scheme.empty() && uri.host.empty())
		throw HTTPUriBadHost();
	if (!Utils::isValidUriHost(uri.host))
		throw HTTPUriBadHost();
	if (!Utils::isValidUriPort(uri.port))
		throw HTTPUriBadPort();
	if (!Utils::isValidUriResource(uri.resource))
		throw HTTPUriBadResource();
	return (uri);
}

Version	HTTPParser::parseVersion(Iterator& begin, const Iterator end)
{
	Version version;
	String name;
	String	major;
	String	minor;
	std::pair<Iterator, String>	temp;

	temp = Utils::parseToken(begin, end, "/");
	if (temp.first != end)
	{
		name = temp.second;
		if (*(end - 1) != '.')
			major = String(temp.first, end);
		else
			major = String(temp.first, end - 1);
		temp = Utils::parseToken(temp.first, end, ".");
		if (temp.first != end)
		{
			major = temp.second;
			minor = String(temp.first, end);
			if (!minor.length())
				throw HTTPBadVersion();
		}
	}
	if (!Utils::isValidVersion(name, major, minor))
		throw HTTPBadVersion();
	version.major = std::atoi(major.c_str());
	version.minor = std::atoi(minor.c_str());
	if (version.major != 1 || version.minor != 1)
		throw HTTPVersionNotSupported();
	return (version);
}

Header	HTTPParser::parseHeaderField(const Iterator begin, const Iterator end)
{
	Header header;
	Iterator it;

	it = Utils::findToken(begin, end, ":", false);
	header.first = parseHeaderFieldName(begin, it);
	if (it != end)
		header.second = parseHeaderFieldValue(it + 1, end);
	return (header);
}

String	HTTPParser::parseHeaderFieldName(const Iterator begin, const Iterator end)
{
	String	name;

	name = String(begin, end);
	if (!Utils::isValidHeader(name))
		name.clear();
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	return (name);
}

String	HTTPParser::parseHeaderFieldValue(const Iterator begin, const Iterator end)
{
	String	value;
	Iterator it;
	Iterator ite;

	it = Utils::ltrimString(begin, end, " \t");
	ite = Utils::rtrimString(begin, end, " \t\r");
	value = String(it, ite);
	return (value);
}

Method	HTTPParser::getMethod(void) const
{
	return (method);
}

Uri	HTTPParser::getUri(void) const
{
	return (uri);
}

Version	HTTPParser::getVersion(void) const
{
	return (version);
}

std::map<String, String> HTTPParser::getHeaders(void) const
{
	return (headers);
}

Status	HTTPParser::getStatus(void) const
{
	return (status);
}

void	HTTPParser::setConfig(Config config)
{
	this->config = config;
}

Config	HTTPParser::getConfig() const
{
	return (config);
}

void	HTTPParser::setBody(const String& body)
{
	this->body = body;
}

String	HTTPParser::getBody(void) const
{
	return (body);
}

http_keepalive_t	HTTPParser::getConnectionType(void)
{
	return (connection);
}

http_encoding_t	HTTPParser::getEncodingType(void)
{
	return (encoding);
}

const char	*HTTPParser::HTTPBadMethod::what() const throw()
{
	return ("Bad Method");
}

const char	*HTTPParser::HTTPMethodNotAllowed::what() const throw()
{
	return ("Method Not Allowed");
}

const char	*HTTPParser::HTTPBadVersion::what() const throw()
{
	return ("Bad Version");
}

const char	*HTTPParser::HTTPBadUri::what() const throw()
{
	return ("Bad Uri");
}

const char	*HTTPParser::HTTPUriOverflow::what() const throw()
{
	return ("Uri Too Long");
}

const char	*HTTPParser::HTTPUriBadScheme::what() const throw()
{
	return ("Uri Bad Scheme");
}

const char	*HTTPParser::HTTPUriBadHost::what() const throw()
{
	return ("Uri Bad Host");
}

const char	*HTTPParser::HTTPUriBadPort::what() const throw()
{
	return ("Uri Bad Port");
}

const char	*HTTPParser::HTTPUriBadResource::what() const throw()
{
	return ("Uri Bad Resource");
}

const char	*HTTPParser::HTTPVersionNotSupported::what() const throw()
{
	return ("Version Not Supported");
}

const char	*HTTPParser::HTTPBadHeader::what() const throw()
{
	return ("Bad Header");
}

HTTPParser::~HTTPParser()
{
}
