/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtility.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:35:06 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/05 21:12:54 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEUTILITY_HPP
# define RESPONSEUTILITY_HPP

# include "../../incs/http.h"

namespace ResponseUtility
{
	std::string	getTime(std::time_t time);
	std::string	translateStatus(Status status);
};

#endif
