/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:23 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 04:21:24 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_env(t_env *env)
{
	if (!env)
		return ;
	while (env)
	{
		ft_dprintf(1, "%s=%s\n", env->key, env->value);
		env = env->next_env;
	}
}

void	update_env(char *key, char *value, t_minishell *minishell)
{
	t_env	*env;

	if (!minishell->env)
	{
		minishell->env = new_env(ft_strdup(key), ft_strdup(value), 0);
		return ;
	}
	env = minishell->env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return ;
		}
		if (!env->next_env)
		{
			env->next_env = new_env(ft_strdup(key), ft_strdup(value), 0);
			return ;
		}
		env = env->next_env;
	}
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
			if (!prev)
				minishell->env = env->next_env;
			else
				prev->next_env = env->next_env;
			free(env->key);
			free(env->value);
			free(env);
			return ;
		}
		prev = env;
		env = env->next_env;
	}
}
