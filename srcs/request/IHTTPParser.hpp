/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:40:30 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/21 10:55:38 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPPARSER_HPP
# define IHTTPPARSER_HPP

# include "../../incs/http.h"

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
