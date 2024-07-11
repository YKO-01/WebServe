/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPResponse.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:21:57 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/05 21:22:10 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPRESPONSE_HPP
# define IHTTPRESPONSE_HPP

# include <iostream>
# include <vector>

# include "../incs/http.h"

class IHTTPResponse
{
	public:
		virtual	~IHTTPResponse() {}
		virtual	std::string	generate(void) = 0;
		virtual	void setStatus(const Status& code) = 0;
		virtual Status getStatus(void) const = 0;
		virtual	void	setBody(std::string body) = 0;
};

#endif
