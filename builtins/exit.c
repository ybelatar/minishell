/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:03 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 03:15:31 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_exit(t_minishell *minishell)
{
	clear_ast(&minishell->ast);
	clear_env(minishell->env);
	clear_pid(minishell);
	clear_pipe(minishell);
	rl_clear_history();
}

int	exit_minishell(char **args, t_minishell *minishell)
{
	int	i;
	int	exit_value;

	ft_dprintf(2, "exit\n");
	if (!args || !*args)
	{
		clear_exit(minishell);
		exit(0);
	}
	if (!ft_isatoi(*args))
	{
		ft_dprintf(2, "minishell: exit : %s: numeric argument required\n",
			*args);
		clear_exit(minishell);
		exit(2);
	}
	i = 0;
	if (args[i + 1])
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	exit_value = ft_atoi(*args);
	clear_exit(minishell);
	exit(exit_value);
}
