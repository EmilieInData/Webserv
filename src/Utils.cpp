/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:49:32 by esellier          #+#    #+#             */
/*   Updated: 2025/07/07 19:19:33 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

// >> listen

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
				throw std::invalid_argument(" Parsing error, 'listen' socket"
					" address is not correct\n");
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

// >> server_name

bool	checkDns(std::vector<std::string>& tmp)
{
	for (size_t i = 0; i < tmp.size(); i++)
	{
		for (size_t j = 0; j < tmp[i].length(); j++)
		{
			if ((tmp[i][j] >= '0' && tmp[i][j] <= '9') || (tmp[i][j] >= 'a' && tmp[i][j] <= 'z')
				|| (tmp[i][j] >= 'A' && tmp[i][j] <= 'Z') || tmp[i][j] == '.' || tmp[i][j] == '-')
				continue;
			return false;
		}
		if (!checkLabel(tmp[i]))
			return false;
	}
	return true;
}

bool	checkLabel(std::string const& str)
{
	std::vector<std::string>	tmp;
	
	if (str.length() > 255)
		throw std::invalid_argument(" Parsing error, 'server_name' arguments"
			" may not exceed 255 characters\n");
	int j = 0;
	for (size_t i = 0; i < str.length(); i++)//cutted in label in a vector
	{
		if (str[i] == '.')
		{
			if (i == 0 || i == str.length() - 1)
				throw std::invalid_argument(" Parsing error, 'server_name' arguments"
					" cannot begin or end by '.'\n");
			if(str[i + 1] && str[i + 1] == '.')
				throw std::invalid_argument(" Parsing error, 'server_name' arguments"
					" cannot have two consecutive '.'\n");
			tmp.push_back(str.substr(j, i - j));
			j = i + 1;
		}
	}
	if (tmp.empty())
		j = 0;
	tmp.push_back(str.substr(j));
	for (size_t i = 0; i < tmp.size(); i++)//checking labels
	{
		if (tmp[i].length() > 63)
			throw std::invalid_argument(" Parsing error, 'server_name' argument's"
				" labels may not exceed 63 characters\n");
		if (tmp[i][0] == '-' || tmp[i][tmp[i].length() - 1] == '-')
			throw std::invalid_argument(" Parsing error, 'server_name' argument's"
				" labels cannot begin or end by '-'\n");
	}
	return true;
}

 unsigned int	strToSize(std::string const& value)
{
	std::string tmp;

	for(size_t i = 0; i < value.length(); i++)
	{
		if (value[i] < '0' || value[i] > '9')
		{
			tmp = value.substr(0, i);
			if (i + 1 != value.length())
				throw std::invalid_argument(" Parsing error, wrong client_max_body_size' arguments\n");
		}
	}
	unsigned int num = strToInt(tmp);
	if (value[value.length() - 1] == 'k' || value[value.length() - 1] == 'K')
		num = num * 1024;
	else if (value[value.length() - 1] == 'm' || value[value.length() - 1] == 'M')
		num = num * 1024 * 1024;
	else
		throw std::invalid_argument(" Parsing error, wrong client_max_body_size' arguments\n");
	return (num);
}
