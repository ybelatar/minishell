/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 05:04:50 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 09:09:52 by ybelatar         ###   ########.fr       */
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
	return (free(name), ft_strdup(value));
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
	if (str[*i] == '\'' || str[*i] == '"')
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
