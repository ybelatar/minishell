/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:57:35 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/05 19:37:38 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print_redirs(t_node_ast *ast)
{
	t_redir_list	*current;

	if (!ast)
		return ;
	current = ast->redirs;
	while (current)
	{
		printf("redir = file : %s type : %d\n", current->file, current->type);
		current = current->next_redir;
	}
}

static void	ft_print_type(t_node_ast *ast)
{
	if (!ast)
		return ;
	printf("type = %d\n", ast->type);
}

static void	ft_print_args(t_node_ast *ast)
{
	char	**s;

	if (!ast)
		return ;
	if (!ast->args)
		return ;
	s = ast->args;
	while (*s)
		printf("arg = %s\n", *(s++));
}

void	ft_exec(t_node_ast *current)
{
	if (!current)
		return ;
	ft_print_type(current);
	ft_print_args(current);
	ft_print_redirs(current);
	printf("\n====================\n");
	ft_exec(current->left_child);
	ft_exec(current->right_child);
}
