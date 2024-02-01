/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:40:45 by pibosc            #+#    #+#             */
/*   Updated: 2024/02/01 19:27:36 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
		rl_redisplay();
	}
}

void	fork_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		g_status = 131;
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	heredoc_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		ft_dprintf(STDERR_FILENO, "\n");
		g_status = 130;
		garbage_collector(NULL, NULL, 1);
		exit(g_status);
	}
}
