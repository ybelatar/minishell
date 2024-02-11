/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:03:59 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 21:32:11 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_heredoc(int sig)
{
	int	fd;

	if (sig == SIGINT)
	{
		fd = open("/dev/null", O_RDONLY);
		if (fd < 0)
			return ;
		dup2(fd, 0);
		ft_close(fd);
		ft_dprintf(2, "\n");
		g_status = 257;
	}
}

static inline int	handle_sig(t_minishell *minishell, int out)
{
	g_status = 130;
	free(minishell->expanded);
	dup2(minishell->fd, 0);
	ft_close(minishell->fd);
	ft_close(out);
	signal(SIGINT, SIG_IGN);
	return (1);
}

static inline int	routine(t_minishell *minishell, int led, int out)
{
	char	*s;

	while (1)
	{
		s = readline("> ");
		if (g_status == 257)
			return (handle_sig(minishell, out));
		if (!s)
			break ;
		if (!ft_strcmp(s, minishell->expanded))
		{
			signal(SIGINT, SIG_IGN);
			return (free(s), free(minishell->expanded), dup2(minishell->fd, 0),
				ft_close(out), ft_close(minishell->fd), 0);
		}
		if (led)
			s = expand_env_one(s, minishell, 1);
		if (s)
			write(out, s, ft_strlen(s));
		write(out, "\n", 1);
		free(s);
	}
	ft_dprintf(2, H, "minishell:", minishell->expanded);
	free(minishell->expanded);
	return (0);
}

int	ft_read(t_minishell *minishell, char *limiter, int out, int in)
{
	int	led;

	led = 0;
	if (!ft_strchr(limiter, '"') && !ft_strchr(limiter, '\''))
		led = 1;
	minishell->fd = dup(0);
	if (minishell->fd < 0)
	{
		ft_close(out);
		ft_close(in);
		clear_exit(minishell);
		ft_dprintf(2, "minishell: dup error\n");
		exit(1);
	}
	minishell->expanded = without_quotes(limiter, 0);
	signal(SIGINT, sig_heredoc);
	if (routine(minishell, led, out))
		return (1);
	dup2(minishell->fd, 0);
	signal(SIGINT, SIG_IGN);
	return (ft_close(out), ft_close(minishell->fd), 0);
}
