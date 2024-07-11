/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalid <khalid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:28:04 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/11 11:22:43 by khalid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <map>
# include "IHTTPResponse.hpp"
# include "utils/ResponseUtility.hpp"

class HTTPResponse : public IHTTPResponse
{
	private:
		//Version	version;
		std::string	body;
		std::string	response;

		Status status;
		std::map<std::string, std::string>	headers;
	public:
		HTTPResponse();
		std::string&	operator[](const std::string& header);
		virtual std::string generate(void);
		virtual	void setStatus(const Status& code);
		virtual	Status	getStatus(void) const;
		virtual void setBody(std::string body);
		~HTTPResponse();
};

#endif
