/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:29:56 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/07/26 20:33:35 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <map>
# include <vector>
# include <iostream>

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
