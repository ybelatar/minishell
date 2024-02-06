/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:57:35 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/06 22:21:16 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static inline void	ft_child(t_cmd *cmd, t_minishell *minishell, t_node_ast *ast)
{
	if (cmd->fd_in != 0)
		dup2(cmd->fd_in, 0);
	if (cmd->fd_out != 1)
		dup2(cmd->fd_out, 1);
	ft_close(minishell->in);
	ft_close(minishell->out);
	execve(ast->args[0], ast->args, 0);
	ft_dprintf(2, "%s: %s\n", ast->args[0], strerror(errno));
	clear_ast(&ast);
	clear_env(minishell->env);
	exit(127);
}

static void	ft_exec_cmd(t_minishell *minishell, t_node_ast *ast)
{
	t_cmd	cmd;

	// ft_expand(ast);
	cmd = (t_cmd){0, 1, -1};
	if (ft_open_redirs(minishell, &cmd, ast->redirs))
		return ;
	sig_fork(&cmd);
	if (cmd.pid < 0)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		return ;
	}
	if (cmd.pid == 0)
		ft_child(&cmd, minishell, ast);
	add_pid_list(minishell->pid_list, cmd.pid);
}

static void	ft_exec_pipe(t_minishell *minishell, t_node_ast *ast)
{
	(void)minishell;
	(void)ast;
}

static void	ft_exec_or(t_minishell *minishell, t_node_ast *ast)
{
	(void)minishell;
	(void)ast;
}

static void	ft_exec_and(t_minishell *minishell, t_node_ast *ast)
{
	(void)minishell;
	(void)ast;
}

static void	ft_wait(t_minishell *minishell)
{
	t_pid_list	*current;

	current = minishell->pid_list;
	while (current)
	{
		waitpid(current->pid, &g_status, 0);
		current = current->next;
	}
}

/*	Executes all commands in an Abstract Syntax Tree (AST)
	takes a t_minishell struct pointer containing a t_ast struct pointer
	as an argument.
*/
void	ft_exec(t_minishell *minishell)
{
	if (!minishell || !minishell->ast)
		return ;
	if (minishell->ast->type == T_CMD)
		ft_exec_cmd(minishell, minishell->ast);
	else if (minishell->ast->type == T_PIPE)
		ft_exec_pipe(minishell, minishell->ast);
	else if (minishell->ast->type == T_OR)
		ft_exec_or(minishell, minishell->ast);
	else if (minishell->ast->type == T_AND)
		ft_exec_and(minishell, minishell->ast);
}
