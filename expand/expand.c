/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 03:18:50 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 05:36:07 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	expand_redir(t_redir_list *tmp, char **tab,
		t_minishell *minishell)
{
	while (tmp)
	{
		if (tmp->type != R_HEREDOC)
		{
			expand_env_redir(tmp, minishell);
			if (tmp->file && has_wildcard(tmp->file))
			{
				tab = sort_tab(expanded_wildcard(tmp->file));
				if (!tab || plen(tab) > 1)
				{
					clear_tab(tab);
					free(tmp->file);
					tmp->file = 0;
				}
				else
				{
					free(tmp->file);
					tmp->file = *tab;
					free(tab);
				}
			}
			strip_quotes_redir(tmp);
		}
		tmp = tmp->next_redir;
	}
}

void	ft_expand(t_node_ast *node, t_minishell *minishell)
{
	t_redir_list	*tmp;
	char			**tab;

	if (node->is_expanded)
		return ;
	if (node->args && node->type == T_CMD)
	{
		expand_env(node, minishell);
		rm_empty(node);
		split_no_quotes(node);
		wildcards(node);
		strip_quotes(node);
	}
	tmp = node->redirs;
	tab = 0;
	expand_redir(tmp, tab, minishell);
	node->is_expanded = 1;
}

void	ft_recexpand(t_node_ast *node, t_minishell *minishell)
{
	if (!node)
		return ;
	ft_expand(node, minishell);
	ft_recexpand(node->left_child, minishell);
	ft_recexpand(node->right_child, minishell);
}
