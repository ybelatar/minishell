/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 21:37:04 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 09:08:36 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type	get_redir_type(t_token_type type)
{
	if (type == REDIRECT_IN)
		return (R_IN);
	if (type == REDIRECT_OUT)
		return (R_OUT);
	if (type == HEREDOC)
		return (R_HEREDOC);
	if (type == REDIRECT_APPEND)
		return (R_APPEND);
	return (R_IN);
}

int	add_redir(t_minishell *minishell, t_node_ast *node, t_token **token)
{
	t_token_type	type;
	t_redir_list	*new;

	(void)minishell;
	type = (*token)->type;
	move_def_token(token, 1);
	new = create_redir(get_redir_type(type), (*token)->content);
	if (!new)
		return (ft_dprintf(2, "minishell: malloc error\n"), 0);
	add_last_redir(node, new);
	move_def_token(token, 1);
	return (1);
}

t_redir_list	*create_redir(t_redir_type type, char *file)
{
	t_redir_list	*redir;

	redir = ft_calloc(1, sizeof(t_redir_list));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->pre_file = 0;
	if (!redir->file)
		return (free(redir), NULL);
	return (redir);
}

void	add_last_redir(t_node_ast *node, t_redir_list *new)
{
	t_redir_list	*lst;

	if (!node->redirs)
		node->redirs = new;
	else
	{
		lst = node->redirs;
		while (lst && lst->next_redir)
			lst = lst->next_redir;
		lst->next_redir = new;
	}
}
