/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 04:55:52 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 05:14:04 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*single_quote(char *str, int *i)
{
	int	start;

	start = *i;
	(*i)++;
	while (str[*i] != '\'')
		(*i)++;
	(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*normal(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && str[*i] != '"' && str[*i] != '$' && str[*i] != '\'')
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*expand_env_one(char *str, t_minishell *minishell, int led)
{
	char	*res;
	int		i;

	i = 0;
	res = NULL;
	while (str[i])
	{
		if (str[i] == '\'')
			res = ft_strjoin_free2(res, single_quote(str, &i));
		else if (str[i] == '"')
			res = ft_strjoin_free2(res, double_quote(str, &i, minishell));
		else if (str[i] == '$')
			res = ft_strjoin_free2(res, variable_env(str, &i, minishell));
		else
			res = ft_strjoin_free2(res, normal(str, &i));
	}
	if (!ft_strcmp(str, "~"))
		res = expand_tild(res, minishell);
	if (led)
		free(str);
	return (res);
}

void	expand_env_redir(t_redir_list *redir, t_minishell *minishell)
{
	free(redir->pre_file);
	redir->pre_file = redir->file;
	redir->file = expand_env_one(redir->file, minishell, 0);
}

void	expand_env(t_node_ast *node, t_minishell *minishell)
{
	int	i;

	if (!node->args)
		return ;
	i = 0;
	while (node->args[i])
	{
		node->args[i] = expand_env_one(node->args[i], minishell, 1);
		i++;
	}
}
