/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 00:14:55 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/09 09:31:37 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_exec_pipe(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd);
static void	ft_exec_and(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd);
static void	ft_exec_or(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd);

static void	ft_exec_cmd(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	ft_expand(ast, minishell);
	if (!ast->args || !ast->args[0])
		ft_exec_child(minishell, ast, cmd);
	else if (!ft_strcmp("cd", ast->args[0]))
		ft_exec_builtin(minishell, ast, cmd, &cd);
	else if (!ft_strcmp("echo", ast->args[0]))
		ft_exec_builtin(minishell, ast, cmd, &echo);
	else if (!ft_strcmp("env", ast->args[0]))
		ft_exec_builtin(minishell, ast, cmd, &env);
	else if (!ft_strcmp("exit", ast->args[0]))
	{
		if (cmd->pipe_type)
			return ;
		ft_exec_builtin(minishell, ast, cmd, &exit_minishell);
	}
	else if (!ft_strcmp("export", ast->args[0]))
		ft_exec_builtin(minishell, ast, cmd, &export);
	else if (!ft_strcmp("pwd", ast->args[0]))
		ft_exec_builtin(minishell, ast, cmd, &pwd);
	else if (!ft_strcmp("unset", ast->args[0]))
		ft_exec_builtin(minishell, ast, cmd, &unset);
	else
		ft_exec_child(minishell, ast, cmd);
}

static void	ft_exec_pipe(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
		&ft_exec_and};
	int				pipedes[2];
	t_cmd			actual;

	(void)cmd;
	if (pipe(pipedes))
	{
		ft_dprintf(2, "minishell: pipe error");
		clear_exit(minishell);
	}
	minishell->pipe_list = add_pipe_list(minishell, pipedes[0],
		pipedes[1]);
	actual = (t_cmd){0, 1, 1, -1};
	exec_fct[ast->left_child->type](minishell, ast->left_child, &actual);
	actual.pipe_type = 2;
	if (cmd->pipe_type == 1)
		actual.pipe_type = 3;
	ft_exec_cmd(minishell, ast->right_child, &actual);
}

static void	ft_exec_or(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
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

static void	ft_exec_and(t_minishell *minishell, t_node_ast *ast, t_cmd *cmd)
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

void	ft_exec(t_minishell *minishell)
{
	const t_fct_ptr	exec_fct[4] = {&ft_exec_cmd, &ft_exec_pipe, &ft_exec_or,
		&ft_exec_and};
	t_cmd			cmd;

	if (!minishell || !minishell->ast)
		return ;
	cmd = (t_cmd){0, 1, 0, -1};
	exec_fct[minishell->ast->type](minishell, minishell->ast, &cmd);
	ft_wait(minishell);
}
