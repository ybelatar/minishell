/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:07 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/08 09:44:20 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	display_export(t_env *env)
{
	int	len;

	if (!env)
		return ;
	while (env)
	{
		if (write(1, "export ", 7) < 7)
			return (ft_dprintf(2, "minishell: export: write error: %s",
					strerror(errno)), 125);
		len = ft_strlen(env->key);
		if (write(1, env->key, len) < len)
			return (ft_dprintf(2, "minishell: export: write error: %s",
					strerror(errno)), 125);
		if (write(1, "=\"", 2) < 2)
			return (ft_dprintf(2, "minishell: export: write error: %s",
					strerror(errno)), 125);
		len = ft_strlen(env->value);
		if (write(1, env->value, len) < len)
			return (ft_dprintf(2, "minishell: export: write error: %s",
					strerror(errno)), 125);
		if (write(1, "\"\n", 2) < 2)
			return (ft_dprintf(2, "minishell: export: write error: %s",
					strerror(errno)), 125);
		env = env->next_env;
	}
	return (0);
}

int	print_justincase(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i == 0 && !ft_isalpha(str[i]) && str[i] != '_')
		{
			ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n",
				str);
			return (1);
		}
		else if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n",
				str);
			return (1);
		}
		i++;
	}
	return (0);
}

void	print_error(char *arg)
{
	ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", arg);
}

int	export_one(char *arg, t_minishell *minishell)
{
	int		i;
	char	*linus;
	char	*boom;

	if (ft_strchri(arg, '=') <= 0)
		return (print_justincase(arg));
	i = 0;
	while (i < ft_strchri(arg, '='))
	{
		if (i == 0 && !ft_isalpha(arg[i]) && arg[i] != '_')
			return (print_error(arg), 1);
		else if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (print_error(arg), 1);
		i++;
	}
	if (!arg[ft_strchri(arg, '=') + 1])
		return (0);
	linus = ft_substr(arg, 0, ft_strchri(arg, '='));
	boom = ft_substr(arg, ft_strchri(arg, '=') + 1, ft_strlen(arg));
	update_env(linus, boom, minishell);
	free(linus);
	free(boom);
	return (0);
}

int	export(char **args, t_minishell *minishell)
{
	int	i;
	int	ret;

	if (!*args)
		return (display_export(minishell->env), 0);
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
