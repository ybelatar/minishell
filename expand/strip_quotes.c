/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 03:51:07 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 05:35:02 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen_wquotes(char *str)
{
	int		i;
	int		len;
	char	c;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			c = str[i++];
			while (str[i] && str[i] != c)
			{
				++i;
				++len;
			}
		}
		else
		{
			++i;
			++len;
		}
	}
	return (len);
}

char	*without_quotes(char *str, int led)
{
	char	*res;
	int		i;
	int		j;
	char	c;

	res = ft_calloc(ft_strlen_wquotes(str) + 1, 1);
	if (!res && led)
		return (free(str), NULL);
	else if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		c = str[i++];
		while ((c == '"' || c == '\'') && str[i] && str[i] != c)
			res[j++] = str[i++];
		if (c != '"' && c != '\'')
			res[j++] = c;
	}
	if (led)
		free(str);
	return (res);
}

void	strip_quotes_tab(char **tab)
{
	if (!tab)
		return ;
	while (*tab)
	{
		*tab = without_quotes(*tab, 1);
		if (!*tab)
			return ;
		++tab;
	}
}

void	strip_quotes_redir(t_redir_list *redir)
{
	if (!redir->file)
		return ;
	redir->file = without_quotes(redir->file, 1);
}

void	strip_quotes(t_node_ast *node)
{
	int	i;

	i = 0;
	while (node->args[i])
	{
		node->args[i] = without_quotes(node->args[i], 1);
		if (!node->args[i])
			return ;
		positive(node->args[i]);
		i++;
	}
}
