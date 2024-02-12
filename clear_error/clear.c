/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 04:19:55 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 04:31:47 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_pretokens(t_pretoken **pretokens)
{
	t_pretoken	*tmp;

	if (!pretokens || !*pretokens)
		return ;
	while (*pretokens)
	{
		tmp = (*pretokens)->next_pretoken;
		free((*pretokens)->content);
		free(*pretokens);
		*pretokens = tmp;
	}
}

void	clear_tokens(t_token **tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		tmp = (*tokens)->next_token;
		free(*tokens);
		*tokens = tmp;
	}
}

void	clear_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	clear_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	while (env)
	{
		tmp = env->next_env;
		free(env->key);
		env->key = 0;
		free(env->value);
		env->value = 0;
		free(env);
		env = tmp;
	}
}

void	clear_redirs(t_node_ast *ast)
{
	t_redir_list	*tmp;
	t_redir_list	*current;

	if (!ast)
		return ;
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
}
