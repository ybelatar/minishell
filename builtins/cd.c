/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:45:56 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/10 06:37:17 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(char **args, t_minishell *minishell)
{
	char	tmp[2048];
	char	tmp1[2048];
	char	*s;

	if (!args || !*args)
	{
		s = get_env("HOME", minishell->env);
		if (s)
		{
			if (!getcwd(tmp, 2048))
				return (1);
			if (chdir(s) == -1)
				return (ft_dprintf(2, "minishell : cd: %s: %s\n", s,
						strerror(errno)), 1);
			if (!getcwd(tmp1, 2048))
				return (1);
			update_env("OLDPWD", tmp, minishell);
			update_env("PWD", getcwd(tmp1, 2048), minishell);
			return (0);
		}
		return (ft_dprintf(2, "minishell: cd: HOME not set\n"), 1);
	}
	if (*(args + 1))
		return (ft_dprintf(2, "minishell: cd: too many arguments\n"), 1);
	if (!ft_strcmp(*args, "-"))
	{
		s = get_env("OLDPWD", minishell->env);
		if (s)
		{
			if (!getcwd(tmp, 2048))
				return (1);
			if (chdir(s) == -1)
				return (ft_dprintf(2, "minishell : cd: %s: %s\n", s,
						strerror(errno)), 1);
			if (!getcwd(tmp1, 2048))
				return (1);
			update_env("OLDPWD", tmp, minishell);
			update_env("PWD", getcwd(tmp1, 2048), minishell);
			if (ft_write(tmp, ft_strlen(tmp), "cd"))
				return (125);
			if (ft_write("\n", 1, "cd"))
				return (125);
			return (0);
		}
		return (ft_dprintf(2, "minishell: cd: OLDPWD not set\n"), 1);
	}
	if (!getcwd(tmp, 2048))
		return (1);
	if (chdir(*args) == -1)
		return (ft_dprintf(2, "minishell : cd: %s: %s\n", *args,
				strerror(errno)), 1);
	if (!getcwd(tmp1, 2048))
		return (1);
	update_env("OLDPWD", tmp, minishell);
	update_env("PWD", getcwd(tmp1, 2048), minishell);
	return (0);
}
