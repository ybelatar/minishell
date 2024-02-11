/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:07 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 05:25:19 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	ft_env_len(t_env *env)
{
	int	len;

	if (!env)
		return (0);
	len = 0;
	while (env)
	{
		++len;
		env = env->next_env;
	}
	return (len);
}

static inline t_env	**ft_sort_env(t_env *env)
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
		index++;
	}
	while (index + 1 < len)
	{
		if (!res[index]->old && ft_strcmp(res[index]->key, res[index + 1]->key) > 0)
		{
			tmp = res[index];
			res[index] = res[index + 1];
			res[index + 1] = tmp;
			index = 0;
			continue ;
		}
		index++;
	}
	return (res);
}

static inline int	display_export(t_minishell *minishell, t_env *env)
{
	char	*s;
	t_env	**res;
	int		index;
	int		len;

	if (!env)
		return (0);
	res = ft_sort_env(env);
	if (!res)
		return (1);
	index = 0;
	len = ft_env_len(env);
	while (index < len)
	{
		if (res[index]->unset)
		{
			index++;
			continue ;
		}
		if (res[index]->value)
			s = ft_mprintf("export %s=\"%s\"\n", res[index]->key, res[index]->value);
		else
			s = ft_mprintf("export %s\n", res[index]->key, res[index]->value);
		if (!s)
		{
			clear_exit(minishell); 
			ft_dprintf(2, "minishell: malloc error\n");
			exit(1);
		}
		if (ft_write(s, ft_strlen(s), "export"))
			return (free(s), 1);
		free(s);
		index++;
	}
	free(res);
	return (0);
}

static inline int	export_one(char *str, t_minishell *minishell)
{
	char	*find;

	if (!str)
		return (1);
	if (!ft_isalpha(*str) && *str != '_')
		return (ft_dprintf(2,
				"minishell: export: `%s': not a valid identifier\n", str), 1);
	find = str;
	while (*find)
	{
		if (*find == '=')
			break ;
		else if (!ft_isalnum(*find) && *find != '_')
			return (ft_dprintf(2,
					"minishell: export: `%s': not a valid identifier\n", str),
				1);
		++find;
	}
	find = ft_strchr(str, '=');
	if (find)
	{
		*(find - 1) = 0;
		return (update_env(str, find, minishell), 0);
	}
	return (update_env(str, 0, minishell), 0);
}

int	export(char **args, t_minishell *minishell)
{
	int	i;
	int	ret;

	if (!*args)
		return (display_export(minishell, minishell->env), 0);
	i = 0;
	ret = 0;
	while (args[i])
	{
		if (export_one(args[i], minishell))
			ret = 1;
		i++;
	}
	return (ret);
}
