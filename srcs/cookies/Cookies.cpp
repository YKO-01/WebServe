/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:40:57 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/07/09 17:49:49 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookies.hpp"

Cookies::Cookies(HTTPResponse *response, std::string key, std::string value, std::string expires)
{
    std::time_t now = std::time(0);
	std::tm *ltm = std::localtime(&now);
	std::string expires = "expires=";
	expires += std::to_string(ltm->tm_mday + 1);
	expires += " ";
	expires += std::to_string(ltm->tm_hour);
	expires += ":";
	expires += std::to_string(ltm->tm_min);
	expires += ":";
	expires += std::to_string(ltm->tm_sec);
	expires += " GMT";
    response->push_header("Set-Cookie", key + "=" + value + "; expires=" + expires);
}