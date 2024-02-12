/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 22:42:39 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 04:31:51 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_ast(t_node_ast **ast)
{
	t_node_ast	*tmp_l;
	t_node_ast	*tmp_r;

	if (!ast || !(*ast))
		return ;
	tmp_l = (*ast)->left_child;
	tmp_r = (*ast)->right_child;
	if ((*ast)->type == T_CMD)
	{
		clear_tab((*ast)->args);
		(*ast)->args = 0;
		clear_redirs(*ast);
		(*ast)->redirs = 0;
	}
	free(*ast);
	*ast = 0;
	clear_ast(&tmp_l);
	clear_ast(&tmp_r);
}

void	clear_all_redirs(t_node_ast *lst)
{
	t_node_ast		*ast;
	t_redir_list	*current;
	t_redir_list	*tmp;

	if (!lst)
		return ;
	ast = lst;
	current = ast->redirs;
	while (current)
	{
		tmp = current->next_redir;
		ft_close(current->fd);
		free(current->file);
		free(current->pre_file);
		free(current);
		current = tmp;
	}
	ast->redirs = 0;
	clear_all_redirs(lst->left_child);
	clear_all_redirs(lst->right_child);
}

void	move_def(t_pretoken **pretoken, int i)
{
	t_pretoken	*tmp;

	tmp = (*pretoken)->next_pretoken;
	if (i)
		free((*pretoken)->content);
	free(*pretoken);
	*pretoken = tmp;
}

void	move_def_token(t_token **token, int i)
{
	t_token	*tmp;

	tmp = (*token)->next_token;
	if (i)
		free((*token)->content);
	free(*token);
	*token = tmp;
}
