/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:01:19 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 05:20:46 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_ambiguous(t_redir_list *redir)
{
	return ((!redir->file && ft_strchr(redir->pre_file, '*')) || (redir->file
			&& !(*redir->file) && !ft_strncmp(redir->pre_file, "$", 1)));
}

int	has_wildcard(char *str)
{
	int		i;
	char	c;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			c = str[i];
			i++;
			while (str[i] && str[i] != c)
				i++;
			if (!str[i])
				break ;
		}
		else if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

static inline void	insert_utils(char **args, char **tab, int *i)
{
	(*i) += plen(tab);
	free(args);
	free(tab);
}

char	**insert_tab_in_tab(char **args, char **tab, int *i)
{
	char	**res;
	int		j;
	int		k;
	int		l;

	res = malloc(sizeof(char *) * (plen(args) + plen(tab) + 1));
	if (!res)
		return ((*i)++, args);
	j = 0;
	k = 0;
	l = 0;
	while (j < plen(args) + plen(tab) - 1)
	{
		if (j == *i)
		{
			free(args[k++]);
			while (tab && tab[l])
				res[j++] = tab[l++];
		}
		else
			res[j++] = args[k++];
	}
	res[j] = 0;
	insert_utils(args, tab, i);
	return (res);
}

void	wildcards(t_node_ast *node)
{
	int	i;

	i = 0;
	while (node->args[i])
	{
		if (has_wildcard(node->args[i]))
			node->args = insert_tab_in_tab(node->args,
					sort_tab(expanded_wildcard(node->args[i])), &i);
		else
			i++;
	}
}
