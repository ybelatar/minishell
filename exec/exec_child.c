/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 00:19:57 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/12 00:49:06 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	ft_handle_err(t_node_ast *ast)
{
	struct stat	path_stat;

	stat(ast->args[0], &path_stat);
	// if ((!ft_strncmp(ast->args[0], "./", 2) || !ft_strncmp(ast->args[0],
	//			"../",
	// 			3) || ft_get_last_char(ast->args[0]) == '/')
	// 	&& access(ast->args[0], F_OK)) return (ft_dprintf(2, "minishell: %s:
	// 		%s\n", ast->args[0], strerror(errno)), 127);
	// else if (!access(ast->args[0], F_OK) && (!ft_strncmp(ast->args[0], "./",
	//			2)
	// 		|| !ft_strncmp(ast->args[0], "../", 3)
	// 		|| ft_get_last_char(ast->args[0]) == '/')
	// 	&& S_ISDIR(path_stat.st_mode)) return (ft_dprintf(2, "minishell: %s:
	// 		%s\n", ast->args[0], "Is a directory"), 126);
	// else if ((!ft_strncmp(ast->args[0], "./", 2) || !ft_strncmp(ast->args[0],
	// 			"../", 3) || ft_get_last_char(ast->args[0]) == '/')
	// 	&& !access(ast->args[0], F_OK) && access(ast->args[0],
	// 		X_OK)) return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
	// 		strerror(errno)), 127);
	// else if (access(ast->args[0], F_OK) || access(ast->args[0],
	// 		X_OK)) return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
	// 		"command not found"), 127);
	if ((!ft_strncmp(ast->args[0], "./", 2) || !ft_strncmp(ast->args[0], "../",
				3) || ft_strchr(ast->args[0], '/'))
		&& access(ast->args[0], F_OK))
		return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
				strerror(errno)), 127);
	else if ((!ft_strncmp(ast->args[0], "./", 2) || !ft_strncmp(ast->args[0],
				"../", 3) || ft_strchr(ast->args[0], '/'))
		&& S_ISDIR(path_stat.st_mode))
		return (ft_dprintf(2, "minishell: %s: Is a directory\n", ast->args[0]),
			126);
	else if (!access(ast->args[0], F_OK) && ft_strchr(ast->args[0], '/')
		&& S_ISDIR(path_stat.st_mode))
		return (ft_dprintf(2, "minishell: %s: Is a directory\n", ast->args[0]),
			126);
	else if ((!ft_strncmp(ast->args[0], "./", 2) || !ft_strncmp(ast->args[0],
				"../", 3) || ft_strchr(ast->args[0], '/'))
		&& access(ast->args[0], X_OK))
		return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
				strerror(errno)), 126);
	return (ft_dprintf(2, "minishell: %s: command not found\n", ast->args[0]),
		127);
}

static inline int	ft_child(t_minishell *minishell, t_cmd *cmd,
		t_node_ast *ast)
{
	char	**env;
	char	*bin;
	int		status;

	if (ft_open_redirs(minishell, cmd, ast->redirs))
		return (clear_ast(&minishell->ast), clear_env(minishell->env),
			clear_pid(minishell), 1);
	clear_all_redirs(minishell->ast);
	env = ft_get_env(minishell);
	bin = 0;
	status = 0;
	if (ast->args && ast->args[0])
	{
		bin = ft_get_bin(minishell, ast->args[0]);
		if (bin)
			execve(bin, ast->args, env);
		status = ft_handle_err(ast);
	}
	ft_free_tab(env);
	if (bin)
		free(bin);
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

static inline void	sig_fork(t_minishell *minishell, int *pipedes,
		t_node_ast *ast, t_cmd *cmd)
{
	signal(SIGINT, SIG_IGN);
	cmd->pid = fork();
	if (!cmd->pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	if (cmd->pid < 0)
		return (ft_dprintf(2, "minishell: fork error\n"),
			clear_exit(minishell));
	if (!cmd->pid)
	{
		clear_pipe(minishell);
		if (pipedes[0] > 0)
			dup2(pipedes[0], 0);
		if (pipedes[1] > 0)
			dup2(pipedes[1], 1);
		ft_close(pipedes[0]);
		ft_close(pipedes[1]);
		exit(ft_child(minishell, cmd, ast));
	}
	else if (cmd->pid)
		minishell->pid_list = add_pid_list(minishell, cmd->pid);
}

void	ft_exec_child(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	int	pipedes[2];

	ft_expand(ast, minishell);
	pipedes[0] = -1;
	pipedes[1] = -1;
	ft_handle_pipe(minishell, cmd, pipedes);
	sig_fork(minishell, pipedes, ast, cmd);
	ft_close(pipedes[0]);
	ft_close(pipedes[1]);
}
