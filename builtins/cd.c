/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:45:56 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 21:35:37 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	handle_oldpwd(t_minishell *minishell, char *tmp, char *tmp1)
{
	char	*s;

	s = get_env("OLDPWD", minishell->env);
	if (s)
	{
		if (!getcwd(tmp, PATH_MAX))
			return (1);
		if (chdir(s) == -1)
			return (ft_dprintf(2, "minishell : cd: %s: %s\n", s,
					strerror(errno)), 1);
		if (!getcwd(tmp1, PATH_MAX))
			return (1);
		update_env("OLDPWD", tmp, minishell);
		update_env("PWD", getcwd(tmp1, PATH_MAX), minishell);
		if (ft_write(tmp, ft_strlen(tmp), "cd"))
			return (1);
		if (ft_write("\n", 1, "cd"))
			return (1);
		return (0);
	}
	return (ft_dprintf(2, "minishell: cd: OLDPWD not set\n"), 1);
}

static inline int	handle_no_args(t_minishell *minishell, char *tmp,
		char *tmp1)
{
	char	*s;

	s = get_env("HOME", minishell->env);
	if (s)
	{
		if (!getcwd(tmp, PATH_MAX))
			return (1);
		if (chdir(s) == -1)
			return (ft_dprintf(2, "minishell : cd: %s: %s\n", s,
					strerror(errno)), 1);
		if (!getcwd(tmp1, PATH_MAX))
			return (1);
		update_env("OLDPWD", tmp, minishell);
		update_env("PWD", getcwd(tmp1, PATH_MAX), minishell);
		return (0);
	}
	return (ft_dprintf(2, "minishell: cd: HOME not set\n"), 1);
}

int	cd(char **args, t_minishell *minishell)
{
	char	tmp[PATH_MAX];
	char	tmp1[PATH_MAX];

	if (!args || !*args)
		return (handle_no_args(minishell, tmp, tmp1));
	if (*(args + 1))
		return (ft_dprintf(2, "minishell: cd: too many arguments\n"), 1);
	if (!ft_strcmp(*args, "-"))
		return (handle_oldpwd(minishell, tmp, tmp1));
	if (!getcwd(tmp, PATH_MAX))
		return (1);
	if (chdir(*args) == -1)
		return (ft_dprintf(2, "minishell : cd: %s: %s\n", *args,
				strerror(errno)), 1);
	if (!getcwd(tmp1, PATH_MAX))
		return (1);
	update_env("OLDPWD", tmp, minishell);
	update_env("PWD", getcwd(tmp1, PATH_MAX), minishell);
	return (0);
}
