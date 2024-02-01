/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:27:07 by pibosc            #+#    #+#             */
/*   Updated: 2024/02/01 20:42:40 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
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

void	ft_prompt(t_minishell *minishell)
{
	char	*cwd;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	minishell->of = dup(1);
	cwd = getcwd(0, 0);
	if (cwd)
	{
		minishell->prompt = ft_strjoin_free2(ft_strdup("\e[1;96m"), cwd);
		minishell->prompt = ft_strjoin_free(minishell->prompt, "\e[0m ");
		minishell->cmd_line = readline(minishell->prompt);
		free(minishell->prompt);
	}
	else
		minishell->cmd_line = readline("\e[1;96mMinishell\e[0m$ ");
}

void	ft_close_fd(t_exec *data)
{
	if (data->fd_out != STDOUT_FILENO)
	{
		ft_close(data->fd_out);
		data->fd_out = STDOUT_FILENO;
	}
}
