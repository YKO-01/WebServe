/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:29:56 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/07/29 21:50:49 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <map>
# include <vector>
# include <iostream>
#include "../config/Config.hpp"

class   CGI
{
    private:
        std::map<std::string, std::string> env;
        std::vector<std::pair<std::string, std::string> > headers;
		std::string	output;
        
    public:
        CGI(std::map<std::string, std::string> _env);
        bool exec_cgi();
        char** set_env();
		std::string	get_cgi_output() const;
		std::vector<std::pair<std::string, std::string> > get_cgi_heahers();
};
