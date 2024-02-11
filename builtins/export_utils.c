/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:12:27 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 21:35:08 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	ft_sort(t_env **res, int len, t_env *tmp, int index)
{
	while (index + 1 < len && res[index + 1]->old)
	{
		if (ft_strcmp(res[index]->key, res[index + 1]->key) > 0)
		{
			tmp = res[index];
			res[index] = res[index + 1];
			res[index + 1] = tmp;
			index = 0;
			continue ;
		}
		++index;
	}
	while (index + 1 < len)
	{
		if (!res[index]->old && ft_strcmp(res[index]->key, res[index
					+ 1]->key) > 0)
		{
			tmp = res[index];
			res[index] = res[index + 1];
			res[index + 1] = tmp;
			index = 0;
			continue ;
		}
		++index;
	}
}

t_env	**ft_sort_env(t_env *env)
{
	t_env	**res;
	t_env	*tmp;
	int		len;
	int		index;

	len = ft_env_len(env);
	res = malloc(len * sizeof(t_env *));
	if (!res)
		return (0);
	index = 0;
	while (index < len)
	{
		res[index++] = env;
		env = env->next_env;
	}
	index = 0;
	tmp = 0;
	ft_sort(res, len, tmp, index);
	return (res);
}
