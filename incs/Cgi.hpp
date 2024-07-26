/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:29:56 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/07/26 09:13:17 by hkasbaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <stdbool.h>
#include <signal.h>


class   CGI
{
    private:
        std::map<std::string, std::string> _env;
        std::string outpute;
        
    public:
        CGI(std::map<std::string, std::string> _env);
        void set_output(const std::string &output);
        std::string get_output();
        std::string check_extension_file(const std::string &file);
        // std::string split_equal(const std::string &str);
        void exec_cpp(const std::string &path, char **env);
        bool exec_cgi();
        char** set_env();
};