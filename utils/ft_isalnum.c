/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 02:29:24 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/05 17:50:11 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9'))
		return (1);
	return (0);
}

int	ft_isatoi_utils(int c)
{
	if ((c >= '0' && c <= '9') || (c == 32 || (c >= 9 && c <= 13)) || c == '+'
		|| c == '-')
		return (1);
	return (0);
}

int	ft_isatoi(char *str)
{
	size_t	i;
	int		sign;

	sign = 1;
	i = 0;
	if (*str == '-')
	{
		sign = -sign;
		++str;
	}
	else if (*str == '+')
		++str;
	while (*str >= '0' && *str <= '9')
	{
		i = i * 10 + *str - '0';
		if ((i > LONG_MAX && sign == 1) || (i - 1 > LONG_MAX && sign == -1))
			return (0);
		++str;
	}
	if (*str)
		return (0);
	return (1);
}
