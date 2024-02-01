/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:03 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/01 20:42:20 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "exec.h"

void	clear_exit(t_minishell *minishell)
{
	clear_ast(&minishell->ast);
	clear_env(minishell->env);
	ft_close(minishell->of);
	rl_clear_history();
}

int	exit_minishell(char **tab, t_minishell *minishell)
{
	int	i;
	int	exit_value;

	printf("exit\n");
	if (!tab || !*tab)
	{
		clear_exit(minishell);
		exit(0);
	}
	if (!ft_isatoi(*tab))
	{
		ft_dprintf(2, "minishell: exit : %s: numeric argument required\n",
			*tab);
		clear_exit(minishell);
		exit(2);
	}
	i = 0;
	if (tab[i + 1])
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	exit_value = ft_atoi(*tab);
	clear_exit(minishell);
	exit(exit_value);
}
