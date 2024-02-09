/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 22:10:40 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/09 03:18:33 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pid_list	*new_pid_list(int pid)
{
	t_pid_list	*new;

	new = malloc(sizeof(t_pid_list));
	if (!new)
		return (NULL);
	new->pid = pid;
	new->next = NULL;
	return (new);
}

t_pid_list	*add_pid_list(t_minishell *minishell, int pid)
{
	t_pid_list	*new;
	t_pid_list	*current;

	new = new_pid_list(pid);
	if (!new)
	{
		ft_dprintf(2, "minishell: malloc error");
		clear_exit(minishell);
	}
	if (!minishell || !minishell->pid_list)
		return (new);
	current = minishell->pid_list;
	while (current->next)
		current = current->next;
	current->next = new;
	return (minishell->pid_list);
}

void	clear_pid(t_minishell *minishell)
{
	t_pid_list	*tmp;
	t_pid_list	*list;

	if (!minishell->pid_list)
		return ;
	list = minishell->pid_list;
	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
	minishell->pid_list = 0;
}
