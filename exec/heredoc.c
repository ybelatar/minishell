/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:44:00 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/11 21:50:19 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_heredocs(t_minishell *minishell, t_node_ast *ast)
{
	t_redir_list	*tmp;
	t_heredoc		heredoc;

	if (!ast)
		return ;
	if (ast->type == T_CMD)
	{
		tmp = ast->redirs;
		while (tmp && !minishell->stop)
		{
			if (tmp->type == R_HEREDOC)
			{
				if (!ft_read_heredoc(minishell, &heredoc, tmp->file))
					tmp->fd = heredoc.in;
				else
					minishell->stop = 1;
			}
			tmp = tmp->next_redir;
		}
	}
	get_heredocs(minishell, ast->left_child);
	get_heredocs(minishell, ast->right_child);
}

static void	ft_get_rand(t_heredoc *heredoc)
{
	int		fd;
	int		r;
	int		bytes;
	char	buff[33];

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		return ;
	bytes = 0;
	while (bytes < 32)
	{
		r = read(fd, buff, 32);
		if (r < 2)
			return ;
		--r;
		while (((buff[r] >= 'a' && buff[r] <= 'z') || (buff[r] >= 'A'
					&& buff[r] <= 'Z') || (buff[r] >= '0' && buff[r] <= '9'))
			&& bytes < 32 && r)
			heredoc->rand[bytes++] = buff[r--];
	}
	close(fd);
	heredoc->rand[bytes] = 0;
	heredoc->path = heredoc->rand;
}

int	ft_read_heredoc(t_minishell *minishell, t_heredoc *heredoc, char *limiter)
{
	ft_get_rand(heredoc);
	if (!heredoc->path)
	{
		g_status = 1;
		return (ft_dprintf(2, "minishell: %s\n", strerror(errno)), 1);
	}
	heredoc->out = open(heredoc->path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc->out < 0)
	{
		g_status = 1;
		free(heredoc->path);
		return (ft_dprintf(2, "minishell: %s\n", strerror(errno)), 1);
	}
	heredoc->in = open(heredoc->path, O_RDONLY);
	if (heredoc->in < 0)
	{
		g_status = 1;
		close(heredoc->out);
		return (ft_dprintf(2, "minishell: %s\n", strerror(errno)), 1);
	}
	unlink(heredoc->path);
	if (ft_read(minishell, limiter, heredoc->out, heredoc->in))
		return (ft_close(heredoc->in), 1);
	return (0);
}

// void	ft_print_rand(void)
// {
// 	char	rand[33];

// 	ft_get_rand(rand);
// 	printf("%s\n", rand);
// }
