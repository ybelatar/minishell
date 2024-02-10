/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:23 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/10 12:03:06 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(char *key, char *value, t_minishell *minishell)
{
	t_env	*env;

	if (!minishell->env)
	{
		minishell->env = new_env(ft_strdup(key), ft_strdup(value), 0, 0);
		return ;
	}
	env = minishell->env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (env->old)
				env->unset = 0;
			return ;
		}
		if (!env->next_env)
		{
			env->next_env = new_env(ft_strdup(key), ft_strdup(value), 0, 0);
			return ;
		}
		env = env->next_env;
	}
}

static inline void	ft_del_env(t_env *env)
{
	if (!env)
		return ;
	free(env->key);
	free(env->value);
	free(env);
}

void	delete_env(char *key, t_env *env, t_minishell *minishell)
{
	t_env	*prev;

	if (!env)
		return ;
	prev = NULL;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			if (env->old)
			{
				env->unset = 1;
				return ;
			}
			if (!prev)
				minishell->env = env->next_env;
			else
				prev->next_env = env->next_env;
			ft_del_env(env);
			return ;
		}
		prev = env;
		env = env->next_env;
	}
}
