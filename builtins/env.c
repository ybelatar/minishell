/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:01 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/08 09:43:27 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_minishell *minishell)
{
	t_env	*env;
	int		len;

	if (!minishell->env)
		return (0);
	env = minishell->env;
	while (env)
	{
		len = ft_strlen(env->key);
		if (write(1, env->key, len) < len)
			return (ft_dprintf(2, "minishell: env: write error: %s\n",
				strerror(errno)), 125);
		if (!write(1, "=", 1))
			return (ft_dprintf(2, "minishell: env: write error: %s\n",
				strerror(errno)), 125);
		len = ft_strlen(env->value);
		if (write(1, env->value, len) < len)
			return (ft_dprintf(2, "minishell: env: write error: %s\n",
				strerror(errno)), 125);
		if (!write(1, "\n", 1))
			return (ft_dprintf(2, "minishell: env: write error: %s\n",
				strerror(errno)), 125);
		env = env->next_env;
	}
	return (0);
}
