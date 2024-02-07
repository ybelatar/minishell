/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:57:35 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/07 07:01:12 by wouhliss         ###   ########.fr       */
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

static inline void	sig_fork(t_cmd *cmd)
{
	signal(SIGINT, fork_sig_handler);
	signal(SIGQUIT, fork_sig_handler);
	cmd->pid = fork();
}

static inline int	ft_handle_err(t_node_ast *ast)
{
	struct stat	path_stat;
	int			status;

	status = 127;
	stat(ast->args[0], &path_stat);
	if (!access(ast->args[0], F_OK) && access(ast->args[0], X_OK))
	{
		ft_dprintf(2, "minishell: %s: %s\n", ast->args[0], strerror(errno));
		status = 126;
	}
	else if (!ft_strncmp(ast->args[0], "./", 2) && access(ast->args[0], F_OK))
		ft_dprintf(2, "minishell: %s: %s\n", ast->args[0], strerror(errno));
	else if (!ft_strncmp(ast->args[0], "./", 2) && !access(ast->args[0], F_OK)
		&& S_ISDIR(path_stat.st_mode))
	{
		ft_dprintf(2, "minishell: %s: %s\n", ast->args[0], "Is a directory");
		status = 126;
	}
	else
		ft_dprintf(2, "%s: %s\n", ast->args[0], "command not found");
	return (status);
}

static inline void	ft_child(t_minishell *minishell, t_node_ast *ast)
{
	char	**env;
	char	*bin;
	int		status;

	ft_close(minishell->in);
	ft_close(minishell->out);
	env = ft_get_env(minishell);
	execve(ast->args[0], ast->args, env);
	bin = ft_get_bin(minishell, ast->args[0]);
	if (bin)
		execve(bin, ast->args, env);
	status = ft_handle_err(ast);
	clear_ast(&minishell->ast);
	clear_env(minishell->env);
	clear_pid(minishell);
	ft_free_tab(env);
	if (bin)
		free(bin);
	exit(status);
}

void	ft_exec_cmd(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	// ft_expand(ast);
	dup2(minishell->out, 1);
	dup2(minishell->in, 0);
	if (ft_open_redirs(minishell, cmd, ast->redirs))
		return ;
	sig_fork(cmd);
	if (cmd->pid < 0)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		return ;
	}
	if (cmd->pid == 0)
		ft_child(minishell, ast);
	if (cmd->pid)
		minishell->pid_list = add_pid_list(minishell->pid_list, cmd->pid);
}

void	ft_exec_pipe(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	// const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
	//		&ft_exec_and};
	// int				pipedes[2];
	// if (pipe(pipedes))
	// {
	// 	perror("bash");
	// 	if (cmd)
	// 		if (cmd->fd_out != 1)
	// 			ft_close(cmd->fd_out);
	// 	return ;
	// }
	(void)minishell;
	(void)cmd;
	(void)ast;
	// cmd->fd_out = pipedes[1];
	// exec_fct[ast->left_child->type](minishell, ast->left_child, cmd);
	// ft_close(cmd->fd_out);
	// cmd->fd_in = pipedes[0];
	// exec_fct[ast->right_child->type](minishell, ast->right_child, cmd);
	// ft_close(cmd->fd_in);
}

void	ft_exec_or(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
			&ft_exec_and};
	t_node_ast		*left;
	t_node_ast		*right;

	left = ast->left_child;
	exec_fct[left->type](minishell, left, cmd);
	ft_wait(minishell);
	if (!g_status)
		return ;
	right = ast->right_child;
	exec_fct[right->type](minishell, right, cmd);
	ft_wait(minishell);
}

void	ft_exec_and(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
			&ft_exec_and};
	t_node_ast		*left;
	t_node_ast		*right;

	left = ast->left_child;
	exec_fct[left->type](minishell, left, cmd);
	ft_wait(minishell);
	if (g_status)
		return ;
	right = ast->right_child;
	exec_fct[right->type](minishell, right, cmd);
	ft_wait(minishell);
}

static void	ft_wait(t_minishell *minishell)
{
	t_pid_list	*current;
	int			status;

	current = minishell->pid_list;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		current = current->next;
	}
	g_status = WEXITSTATUS(status);
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
	cmd = (t_cmd){0, 1, -1};
	exec_fct[minishell->ast->type](minishell, minishell->ast, &cmd);
	if (minishell->pid_list)
		ft_wait(minishell);
}
