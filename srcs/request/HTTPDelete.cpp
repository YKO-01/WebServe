/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 13:56:27 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/29 21:28:49 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPDelete.hpp"

HTTPDelete::HTTPDelete(HTTPParser *parser, HTTPResponse *response, Route route) : parser(parser), response(response), target(route), resource(parser->getUri().resource)
{
	absolute_resource = target.get_directory() + this->resource.substr(target.get_path().length());
}

Status	HTTPDelete::processResource()
{
	if (access(absolute_resource.c_str(), F_OK) < 0)
		return (HTTP_NOT_FOUND);
	if (Utils::isDirectory(absolute_resource))
		status = processDirectory(absolute_resource);
	else
		status = processFile(absolute_resource);
	return (status);
}

Status HTTPDelete::processDirectory(String directory)
{
	if (resource[resource.length() - 1] != '/')
		return (HTTP_CONFLICT);
	if (target.get_useCGI())
	{
		if (!target.get_default_file().empty())
		{
			resource += target.get_default_file();
			if (!access((directory + resource).c_str(), F_OK))
				return (processFile(resource));
		}
	}
	else
	{
		if (deleteDirectory(directory))
			return (HTTP_NO_CONTENT);
		else
		{
			if (!access(directory.c_str(), W_OK))
				return (HTTP_SERVER_ERROR);
		}
	}
	return (HTTP_FORBIDDEN);
}

Status HTTPDelete::processFile(String file)
{
	if (target.get_useCGI() && Utils::hasCgiExtension(file))
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
	else
		std::remove(file.c_str());
	return (HTTP_NO_CONTENT);
}

Status HTTPDelete::getStatus(void) const
{
	return (status);
}

String HTTPDelete::getResult(void) const
{
	return (result);
}

bool HTTPDelete::deleteDirectory(String folder)
{
	DIR				*dir;
	struct dirent	*dr;

	String path;
	dir = opendir(folder.c_str());
	if (!dir)
		return (false);
	dr = readdir(dir);
	while (dr)
	{
		if (std::strcmp(dr->d_name, ".") != 0 && std::strcmp(dr->d_name, "..") != 0)
		{
			path = folder + dr->d_name;
			if (Utils::isDirectory(path))
				deleteDirectory(path + "/");
			else
				std::remove(path.c_str());
		}
		dr = readdir(dir);
	}
	return (!std::remove(folder.c_str()));
}

std::map<String, String>	HTTPDelete::initCGIEnv(void)
{
	std::map<String, String>      env;

	env["SERVER_SOFTWARE"] = "phantom/1.0.0";
	env["SERVER_NAME"] = parser->getConfig().get_host();
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["REQUEST_METHOD"] = "DELETE";
	env["PATH_INFO"] = target.get_directory();
	env["SCRIPT_NAME"] = resource.substr(target.get_path().length());
	env["HTTP_COOKIE"] = (*parser)["cookie"];
	return (env);
}

HTTPDelete::~HTTPDelete()
{
}
