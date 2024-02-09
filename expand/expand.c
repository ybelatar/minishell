/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 03:18:50 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 09:23:48 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void ft_expand(t_node_ast *node, t_minishell *minishell)
{
	t_redir_list *tmp;
	
	if (node->args && node->type == T_CMD && !node->is_expanded)
	{
		expand_env(node, minishell);
		rm_empty(node);
		split_no_quotes(node);
		wildcards(node);
		strip_quotes(node);
	}
	tmp = node->redirs;
	while (tmp)
	{
		if (tmp->type != R_HEREDOC)
		{
			expand_env_redir(tmp, minishell);
			strip_quotes_redir(tmp);
		}
		tmp = tmp->next_redir;
	}
	node->is_expanded = 1;
}

void ft_recexpand(t_node_ast *node, t_minishell *minishell)
{
	if (!node)
		return ;
	ft_expand(node, minishell);
	ft_recexpand(node->left_child, minishell);
	ft_recexpand(node->right_child, minishell);
}