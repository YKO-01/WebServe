/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalid <khalid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 08:12:16 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/11 11:31:15 by khalid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

Iterator Utils::findToken(const Iterator begin, const Iterator end, String token, bool _override)
{
	Iterator it;

	it = std::search(begin, end, token.begin(), token.end());
	if (it != end)
		return ((it + (_override ? token.length() : 0)));
	return (it);
}

std::size_t	Utils::locateToken(const Iterator begin, const Iterator end, String token)
{
	Iterator it;

	it = findToken(begin, end, token, false);
	if (it != end)
		return (std::distance(begin, it));
	return (String::npos);
}

std::pair<Iterator, String>	Utils::parseToken(const Iterator begin, const Iterator end, String token, bool _override)
{
	Iterator it;

	it = findToken(begin, end, token, _override);
	if (it != end)
		return (std::make_pair(it, String(begin, it - (_override ? token.length() : 0))));
	return (std::make_pair(it, String()));
}

Iterator Utils::ltrimString(const Iterator begin, const Iterator end, String charset)
{
	Iterator it;
	size_t	i;

	it = begin;
	while (it != end)
	{
		i = -1;
		while (++i != charset.length() && *it != charset[i])
			;
		if (i == charset.length())
			return (it);
		it++;
	}
	return (it);
}

Iterator Utils::rtrimString(const Iterator begin, const Iterator end, String charset)
{
	Iterator it;
	size_t	i;

	it = end;
	while (it != begin)
	{
		i = -1;
		while (++i != charset.length() && *it != charset[i])
			;
		if (i == charset.length())
			return (it + 1);
		it--;
	}
	return (it);
}

String  Utils::readFile(String resource)
{
	std::ifstream file(resource);
	if (!file.is_open())
		return ("");
	std::stringstream       buffer;
	buffer << file.rdbuf();
   	return (buffer.str());
}

bool	Utils::isValidUriCharacters(String uri)
{
	if (uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != String::npos)
		return (false);
	return (true);
}

bool	Utils::isValidMethod(String method)
{
	    if (method.empty())
                return (false);
        if (method.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ-_") != String::npos)
                return (false);
        return (true);
}

bool	Utils::isValidUriScheme(String scheme)
{
	   if (scheme.empty())
                return (true);
        if (!std::isalpha(scheme[0]))
                return (false);
        else if (scheme.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-.") != String::npos)
                return (false);
        return (true);
}

bool	Utils::isValidUriHost(String host)
{
        if (host.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-") != String::npos)
                return (false);
        else if (host.find("..") != String::npos)
                return (false);
        return (true);
}

bool	Utils::isValidUriPort(String port)
{
	if (port.find_first_not_of("0123456789") != String::npos || port.length() > 6)
		return (false);
	if (std::atoi(port.c_str()) > 65535)
		return (false);
	return (true);
}

bool	Utils::isValidUriResource(String resource)
{
	if (resource.empty() || resource[0] != '/')
		return (false);
	return (true);
}

bool	Utils::isValidVersion(String name, String major, String minor)
{
	if (name.compare("HTTP"))
		return (false);
	if (major.empty() || major.find_first_not_of("0123456789") != String::npos)
		return (false);
	if (minor.find_first_not_of("0123456789") != String::npos)
		return (false);
	return (true);
}

bool	Utils::isnotprint(char c)
{
	if (c < 32 || c > 126)
		return (true);
	return (false);
}

bool	Utils::isValidHeader(String header)
{
	if (header.empty() || header.find_first_of(" \t") != String::npos)
		return (false);
	if (std::find_if(header.begin(), header.end(), Utils::isnotprint) != header.end())
		return (false);
	return (true);
}

bool    Utils::isDirectory(String path)
{
        struct stat st;

        if (stat(path.c_str(), &st) < 0)
                return (false);
        return (S_ISDIR(st.st_mode));
}

bool	Utils::matchPathToRoute(String path, String route)
{
	if (!path.compare(0, route.length(), route))
	{
		if (path.length() > route.length() && path[route.length()] != '/' && route != "/")
			return (false);
		return (true);
	}
	return (false);
}

String	Utils::identifyContentType(std::string filename)
{
	std::map<std::string, std::string> mime_types;

	std::ifstream file("/Users/ael-mhar/Desktop/webserv/main/srcs/utils/mime.txt");

	if (!file.is_open())
		return ("");
    std::string line;
    while (std::getline(file, line))
    {
        std::string key = line.substr(0, line.find(" "));
        std::string value = line.substr(line.find("|") + 1);
        mime_types[key] = String(ltrimString(value.begin(), value.end()), value.end()); 
    }
	file.close();
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    if (mime_types.find(extension) != mime_types.end())
		return mime_types[extension];
	return ("application/octet-stream");
}

bool	Utils::hasCgiExtension(const String& file)
{
	std::string extension;
	std::string formats[] = {"php", "sh", "cpp", "py"};

	size_t	pos = file.find_last_of(".");
	if (pos != std::string::npos)
	{
		extension = file.substr(pos + 1);
		for (size_t i = 0; i < formats->size() ; i++)
        {
            if (extension == formats[i])
                return (true);
        }
	}
	return (false);
}
