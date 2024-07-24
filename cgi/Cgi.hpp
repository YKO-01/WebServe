/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:29:56 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/07/21 16:49:34 by ael-mhar         ###   ########.fr       */
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
		std::string	get_cgi_extension(const std::string &file);
		std::string	getCgiOutput() const;
		std::vector<std::pair<std::string, std::string> > get_cgi_heahers();
        void exec_cpp(const std::string &path, char **env);
        bool exec_cgi();
        char** set_env();
};
