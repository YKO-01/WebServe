/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:28:04 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/24 20:42:32 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "IHTTPResponse.hpp"
# include "utils/ResponseUtility.hpp"

class HTTPResponse : public IHTTPResponse
{
	private:
		Status status;
		String payload;
		String response;
		std::vector<std::pair<String, String> > headers;

	public:
		HTTPResponse();
		String& operator[](const String& header);
		std::vector<std::pair<String, String> >& operator+=(const std::vector<std::pair<String, String> >& headers);
		virtual String generate(void);
		virtual Status getStatus(void) const;
		virtual void setStatus(const Status& code);
		virtual void setPayload(String payload);
		~HTTPResponse();
};

#endif
