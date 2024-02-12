/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:07 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 02:52:41 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	print_export(char *s, int len, t_env **res)
{
	int	index;

	index = 0;
	while (index < len)
	{
		if (res[index]->unset)
		{
			index++;
			continue ;
		}
		if (res[index]->value)
			s = ft_mprintf("export %s=\"%s\"\n", res[index]->key,
					res[index]->value);
		else
			s = ft_mprintf("export %s\n", res[index]->key, res[index]->value);
		if (!s)
			return (ft_dprintf(2, "minishell: malloc error\n"), 1);
		if (ft_write(s, ft_strlen(s), "export"))
			return (free(s), 1);
		free(s);
		++index;
	}
	return (0);
}

static inline int	display_export(t_env *env)
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
	s = 0;
	if (print_export(s, len, res))
		return (free(res), 1);
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
	{
		if (display_export(minishell->env))
			return (1);
		return (0);
	}
	i = 0;
	ret = 0;
	while (args[i])
	{
		ft_dprintf(2, "%s\n", args[i]);
		if (export_one(args[i], minishell))
			ret = 1;
		i++;
	}
	return (ret);
}
