/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 03:51:07 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 06:30:22 by wouhliss         ###   ########.fr       */
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
			c = str[i];
			i++;
			while (str[i] && str[i] != c)
			{
				++i;
				len++;
			}
			if (!str[i])
				break ;
		}
		else
		{
			i++;
			len++;
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

	res = malloc(ft_strlen_wquotes(str) + 1);
	if (!res)
	{
		if (led)
			free(str);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			c = str[i];
			i++;
			while (str[i] && str[i] != c)
				res[j++] = str[i++];
			if (!str[i])
				break ;
			i++;
		}
		else
			res[j++] = str[i++];
	}
	res[j] = 0;
	if (led)
		free(str);
	return (res);
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
		i++;
	}
}
