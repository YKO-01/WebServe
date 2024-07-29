/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:40:40 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/29 21:28:39 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPPost.hpp"

HTTPPost::HTTPPost(HTTPParser *parser, HTTPResponse *response, Route route) : parser(parser), response(response), target(route), resource(parser->getUri().resource)
{
	absolute_resource = target.get_directory() + this->resource.substr(target.get_path().length());
}

Status  HTTPPost::processResource()
{
	if (!target.get_upload().empty())
		return (uploadResource(target.get_upload()));
	if (access(absolute_resource.c_str(), F_OK) < 0)
		return (HTTP_NOT_FOUND);
	if (Utils::isDirectory(absolute_resource))
		status = processDirectory(absolute_resource);
	else
		status = processFile(absolute_resource);
	return (status);
}

Status	HTTPPost::uploadResource(String resource)
{
	(void) resource;
	String boundary;
	String body;
	String	contentType = (*parser)["content-type"];

	if (contentType.compare(0, 0x1e, "multipart/form-data; boundary="))
		return (HTTP_BAD_REQUEST);
	boundary = contentType.substr(0x1e);
	if (boundary.empty())
		return (HTTP_BAD_REQUEST);
	Iterator	it;
	String	header;
	String content;
	std::pair<Iterator, String> parts;
	std::pair<Iterator, String>	data;

	body = parser->getBody();
	parts = Utils::parseToken(body.begin(), body.end(), boundary);
	it = parts.first;
	while (it != body.end())
	{
		if (parts.second.length() && parts.second != "--")
		{
			data = Utils::parseToken(parts.second.begin(), parts.second.end(), "\r\n\r\n");
			if (data.first != parts.second.end())
			{
				header = data.second;
				content = String(data.first, parts.second.end() - 0x02);
				writeInFile(getFileName(header), content);
			}
		}
		parts = Utils::parseToken(it, body.end(), boundary);
		it = parts.first;
	}
	return (HTTP_CREATED);
}

String	HTTPPost::getFileName(String headers)
{
	String	header;
	String filename;

	std::istringstream ss(headers);
	size_t	pos;
	while (std::getline(ss, header) && !header.empty())
	{
		if (!header.find("Content-Disposition"))
		{
			pos = header.find("filename=\"");
			if (pos != String::npos)
			{
				filename = header.substr(pos + 0xa);
				filename = filename.substr(0, filename.find("\""));
			}
		}
	}
	if (filename.empty())
		return (filename);
	return (target.get_upload() + "/" + filename);
}

void	HTTPPost::writeInFile(String filename, String content)
{
	if (filename.empty())
		return ;
	std::ofstream	file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file for writing : " << filename << std::endl;
		return ;
	}
	file.write(content.c_str(), content.size());
	file.close();
}

Status	HTTPPost::processDirectory(String directory)
{
	if (resource[resource.length() - 1] != '/')
	{
		(*response)["Location"] = resource + "/";
		return (HTTP_MOVED_PERMANENTLY);
	}
	if (!target.get_default_file().empty())
	{
		resource = target.get_default_file();
		if (!access((absolute_resource + resource).c_str(), F_OK))
			return (processFile(resource));
	}
	return (HTTP_FORBIDDEN);
}

Status	HTTPPost::processFile(String resource)
{
	if (target.get_useCGI() && Utils::hasCgiExtension(resource))
	{
		CGI cgiExecuter(initCGIEnv());
		if (!cgiExecuter.exec_cgi())
			return (HTTP_SERVER_ERROR);
		*response += cgiExecuter.get_cgi_heahers();
		response->setPayload(cgiExecuter.get_cgi_output());
		if (!(*response)["Location"].empty())
			return (HTTP_FOUND);
		return (HTTP_OK);
	}
	return (HTTP_FORBIDDEN);
}

std::map<String, String>	HTTPPost::initCGIEnv(void)
{
	std::map<String, String>      env;

	env["SERVER_SOFTWARE"] = "phantom/1.0.0";
	env["SERVER_NAME"] = parser->getConfig().get_host();
	env["SERVER_PORT"] = std::to_string(parser->getConfig().get_port());
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["REQUEST_METHOD"] = "POST";
	env["PATH_INFO"] = target.get_directory();
	env["SCRIPT_NAME"] = resource.substr(target.get_path().length());
	env["QUERY_STRING"] = parser->getBody();
	env["CONTENT_TYPE"] = (*parser)["content-type"];
	env["CONTENT_LENGTH"] = std::to_string(parser->getBody().length());
	env["HTTP_COOKIE"] = (*parser)["cookie"];
	return (env);
}

Status	HTTPPost::getStatus(void) const
{
	return (status);
}

String	HTTPPost::getResult(void) const
{
	return (result);
}

HTTPPost::~HTTPPost()
{
}
