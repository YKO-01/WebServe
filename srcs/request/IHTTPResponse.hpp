/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPResponse.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:21:57 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/09 17:41:52 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPRESPONSE_HPP
# define IHTTPRESPONSE_HPP

# include <iostream>
# include <vector>

# include "../../incs/http.h"

class IHTTPResponse
{
	public:
		virtual	~IHTTPResponse() {}
		virtual	void	push_body(std::string body) = 0;
		virtual	void	push_header(std::string key, std::string value) = 0;
		virtual	std::string	generate(void) = 0;
		virtual	void setStatusCode(const Status& code) = 0;
		virtual Status getStatusCode(void) const = 0;
};

#endif
