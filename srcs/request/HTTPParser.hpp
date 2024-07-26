/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:42:35 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/24 20:42:46 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include "IHTTPParser.hpp"
# include "../config/Config.hpp"
# include "utils/Utils.hpp"

class HTTPParser : public IHTTPParser
{
	private:
		Config config;
		Uri uri;
		Method method;
		Status status;
		Version version;
		std::map<String, String> headers;
		String body;
		http_encoding_t encoding;
		http_keepalive_t connection;

	public:
		HTTPParser(String request);
		String operator[](const String& header);
		virtual Status parseStatusLine(const Iterator begin, const Iterator end);
		virtual Method parseMethod(Iterator& begin, const Iterator end);
		virtual Uri parseUri(Iterator& begin, const Iterator end);
		virtual Version parseVersion(Iterator& begin, const Iterator end);
		virtual Status parseHeaders(Iterator& begin, const Iterator end);
		virtual Header parseHeaderField(const Iterator begin, const Iterator end);
		virtual String parseHeaderFieldName(const Iterator begin, const Iterator end);
		virtual String parseHeaderFieldValue(const Iterator begin, const Iterator end);
		void setConfig(Config config);
		void setBody(const String& body);
		Uri getUri(void) const;
		Status getStatus(void) const;
		Method getMethod(void) const;
		Version getVersion(void) const;
		Config getConfig(void) const;
		String getBody(void) const;
		http_encoding_t getEncodingType(void);
		http_keepalive_t getConnectionType(void);
		std::map<String, String> getHeaders(void) const;
		class HTTPBadMethod : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPMethodNotAllowed: public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPBadUri : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriOverflow : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadScheme : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadHost : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadPort : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadResource : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPBadVersion : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPVersionNotSupported : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPBadHeader : public std::exception
		{
			virtual const char	*what() const throw();
		};
		~HTTPParser();
};

#endif
