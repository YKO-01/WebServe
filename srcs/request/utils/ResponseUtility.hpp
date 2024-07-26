/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtility.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:35:06 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/24 20:43:04 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEUTILITY_HPP
# define RESPONSEUTILITY_HPP

# include "http.h"

namespace ResponseUtility
{
	std::string	getTime(std::time_t time);
	std::string	translateStatus(Status status);
};

#endif
