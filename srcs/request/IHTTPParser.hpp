/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:40:30 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/29 21:43:15 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPPARSER_HPP
# define IHTTPPARSER_HPP

# include "http.h"

class IHTTPParser
{
	public:
		virtual ~IHTTPParser() {}
		virtual Status parseStatusLine(const Iterator begin, const Iterator end) = 0;
		virtual Method parseMethod(Iterator& begin, const Iterator end) = 0;
		virtual Uri parseUri(Iterator& begin, const Iterator end) = 0;
		virtual Version parseVersion(Iterator& begin, const Iterator end) = 0;
		virtual Status parseHeaders(Iterator& begin, const Iterator end) = 0;
		virtual Header parseHeaderField(const Iterator begin, const Iterator end) = 0;
		virtual String parseHeaderFieldName(const Iterator begin, const Iterator end) = 0;
		virtual String parseHeaderFieldValue(const Iterator begin, const Iterator end) = 0;
};

#endif
