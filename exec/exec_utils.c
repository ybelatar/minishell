/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 00:18:02 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 01:49:45 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

static int	ft_env_len(t_minishell *minishell)
{
	int		index;
	t_env	*env;

	if (!minishell->env)
		return (0);
	index = 0;
	env = minishell->env;
	while (env)
	{
		++index;
		env = env->next_env;
	}
	return (index);
}

char	**ft_get_env(t_minishell *minishell)
{
	char	**env;
	t_env	*current;
	int		index;

	if (!minishell->env)
		return (0);
	env = malloc(sizeof(char *) * (ft_env_len(minishell) + 1));
	if (!env)
		return (0);
	current = minishell->env;
	index = 0;
	while (current)
	{
		if (!current->unset && current->value)
		{
			env[index] = ft_strjoin(current->key, "=");
			if (!env[index])
				return (ft_free_tab(env), (char **)0);
			env[index] = ft_strjoin_free(env[index], current->value);
			if (!env[index++])
				return (ft_free_tab(env), (char **)0);
		}
		current = current->next_env;
	}
	env[index] = 0;
	return (env);
}

char	*ft_get_bin(t_minishell *minishell, char *cmd)
{
	const char	*path_env = get_env("PATH", minishell->env);
	char		**tab;
	char		*path;
	int			index;

	if (!ft_strncmp(cmd, "./", 2) || !path_env)
		return (ft_strdup(cmd));
	tab = ft_split(path_env, ':');
	if (!tab)
		return (0);
	index = 0;
	while (tab[index])
	{
		path = ft_strjoin(tab[index++], "/");
		if (!path)
			return (ft_free_tab(tab), (char *)0);
		path = ft_strjoin_free(path, cmd);
		if (!path)
			return (ft_free_tab(tab), (char *)0);
		if (access(path, X_OK) == 0)
			return (ft_free_tab(tab), path);
		free(path);
	}
	return (ft_free_tab(tab), (char *)0);
}

void	ft_wait(t_minishell *minishell)
{
	t_pid_list	*current;
	int			status;

	if (!minishell->pid_list)
		return ;
	current = minishell->pid_list;
	while (current)
	{
		if (current->pid < 0)
		{
			current = current->next;
			continue ;
		}
		waitpid(current->pid, &status, 0);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		{
			ft_dprintf(2, "Quit (core dumped)\n");
			g_status = 131;
		}
		current = current->next;
	}
	clear_pid(minishell);
}
