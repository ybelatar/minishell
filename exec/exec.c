/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:57:35 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/09 05:17:00 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			ft_wait(t_minishell *minishell);

// static void	ft_print_redirs(t_node_ast *ast)
// {
// 	t_redir_list	*current;

// 	if (!ast)
// 		return ;
// 	current = ast->redirs;
// 	while (current)
// 	{
// 		printf("redir = file : %s type : %d\n", current->file, current->type);
// 		current = current->next_redir;
// 	}
// }

// static void	ft_print_type(t_node_ast *ast)
// {
// 	if (!ast)
// 		return ;
// 	printf("type = %d\n", ast->type);
// }

// static void	ft_print_args(t_node_ast *ast)
// {
// 	char	**s;

// 	if (!ast)
// 		return ;
// 	if (!ast->args)
// 		return ;
// 	s = ast->args;
// 	while (*s)
// 		printf("arg = %s\n", *(s++));
// }

// static void	ft_print_ast(t_node_ast *ast)
// {
// 	if (!ast)
// 		return ;
// 	ft_print_args(ast);
// 	ft_print_redirs(ast);
// 	ft_print_type(ast);
// 	ft_print_ast(ast->left_child);
// 	ft_print_ast(ast->right_child);
// }

static inline void	sig_fork(int *pid)
{
	signal(SIGINT, fork_sig_handler);
	signal(SIGQUIT, fork_sig_handler);
	*pid = fork();
}

static inline int	ft_handle_err(t_node_ast *ast)
{
	struct stat	path_stat;

	stat(ast->args[0], &path_stat);
	if (!access(ast->args[0], F_OK) && access(ast->args[0], X_OK))
		return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
				strerror(errno)), 126);
	else if ((!ft_strncmp(ast->args[0], "./", 2)
			|| ft_get_last_char(ast->args[0]) == '/') && access(ast->args[0],
			F_OK))
		return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
				strerror(errno)), 127);
	else if (!access(ast->args[0], F_OK) && (!ft_strncmp(ast->args[0], "./", 2)
			|| ft_get_last_char(ast->args[0]) == '/')
		&& S_ISDIR(path_stat.st_mode))
		return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
				"Is a directory"), 126);
	else if (ft_strncmp(ast->args[0], "./", 2))
		return (ft_dprintf(2, "minishell: %s: %s\n", ast->args[0],
				"command not found"), 127);
	return (0);
}

static inline void	ft_child(t_minishell *minishell, t_node_ast *ast,
		t_cmd *cmd)
{
	char	**env;
	char	*bin;
	int		status;

	(void)cmd;
	// ft_close(minishell->in);
	// ft_close(minishell->out);
	env = ft_get_env(minishell);
	bin = 0;
	status = 0;
	if (ast->args)
	{
		execve(ast->args[0], ast->args, env);
		bin = ft_get_bin(minishell, ast->args[0]);
		if (bin)
			execve(bin, ast->args, env);
		status = ft_handle_err(ast);
	}
	clear_ast(&minishell->ast);
	clear_env(minishell->env);
	clear_pid(minishell);
	// display_pipes(minishell, "after exec", cmd->pipe_type);
	ft_free_tab(env);
	if (bin)
		free(bin);
	exit(status);
}

void	ft_exec_cmd(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	t_pipe_list	*lst;
	int			pid;
	int			pipe_in;
	int			pipe_out;

	ft_expand(ast, minishell);
	// dup2(minishell->in, 0);
	// dup2(minishell->out, 1);
	// display_pipes(minishell, "before dup", cmd->pipe_type);
	pipe_in = -1;
	pipe_out = -1;
	if (cmd->pipe_type)
	{
		lst = get_last_pipe(minishell->pipe_list);
		if (cmd->pipe_type == 1)
		{
			pipe_out = lst->out;
			lst->out = -1;
		}
		else if (cmd->pipe_type == 2)
		{
			pipe_in = lst->in;
			lst->in = -1;
			remove_last_pipe(minishell->pipe_list);
		}
		else if (cmd->pipe_type == 3)
		{
			pipe_in = lst->in;
			lst->in = -1;
			lst = remove_last_pipe(minishell->pipe_list);
			pipe_out = lst->out;
			lst->out = -1;
		}
	}
	// if (!ft_strcmp(ast->args[0], "cd"))
	// {
	// 	if (cmd->pipe_type)
	// 		return ;
	// 	g_status = cd(ast->args + 1, minishell);
	// }
	sig_fork(&pid);
	if (pid < 0)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		return ;
	}
	if (pid == 0)
	{
		clear_pipe(minishell);
		if (pipe_in > 0)
		{
			dup2(pipe_in, 0);
			ft_close(pipe_in);
		}
		if (pipe_out > 0)
		{
			dup2(pipe_out, 1);
			ft_close(pipe_out);
		}
		// display_pipes(minishell, "after dup", cmd->pipe_type);
		if (ft_open_redirs(minishell, cmd, ast->redirs))
		{
			clear_ast(&minishell->ast);
			clear_env(minishell->env);
			clear_pid(minishell);
			exit(errno);
		}
		ft_child(minishell, ast, cmd);
	}
	if (pid)
		minishell->pid_list = add_pid_list(minishell->pid_list, pid);
	ft_close(pipe_in);
	ft_close(pipe_out);
}

void	ft_exec_pipe(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
			&ft_exec_and};
	int				pipedes[2];
	t_cmd			actual;

	(void)cmd;
	if (pipe(pipedes))
	{
		perror("minishell");
		return ;
	}
	minishell->pipe_list = add_pipe_list(minishell->pipe_list, pipedes[0],
			pipedes[1]);
	actual = (t_cmd){0, 1, 1};
	// display_pipes(minishell, "before fct", cmd->pipe_type);
	exec_fct[ast->left_child->type](minishell, ast->left_child, &actual);
	// ft_close(pipedes[1]);
	actual.pipe_type = 2;
	if (cmd->pipe_type == 1)
		actual.pipe_type = 3;
	// display_pipes(minishell, "after fct", cmd->pipe_type);
	ft_exec_cmd(minishell, ast->right_child, &actual);
	// ft_close(pipedes[0]);
}

void	ft_exec_or(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
			&ft_exec_and};
	t_node_ast		*left;
	t_node_ast		*right;

	cmd->pipe_type = 0;
	left = ast->left_child;
	exec_fct[left->type](minishell, left, cmd);
	ft_wait(minishell);
	if (!g_status)
		return ;
	right = ast->right_child;
	exec_fct[right->type](minishell, right, cmd);
}

void	ft_exec_and(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
			&ft_exec_and};
	t_node_ast		*left;
	t_node_ast		*right;

	cmd->pipe_type = 0;
	left = ast->left_child;
	exec_fct[left->type](minishell, left, cmd);
	ft_wait(minishell);
	if (g_status)
		return ;
	right = ast->right_child;
	exec_fct[right->type](minishell, right, cmd);
}

static void	ft_wait(t_minishell *minishell)
{
	t_pid_list	*current;
	int			status;

	if (!minishell->pid_list)
		return ;
	current = minishell->pid_list;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
			{
				ft_dprintf(2, "Quit (core dumped)\n");
				g_status = 131;
			}
		}
		current = current->next;
	}
	clear_pid(minishell);
}

/*	Executes all commands in an Abstract Syntax Tree (AST)
	takes a t_minishell struct pointer containing a t_ast struct pointer
	as an argument.
*/
void	ft_exec(t_minishell *minishell)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
			&ft_exec_and};
	t_cmd			cmd;

	if (!minishell || !minishell->ast)
		return ;
	cmd = (t_cmd){0, 1, 0};
	exec_fct[minishell->ast->type](minishell, minishell->ast, &cmd);
	ft_wait(minishell);
}
