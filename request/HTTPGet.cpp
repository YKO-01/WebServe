/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:16:21 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/26 20:35:03 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPGet.hpp"

HTTPGet::HTTPGet(HTTPParser *parser, HTTPResponse *response, Route route) : parser(parser), response(response), target(route), resource(parser->getUri().resource)
{
	this->absolute_resource = target.get_directory() + this->resource.substr(target.get_path().length(), resource.length());
}

Status HTTPGet::processResource()
{
	if (access(absolute_resource.c_str(), F_OK) < 0)
		status = HTTP_NOT_FOUND;
	else if (Utils::isDirectory(absolute_resource))
		status = processDirectory(resource);
	else
		status = processFile(absolute_resource);
	return (status);
}

Status	HTTPGet::processFile(String resource)
{
	if (access(resource.c_str(), R_OK) < 0)
		return (HTTP_FORBIDDEN);
	if (target.get_useCGI() && Utils::hasCgiExtension(resource))
	{
		CGI cgiExecuter(initCGIEnv()); 
		if (!cgiExecuter.exec_cgi())
			return (HTTP_SERVER_ERROR);
		*response += cgiExecuter.get_cgi_heahers();
		response->setPayload(cgiExecuter.get_cgi_output());
	}
	else
	{
		response->setPayload(Utils::readFile(resource));
		struct stat attrib;
		stat(resource.c_str(), &attrib);
		(*response)["Content-Type"] = Utils::identifyMimeType(resource);
		(*response)["Last-Modified"] = ResponseUtility::getTime(attrib.st_mtime);
	}
	return (HTTP_OK);
}

Status	HTTPGet::processDirectory(String resource)
{
	if (resource[resource.length() - 1] != '/')
	{
		(*response)["Location"] = resource + "/";
		return (HTTP_MOVED_PERMANENTLY);
	}
	if (!target.get_default_file().empty())
	{
		resource = absolute_resource + target.get_default_file();
		this->resource = target.get_default_file();
		if (!access(resource.c_str(), F_OK))
			return (processFile(resource));
	}
	if (target.get_directory_listing())
	{
		response->setPayload(getAutoIndex(absolute_resource));
		return (HTTP_OK);
	}
	return (HTTP_FORBIDDEN);
}

String	HTTPGet::getAutoIndex(String resource)
{
	DIR	*directory;
	struct dirent*	dr;
	String	index;

	directory = opendir(resource.c_str());
	if (!directory)
		;
	index ="<html>\r\n";
	index += "<head><title>Index of " + target.get_path() + this->resource + "</title></head>\r\n";
	index += "<body>\r\n";
	index += "<h1>Index of " + target.get_path() + this->resource + "</h1><hr><pre>";
	index += "<a href=\"../\">../</a>\r\n";
	dr = readdir(directory);
	while (dr)
	{
		if (std::strcmp(dr->d_name, ".") != 0 && strcmp(dr->d_name, "..") != 0)
			index += getIndexEntry(resource, dr->d_name);
		dr = readdir(directory);
	}
	index += "</pre><hr></body>\r\n";
	index += "</html>\r\n";
	return (index);
}

String	HTTPGet::getHyperlink(String& file)
{
	String a;

	a = "<a href=\"" + file + "\">";
	file = (file.length() < 50 ? file : file.substr(0, 46) + "..>");
	a += file;
	a += "</a>";
	return (a);
}

String	HTTPGet::getIndexEntry(String directory, String resource)
{
	String entry;
	struct stat fileStat;
	struct tm* timeinfo;
	char	buffer[20];

	if (stat((directory + resource).c_str(), &fileStat) < 0)
		return (entry);
	timeinfo = localtime(&fileStat.st_mtime);
	if (S_ISDIR(fileStat.st_mode))
	{
		resource += "/";
		entry = getHyperlink(resource) + String(50 - resource.length(), ' ');
		strftime(buffer, 20, "%d-%b-%Y %H:%M", timeinfo);
		entry += buffer + String(20, ' ') + "-";
	}
	else
	{
		entry = getHyperlink(resource) + String(50 - resource.length(), ' ');
		strftime(buffer, 20, "%d-%b-%Y %H:%M", timeinfo);
		entry += buffer + String(20, ' ') + std::to_string(fileStat.st_size);
	}
	entry += "\r\n";
	return (entry);
}

String	HTTPGet::getResult(void) const
{
	return (result);
}

Status	HTTPGet::getStatus(void) const
{
	return (status);
}

std::map<String, String>	HTTPGet::initCGIEnv(void)
{
	std::map<String, String>	env;

	env["SERVER_SOFTWARE"] = "phantom/1.0.0";
	env["SERVER_NAME"] = parser->getConfig().get_host();
	env["SERVER_PORT"] = std::to_string(parser->getConfig().get_port());
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["REQUEST_METHOD"] = "GET";
	env["PATH_INFO"] = target.get_directory();
	env["SCRIPT_NAME"] = resource.substr(target.get_path().length());
	env["QUERY_STRING"] = parser->getUri().query;
	env["HTTP_COOKIE"] = (*parser)["cookie"];
	return (env);
}

HTTPGet::~HTTPGet()
{
}
