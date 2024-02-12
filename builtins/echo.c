/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:45:58 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 02:30:47 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_write(const char *str, int len, const char *cmd)
{
	if (write(1, str, len) < len)
		return (ft_dprintf(2, "minishell: %s: write error: %s\n",
				cmd, strerror(errno)), 1);
	return (0);
}

static inline int	is_option(const char *arg)
{
	if (*arg != '-')
		return (0);
	++arg;
	while (*arg)
	{
		if (*arg != 'n')
			return (0);
		++arg;
	}
	return (1);
}

int	echo(char **args, t_minishell *minishell)
{
	int	led;
	int	i;

	(void)minishell;
	if (!args || !*args)
		return (ft_write("\n", 1, "echo"));
	led = 0;
	i = 0;
	while (args[i] && is_option(args[i]))
	{
		led = 1;
		i++;
	}
	while (args[i])
	{
		if (ft_write(args[i], ft_strlen(args[i]), "echo"))
			return (1);
		if (args[i + 1] && ft_write(" ", 1, "echo"))
			return (1);
		i++;
	}
	if (!led && ft_write("\n", 1, "echo"))
		return (1);
	return (0);
}
