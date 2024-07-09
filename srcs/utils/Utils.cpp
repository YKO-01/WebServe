/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 08:12:16 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/07/07 12:03:22 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

namespace Utils
{
	template <typename T>
	std::string toMyString(T value)
	{
		std::stringstream ss;
		ss << value;
		return (ss.str());
	}
}
std::string Utils::toString(int value)
{
	std::stringstream ss;
	ss << value;
	return (ss.str());
}
size_t Utils::toSize_t(const std::string& str)
{
	std::stringstream ss(str);
	size_t value;
	ss >> value;
	return (value);
}