/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:16:44 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 02:19:34 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_handle_in(t_redir_list *redir)
{
	int	fd;

	if (is_ambiguous(redir))
		return (ft_dprintf(2, "minishell: %s: ambiguous redirect\n",
				redir->pre_file), -1);
	if (!redir->file)
		return (ft_dprintf(2, "minishell: : No such file or directory\n"), -1);
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
		return (ft_dprintf(2, "minishell: %s: %s\n", redir->file,
				strerror(errno)), -1);
	if (dup2(fd, 0) < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		ft_close(fd);
		return (-1);
	}
	ft_close(fd);
	return (fd);
}

static int	ft_handle_out(t_redir_list *redir)
{
	int	fd;

	if (is_ambiguous(redir))
		return (ft_dprintf(2, "minishell: %s: ambiguous redirect\n",
				redir->pre_file), -1);
	if (!redir->file)
		return (ft_dprintf(2, "minishell: : No such file or directory\n"), -1);
	fd = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		return (ft_dprintf(2, "minishell: %s: %s\n", redir->file,
				strerror(errno)), -1);
	if (dup2(fd, 1) < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		ft_close(fd);
		return (-1);
	}
	ft_close(fd);
	return (fd);
}

static int	ft_handle_heredoc(t_redir_list *redir)
{
	if (dup2(redir->fd, 0) < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		ft_close(redir->fd);
		return (-1);
	}
	ft_close(redir->fd);
	return (redir->fd);
}

static int	ft_handle_append(t_redir_list *redir)
{
	int	fd;

	if (is_ambiguous(redir))
		return (ft_dprintf(2, "minishell: %s: ambiguous redirect\n",
				redir->pre_file), -1);
	if (!redir->file)
		return (ft_dprintf(2, "minishell: : No such file or directory\n"), -1);
	fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
		return (ft_dprintf(2, "minishell: %s: %s\n", redir->file,
				strerror(errno)), -1);
	if (dup2(fd, 1) < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		ft_close(fd);
		return (-1);
	}
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
			cmd->fd_in = ft_handle_in(current);
		else if (current->type == R_OUT)
			cmd->fd_out = ft_handle_out(current);
		else if (current->type == R_HEREDOC)
			cmd->fd_in = ft_handle_heredoc(current);
		else if (current->type == R_APPEND)
			cmd->fd_out = ft_handle_append(current);
		if (cmd->fd_in < 0 || cmd->fd_out < 0)
			return (1);
		current = current->next_redir;
	}
	return (0);
}
