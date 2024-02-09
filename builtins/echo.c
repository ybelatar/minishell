/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:45:58 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 04:12:01 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_option(char *arg)
{
	int	i;

	if (ft_strlen(arg) < 2)
		return (0);
	i = 0;
	if (arg[i++] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo(char **args, t_minishell *minishell)
{
	int	led;
	int	i;
	int	len;

	(void)minishell;
	if (!args || !*args)
		return (0);
	led = 0;
	i = 0;
	while (args[i] && is_option(args[i]))
	{
		led = 1;
		i++;
	}
	while (args[i])
	{
		len = ft_strlen(args[i]);
		if (write(1, args[i], len) < len)
			return (ft_dprintf(2, "minishell: echo: write error: %s\n",
					strerror(errno)), 125);
		if (args[i + 1])
			if (!write(1, " ", 1))
				return (ft_dprintf(2, "minishell: echo: write error: %s\n",
						strerror(errno)), 125);
		i++;
	}
	if (!led)
		if (!write(1, "\n", 1))
			return (ft_dprintf(2, "minishell: echo: write error: %s\n",
					strerror(errno)), 125);
	return (0);
}
