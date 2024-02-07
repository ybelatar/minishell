/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:45:58 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/07 02:36:15 by wouhliss         ###   ########.fr       */
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

int	echo(char **tab)
{
	int	led;
	int	i;

	if (!tab || !*tab)
		return (0);
	led = 0;
	i = 0;
	while (tab[i] && is_option(tab[i]))
	{
		led = 1;
		i++;
	}
	while (tab[i])
	{
		if (!tab[i + 1])
			printf("%s", tab[i]);
		else
			printf("%s ", tab[i]);
		i++;
	}
	if (!led)
		printf("\n");
	return (0);
}
