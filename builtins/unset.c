/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:12 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/10 11:49:39 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(char **args, t_minishell *minishell)
{
	int	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
	{
		delete_env(args[i], minishell->env, minishell);
		i++;
	}
	return (0);
}
