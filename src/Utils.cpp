/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:49:32 by esellier          #+#    #+#             */
/*   Updated: 2025/06/25 18:24:19 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

bool	isInt(std::string const& value)
{
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] > '9' || value[i] < '0' || i > 5 )
			return false;
	}
	
	return true;
}

bool	isIp(std::string const& value)
{
	for (size_t i = 0; i < value.length(); i++)
	{
		if ((value[i] > '9' || value[i] < '0' ) && value[i] != '.')
			return false;
		if (value[i] == '.' && (!value[i + 1] || value[i + 1] == '.' || i == 0))
			throw std::invalid_argument(" Parsing error, 'listen' ip address is not correct\n");
	}
	return true;
}

bool	isSocket(std::string const& value)
{
	int	flag = 0;
	
	for (size_t i = 0; i < value.length(); i++)
	{
		if ((value[i] > '9' || value[i] < '0' ) && value[i] != '.' && value[i] != ':')
			return false;
		if (value[i] == '.')
		{
			if (!value[i + 1] || value[i + 1] == '.' || value[i + 1] == ':' || i == 0)
				throw std::invalid_argument(" Parsing error, 'listen' socket address is not correct\n");
			flag++;
		}
		if (value[i] == ':')
		{
			if (!value[i + 1] || value[i + 1] == '.' || value[i + 1] == ':' || flag != 3)
				throw std::invalid_argument(" Parsing error, 'listen' socket address is not correct\n");
		}
	}
	return true;
}

int	strToInt(std::string const& value)
{
	int num;
    std::istringstream str(value);
    
	str >> num;
    if (str.fail())
	    throw std::runtime_error(" error to convert the string argument in int");
	return num;
}

bool    checkIpAddress(std::string const& value)
{
	int	tmp;
	size_t	j = 0;
	std::vector<std::string> nums;
	
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] > '9' || value[i] < '0')
		{
			nums.push_back(value.substr(j, i - j));
			j = i + 1;
		}
	}
	nums.push_back(value.substr(j));
	for (size_t i = 0; i < nums.size(); i++)
	{
 		if (i > 3)
			return false;
		if (nums[i].length() > 3)
		    return false;
		tmp = strToInt(nums[i]);
		if (tmp < 0 || tmp > 255)
			return false;		
	}
	return true;
}

bool    checkSocketAddress(std::string const& value)
{
	int	tmp;
	size_t	j = 0;
	std::vector<std::string> nums;
	
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] > '9' || value[i] < '0')
		{
			nums.push_back(value.substr(j, i - j));
			j = i + 1;
		}
	}
	nums.push_back(value.substr(j));
	for (size_t i = 0; i < nums.size(); i++)
	{
 		if (i > 4)
			return false;
		if (nums[i].length() > 5)
		    return false;
		tmp = strToInt(nums[i]);
		if (i < 4 && (tmp < 0 || tmp > 255))
			return false;
		if (i == 4 && (tmp < 0 || tmp > 65535))
			return false;
	}
	return true;
}

std::string	socketToIp(std::string const& value)
{
	std::string str = "";
	
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] == ':')
			str = value.substr(0, i);
	}
	return str;
}

int			socketToPort(std::string const& value)
{
	std::string str = "";
	
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] == ':')
			str = value.substr(i + 1);
	}
	int num = strToInt(str);
	return num;
}
