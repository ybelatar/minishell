/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:16:44 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/08 23:38:26 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_handle_in(t_minishell *minishell, t_cmd *cmd,
		t_redir_list *redir)
{
	int	fd;

	(void)minishell;
	(void)cmd;
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (-1);
	}
	dup2(fd, 0);
	ft_close(fd);
	return (fd);
}

static int	ft_handle_out(t_minishell *minishell, t_cmd *cmd,
		t_redir_list *redir)
{
	int	fd;

	(void)minishell;
	(void)cmd;
	fd = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (-1);
	}
	dup2(fd, 1);
	ft_close(fd);
	return (fd);
}

static int	ft_handle_heredoc(t_minishell *minishell, t_cmd *cmd,
		t_redir_list *redir)
{
	t_heredoc	heredoc;

	(void)minishell;
	(void)cmd;
	dup2(redir->fd, 0);
	ft_close(redir->fd);
	return (heredoc.in);
}

static int	ft_handle_append(t_minishell *minishell, t_cmd *cmd,
		t_redir_list *redir)
{
	int	fd;

	(void)minishell;
	(void)cmd;
	fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (-1);
	}
	dup2(fd, 1);
	close(fd);
	return (fd);
}

/*	Opens all redirections from a t_redir_list linked list
	to check their access.*/
int	ft_open_redirs(t_minishell *minishell, t_cmd *cmd, t_redir_list *redirs)
{
	t_redir_list	*current;

	if (!minishell)
		return (1);
	if (!redirs)
		return (0);
	current = redirs;
	while (current)
	{
		if (current->type == R_IN)
			cmd->fd_in = ft_handle_in(minishell, cmd, current);
		else if (current->type == R_OUT)
			cmd->fd_out = ft_handle_out(minishell, cmd, current);
		else if (current->type == R_HEREDOC)
			cmd->fd_in = ft_handle_heredoc(minishell, cmd, current);
		else if (current->type == R_APPEND)
			cmd->fd_out = ft_handle_append(minishell, cmd, current);
		if (cmd->fd_in < 0 || cmd->fd_out < 0)
			return (1);
		current = current->next_redir;
	}
	return (0);
}
