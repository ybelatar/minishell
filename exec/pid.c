/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 22:10:40 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/06 22:15:24 by wouhliss         ###   ########.fr       */
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

t_pid_list	*add_pid_list(t_pid_list *list, int pid)
{
	t_pid_list	*new;
	t_pid_list	*current;

	new = new_pid_list(pid);
	if (!new)
		return (NULL);
	if (!list)
		return (new);
	current = list;
	while (current->next)
		current = current->next;
	current->next = new;
	return (list);
}

void	clear_pid(t_pid_list *list)
{
	t_pid_list	*tmp;

	if (!list)
		return ;
	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
}
