/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:40:40 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/16 10:16:53 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPPost.hpp"

HTTPPost::HTTPPost(HTTPParser *parser, HTTPResponse *response, Route route) : parser(parser), target(route), resource(parser->getUri().resource), response(response)
{
	resource.erase(0, target.get_path().length());
	absolute_resource = target.get_directory() + this->resource;
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
	std::cout << "Hello" << std::endl;
	boundary = contentType.substr(0x1e);
	if (boundary.empty())
		return (HTTP_BAD_REQUEST);
	Iterator	it;
	std::string	header;
	std::string content;
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

String	HTTPPost::getFileName(std::string headers)
{
	std::string	header;
	std::string filename;

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
	return (target.get_upload() + "/" + filename);
}

void	HTTPPost::writeInFile(std::string filename, std::string content)
{
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
	if (directory[directory.length() - 1] != '/')
	{
		(*response)["Location"] = target.get_path() + resource + "/";
		return (HTTP_MOVED_PERMANENTLY);
	}
	if (!target.get_default_file().empty())
	{
		resource = target.get_default_file();
		if (!access((directory + resource).c_str(), F_OK))
			return (processFile(resource));
	}
	return (HTTP_FORBIDDEN);
}

Status	HTTPPost::processFile(String resource)
{
	if (target.get_useCGI() && Utils::hasCgiExtension(resource))
	{
		CGI cgiExecuter(getCgiEnv());
		if (!cgiExecuter.exec_cgi())
			return (HTTP_SERVER_ERROR);
		result = cgiExecuter.getCgiOutput();
		return (HTTP_OK);
	}
	return (HTTP_FORBIDDEN);
}

std::map<String, String>	HTTPPost::getCgiEnv(void)
{
	std::map<std::string, std::string>      env;

	env["SERVER_SOFTWARE"] = "phantom/1.0.0";
	env["SERVER_NAME"] = parser->getConfig().get_host();
	env["SERVER_PORT"] = std::to_string(parser->getConfig().get_port());
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["REQUEST_METHOD"] = "POST";
	env["PATH_INFO"] = target.get_directory();
	env["SCRIPT_NAME"] = resource;
	env["QUERY_STRING"] = parser->getBody();
	env["CONTENT_TYPE"] = (*parser)["content-type"];
	env["CONTENT_LENGTH"] = std::to_string(parser->getBody().length());
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
