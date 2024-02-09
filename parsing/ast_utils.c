/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 21:37:13 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 08:01:12 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PIPEPIPE || token->type == AND || token->type == OR)
		return (1);
	return (0);
}

int	is_redir(t_token *token)
{
	if (token->type >= 1 && token->type <= 4)
		return (1);
	return (0);
}

int	get_precedence(t_token_type type)
{
	if (type == OR || type == AND)
		return (0);
	return (1);
}

t_node_ast	*create_node(t_node_type type)
{
	t_node_ast	*node;

	node = ft_calloc(1, sizeof(t_node_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->left_child = NULL;
	node->right_child = NULL;
	node->is_expanded = 0;
	return (node);
}
