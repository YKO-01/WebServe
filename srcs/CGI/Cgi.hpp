/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:29:56 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/07/16 09:20:33 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <stdbool.h>


class   CGI
{
    private:
        std::map<std::string, std::string> _env;
        std::string output;
        
    public:
        CGI(std::map<std::string, std::string> _env);
        std::string check_extension_file(const std::string &file);
        void exec_cpp(const std::string &path, char **env);
        bool exec_cgi();
        std::string getCgiOutput();
        char** set_env();
};