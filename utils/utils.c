/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:27:07 by pibosc            #+#    #+#             */
/*   Updated: 2024/02/06 20:57:38 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	while ((unsigned char)s1[i]
// 		&& (unsigned char)s1[i] == (unsigned char)s2[i] && i < n)
// 		i++;
// 	if (i < n)
// 		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// 	return (0);
// }

int	ft_putstr_fd(char *s, int fd)
{
	if (!s)
	{
		write(fd, "(null)", 6);
		return (6);
	}
	else
		write(fd, s, ft_strlen(s));
	return (ft_strlen(s));
}

void	ft_close(int fd)
{
	if (fd > 2)
		close(fd);
}

char	*get_env(char *key, t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
			return (current->value);
		current = current->next_env;
	}
	return (0);
}

/*	Creates a prompt from the last exit code and the current working directory.
	Takes a pointer to a t_minishell struct to set its prompt variable.
	The prompt is allocated so it needs to be freed after its use.*/
void	ft_prompt(t_minishell *minishell)
{
	char	*cwd;

	cwd = getcwd(0, 0);
	if (cwd)
	{
		if (g_status)
			minishell->prompt = ft_strjoin("\e[1;91m➜ \e[1;96m", ft_strchrr(cwd,
						'/'));
		else
			minishell->prompt = ft_strjoin("\e[1;92m➜ \e[1;96m", ft_strchrr(cwd,
						'/'));
		free(cwd);
		minishell->prompt = ft_strjoin_free(minishell->prompt, "\e[0m ");
		return ;
	}
	if (g_status)
		minishell->prompt = ft_strjoin("\e[1;91m➜ \e[1;96m",
				"Minishell\e[0m$ ");
	else
		minishell->prompt = ft_strjoin("\e[1;92m➜ \e[1;96m",
				"Minishell\e[0m$ ");
}
