/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:13:08 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/27 22:11:19 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPUTILS_HPP
#define TCPUTILS_HPP

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fcntl.h>

namespace TCPUtils
{
	std::vector<char>	storeDataInVec(char *buffer);
	long stringToLong(const std::string& str);
	int hexCharToInt(char c);
	size_t hexToDecimal(const std::string& hex);
	std::string parseChunkedBody(std::string chunk);
	bool setNonBlocking(int sockfd);
};

#endif
