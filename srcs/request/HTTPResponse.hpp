/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:28:04 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/09 17:43:23 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "IHTTPResponse.hpp"
# include <map>

class HTTPResponse : public IHTTPResponse
{
	private:
		std::string	body;
		Status status_code;
		std::map<std::string, std::string> headers;

	public:
		HTTPResponse();
		virtual void push_body(std::string body);
		virtual void push_header(std::string key, std::string value);
		virtual std::string generate(void);
		virtual	void setStatusCode(const Status& code);
		virtual	Status	getStatusCode(void) const;
		~HTTPResponse();
};

#endif
