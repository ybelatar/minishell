/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 02:54:04 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 03:44:05 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	sig_fork(t_cmd *cmd)
{
	cmd->pid = fork();
	if (!cmd->pid)
	{
		signal(SIGINT, fork_sig_handler);
		signal(SIGQUIT, fork_sig_handler);
	}
}

static inline int	ft_child(t_minishell *minishell, t_cmd *cmd,
		t_node_ast *ast, t_builtin *fct)
{
	int	status;

	if (ft_open_redirs(minishell, cmd, ast->redirs))
		return (clear_ast(&minishell->ast), clear_env(minishell->env),
			clear_pid(minishell), 1);
	status = (*fct)(ast->args + 1, minishell);
	return (clear_ast(&minishell->ast), clear_env(minishell->env),
		clear_pid(minishell), status);
}

static inline void	ft_handle_pipe(t_minishell *minishell, t_cmd *cmd,
		int *pipedes)
{
	t_pipe_list	*lst;

	if (!cmd->pipe_type)
		return ;
	lst = get_last_pipe(minishell->pipe_list);
	if (cmd->pipe_type == 1)
	{
		pipedes[1] = lst->out;
		lst->out = -1;
	}
	else if (cmd->pipe_type == 2)
	{
		pipedes[0] = lst->in;
		lst->in = -1;
		remove_last_pipe(minishell->pipe_list);
	}
	else if (cmd->pipe_type == 3)
	{
		pipedes[0] = lst->in;
		lst->in = -1;
		lst = remove_last_pipe(minishell->pipe_list);
		pipedes[1] = lst->out;
		lst->out = -1;
	}
}

static inline void	ft_builtin_pipe(t_minishell *minishell, t_node_ast *ast,
		t_cmd *cmd, t_builtin *fct)
{
	int	pipedes[2];

	pipedes[0] = -1;
	pipedes[1] = -1;
	ft_handle_pipe(minishell, cmd, pipedes);
	sig_fork(cmd);
	if (cmd->pid < 0)
		return (ft_dprintf(2, "minishell: fork error\n"), clear_exit(minishell));
	else if (!cmd->pid)
	{
		clear_pipe(minishell);
		if (pipedes[0] > 0)
		{
			dup2(pipedes[0], 0);
			ft_close(pipedes[0]);
		}
		if (pipedes[1] > 0)
		{
			dup2(pipedes[1], 1);
			ft_close(pipedes[1]);
		}
		exit(ft_child(minishell, cmd, ast, fct));
	}
	if (cmd->pid)
		minishell->pid_list = add_pid_list(minishell, cmd->pid);
	ft_close(pipedes[0]);
	ft_close(pipedes[1]);
}

void	ft_exec_builtin(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd,
		t_builtin *fct)
{
	if (cmd->pipe_type)
		ft_builtin_pipe(minishell, ast, cmd, fct);
	else
	{
		minishell->in = dup(0);
		minishell->out = dup(1);
		if (ft_open_redirs(minishell, cmd, ast->redirs))
		{
			dup2(minishell->in, 0);
			dup2(minishell->out, 1);
			ft_close(minishell->in);
			ft_close(minishell->out);
			minishell->in = -1;
			minishell->out = -1;
			g_status = 1;
			return ;
		}
		g_status = (*fct)(ast->args + 1, minishell);
		dup2(minishell->in, 0);
		dup2(minishell->out, 1);
		ft_close(minishell->in);
		ft_close(minishell->out);
		minishell->in = -1;
		minishell->out = -1;
		minishell->pid_list = add_pid_list(minishell, -1);
	}
}
