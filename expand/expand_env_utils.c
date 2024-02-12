/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 05:04:50 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 05:06:53 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*variable_utils(char *str, int *i, t_minishell *minishell)
{
	int		start;
	char	*name;
	char	*value;

	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	name = ft_substr(str, start, *i - start);
	value = get_env(name, minishell->env);
	if (!value)
		return (free(name), ft_strdup(""));
	negate_quotes(value);
	return (free(name), ft_strdup(value));
}

int	is_last(char *str, int i)
{
	int	count;

	count = 0;
	while (str[i])
	{
		if (str[i] == '"')
			count++;
		i++;
	}
	return (count % 2);
}

char	*variable_env(char *str, int *i, t_minishell *minishell)
{
	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@' || str[*i] == '!')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(g_status));
	}
	if (str[*i] == '\'' || (str[*i] == '"' && !is_last(str, *i)))
		return (ft_strdup(""));
	if (!ft_isalpha(str[*i]) && str[*i] != '_')
		return (ft_strdup("$"));
	return (variable_utils(str, i, minishell));
}

char	*double_quote_reg(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] != '"' && str[*i] != '$')
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*double_quote(char *str, int *i, t_minishell *minishell)
{
	char	*res;

	res = NULL;
	(*i)++;
	res = ft_strjoin_free2(res, ft_strdup("\""));
	while (str[*i] != '"')
	{
		if (str[*i] == '$')
			res = ft_strjoin_free2(res, variable_env(str, i, minishell));
		else
			res = ft_strjoin_free2(res, double_quote_reg(str, i));
	}
	(*i)++;
	return (ft_strjoin_free2(res, ft_strdup("\"")));
}
