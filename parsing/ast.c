/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 21:00:44 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 21:50:11 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node_ast	*parse_expression(t_minishell *minishell, t_token **tokens,
				int min_precedence);

t_node_ast	*parse_command(t_minishell *minishell, t_token **token)
{
	t_node_ast	*parsed;

	parsed = create_node(T_CMD);
	if (!parsed)
		return (ft_dprintf(2, "minishell: malloc error\n"), NULL);
	while (*token && ((*token)->type == WORD || is_redir(*token)))
	{
		if ((*token)->type == WORD)
		{
			parsed->args = join_tab(parsed->args, (*token)->content);
			move_def_token(token, 0);
		}
		else
			add_redir(minishell, parsed, token);
	}
	return (parsed);
}

t_node_ast	*parse_term(t_minishell *minishell, t_token **token)
{
	t_node_ast	*parsed;

	if (!token)
		return (0);
	if ((*token)->type == OPEN_P)
	{
		move_def_token(token, 1);
		parsed = parse_expression(minishell, token, 0);
		move_def_token(token, 1);
		return (parsed);
	}
	else
		return (parse_command(minishell, token));
}

t_node_ast	*fuse_node(t_node_type type, t_node_ast *left_node,
		t_node_ast *right_node)
{
	t_node_ast	*node;

	node = ft_calloc(1, sizeof(t_node_ast));
	if (!node)
		return (ft_dprintf(2, "minishell: malloc error"), NULL);
	node->type = type;
	node->left_child = left_node;
	node->right_child = right_node;
	return (node);
}

t_node_ast	*parse_expression(t_minishell *minishell, t_token **tokens,
		int min_precedence)
{
	t_node_ast		*left_node;
	t_node_ast		*right_node;
	t_token_type	ope_type;

	if (!tokens)
		return (0);
	left_node = parse_term(minishell, tokens);
	if (!left_node)
		return (NULL);
	while (*tokens && is_operator(*tokens)
		&& get_precedence((*tokens)->type) >= min_precedence)
	{
		ope_type = (*tokens)->type;
		move_def_token(tokens, 1);
		right_node = parse_expression(minishell, tokens,
				get_precedence(ope_type) + 1);
		if (!right_node)
			return (left_node);
		left_node = fuse_node(get_type(ope_type), left_node, right_node);
		if (!left_node)
			return (NULL);
	}
	return (left_node);
}

t_node_ast	*parser(t_minishell *minishell, t_token *tokens)
{
	t_node_ast	*ast;

	if (!tokens)
		return (0);
	ast = parse_expression(minishell, &tokens, 0);
	if (!ast)
		return (clear_tokens(&tokens), NULL);
	clear_tokens(&tokens);
	return (ast);
}
