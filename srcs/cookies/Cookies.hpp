/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:27:23 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/07/09 17:46:07 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIES_HPP
#define COOKIES_HPP

#include <iostream>
#include "../request/HTTPResponse.hpp"

class Cookies
{
    public:
        Cookies(HTTPResponse *response, std::string key, std::string value, std::string expires);
        ~Cookies();
};

#endif