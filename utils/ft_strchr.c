/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 02:25:43 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/05 19:34:49 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchrr(char *s, char c)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		++i;
	if (i < 2)
		return (s);
	while (i > 0)
	{
		if (s[i] == c)
			return (s + i + 1);
		--i;
	}
	return (s);
}

char	*ft_strchr(char *s, char c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			return ((char *)(s + i + 1));
		}
		i++;
	}
	if (s[i] == c)
		return ((char *)(s + i));
	return (NULL);
}

int	ft_strchri(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			return (i);
		}
		i++;
	}
	if (s[i] == c)
		return (i);
	return (-1);
}
