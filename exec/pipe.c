/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 22:10:40 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 02:21:22 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe_list	*new_pipe_list(int in, int out)
{
	t_pipe_list	*new;

	new = malloc(sizeof(t_pipe_list));
	if (!new)
		return (NULL);
	new->in = in;
	new->out = out;
	new->next = NULL;
	return (new);
}

t_pipe_list	*add_pipe_list(t_minishell *minishell, int in, int out)
{
	t_pipe_list	*new;
	t_pipe_list	*current;

	new = new_pipe_list(in, out);
	if (!new)
	{
		ft_dprintf(2, "minishell: malloc error\n");
		clear_exit(minishell);
	}
	if (!minishell || !minishell->pipe_list)
		return (new);
	current = minishell->pipe_list;
	while (current->next)
		current = current->next;
	current->next = new;
	return (minishell->pipe_list);
}

t_pipe_list	*get_last_pipe(t_pipe_list *list)
{
	if (!list)
		return (0);
	while (list->next)
		list = list->next;
	return (list);
}

t_pipe_list	*remove_last_pipe(t_pipe_list *list)
{
	if (!list)
		return (0);
	while (list->next)
	{
		if (!list->next->next)
		{
			free(list->next);
			list->next = 0;
			break ;
		}
		list = list->next;
	}
	return (list);
}

void	clear_pipe(t_minishell *minishell)
{
	t_pipe_list	*tmp;
	t_pipe_list	*list;

	if (!minishell->pipe_list)
		return ;
	list = minishell->pipe_list;
	while (list)
	{
		tmp = list->next;
		ft_close(list->in);
		ft_close(list->out);
		free(list);
		list = tmp;
	}
	minishell->pipe_list = 0;
}
