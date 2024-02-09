/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:44:00 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/09 09:46:43 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_read_heredoc(t_heredoc *heredoc, const char *limiter)
{
	ft_get_rand(heredoc);
	if (!heredoc->path)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		return (1);
	}
	heredoc->out = open(heredoc->path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc->out < 0)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		free(heredoc->path);
		return (1);
	}
	heredoc->in = open(heredoc->path, O_RDONLY);
	if (heredoc->in < 0)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		close(heredoc->out);
		return (1);
	}
	unlink(heredoc->path);
	ft_read(limiter, heredoc->out);
	return (0);
}

// void	ft_print_rand(void)
// {
// 	char	rand[33];

// 	ft_get_rand(rand);
// 	printf("%s\n", rand);
// }
