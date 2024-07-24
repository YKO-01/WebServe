/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtility.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:35:06 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/16 09:27:26 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEUTILITY_HPP
# define RESPONSEUTILITY_HPP

# include "../../../incs/http.h"
# include <ctime>

namespace ResponseUtility
{
	std::string	getTime(std::time_t time);
	std::string	translateStatus(Status status);
};

#endif
