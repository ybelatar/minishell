/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 00:18:02 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 23:36:41 by wouhliss         ###   ########.fr       */
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

char	**ft_get_env(t_minishell *minishell)
{
	char	**env;
	t_env	*current;
	int		index;

	if (!minishell->env)
		return (0);
	env = malloc(sizeof(char *) * (ft_env_len(minishell->env) + 1));
	if (!env)
		return (0);
	current = minishell->env;
	index = 0;
	while (current)
	{
		if (!current->unset && current->value)
		{
			env[index] = ft_mprintf("%s=%s", current->key, current->value);
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

	if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3) || !path_env)
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
		if (!access(path, X_OK))
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
		if (current->pid > 0)
			waitpid(current->pid, &status, 0);
		if (current->pid > 0 && WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		else if (current->pid > 0 && WIFSIGNALED(status)
			&& WTERMSIG(status) == SIGQUIT)
			(ft_dprintf(2, "Quit (core dumped)\n"), g_status = 131);
		else if (current->pid > 0 && WIFSIGNALED(status)
			&& WTERMSIG(status) == SIGINT)
			(ft_dprintf(2, "\n"), g_status = 130);
		current = current->next;
	}
	clear_pid(minishell);
}
