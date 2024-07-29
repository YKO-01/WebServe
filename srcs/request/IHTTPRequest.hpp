/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 07:11:30 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/29 21:43:21 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPREQUEST_HPP
# define IHTTPREQUEST_HPP

# include "http.h"

# include <map>
# include <vector>
# include <iostream>

typedef std::string	String;
typedef	std::string::iterator Iterator;

class IHTTPRequest
{
	public:
		virtual ~IHTTPRequest() {}
		virtual void processRequest() = 0;
};

#endif
