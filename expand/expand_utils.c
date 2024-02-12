/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 05:05:44 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/12 05:21:00 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_tild(char *str, t_minishell *minishell)
{
	char	*res;

	(void)str;
	res = ft_strdup(get_env("HOME", minishell->env));
	if (res)
		return (free(str), res);
	return (str);
}

void	negate_quotes(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == '"' || str[i] == '\'')
			str[i] = -str[i];
}

void	positive(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] < 0)
			str[i] = -str[i];
}
