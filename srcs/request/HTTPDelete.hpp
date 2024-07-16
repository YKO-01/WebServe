/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPDelete.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:38:46 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/16 09:04:40 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETE_HPP
# define HTTPDELETE_HPP

# include <sstream>
# include <fstream>

# include "IHTTPMethod.hpp"

# include "utils/Utils.hpp"

class HTTPDelete : public IHTTPMethod
{
	private:
		HTTPParser *parser;
		Route	target;
		Status	status;
		String	result;
		String	resource;
		String	absolute_resource;

	public:
		HTTPDelete(HTTPParser *parser, Route route);
		virtual Status processResource();

		virtual Status  processFile(String resource);
		virtual Status  processDirectory(String resource);
		virtual String	getResult(void) const;
		virtual Status  getStatus(void) const;
		virtual std::map<String, String>	getCgiEnv(void);

		bool	deleteDirectory(String directory);

		~HTTPDelete();
};

#endif
