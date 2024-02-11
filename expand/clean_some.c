/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_some.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 05:48:40 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 05:10:49 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_real_len(char **args)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (args[i])
	{
		if (*args[i])
			len++;
		i++;
	}
	return (len);
}

void	rm_empty(t_node_ast *node)
{
	char	**res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * (get_real_len(node->args) + 1));
	if (!res)
		return ;
	while (node->args[i])
	{
		if (*node->args[i])
			res[j++] = ft_strdup(node->args[i]);
		i++;
	}
	res[j] = 0;
	clear_tab(node->args);
	node->args = res;
}
