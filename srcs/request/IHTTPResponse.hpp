/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPResponse.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:21:57 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/29 21:43:28 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPRESPONSE_HPP
# define IHTTPRESPONSE_HPP

# include "http.h"

class IHTTPResponse
{
	public:
		virtual ~IHTTPResponse() {}
		virtual String generate(void) = 0;
		virtual Status getStatus(void) const = 0;
		virtual void setStatus(const Status& code) = 0;
		virtual void setPayload(String payload) = 0;
};

#endif
