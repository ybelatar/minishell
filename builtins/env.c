/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:01 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 03:33:49 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **args, t_minishell *minishell)
{
	t_env		*env;
	char		*s;

	(void)args;
	if (!minishell->env)
		return (0);
	env = minishell->env;
	while (env)
	{
		if (!env->value || env->unset)
		{
			env = env->next_env;
			continue ;
		}
		s = ft_mprintf("%s=%s\n", env->key, env->value);
		if (!s)
			return (clear_exit(minishell), ft_dprintf(2,
					"minishell: malloc error\n"), 3);
		if (ft_write(s, ft_strlen(s), "env"))
			return (free(s), 1);
		free(s);
		env = env->next_env;
	}
	return (0);
}
