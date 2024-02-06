/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:44:00 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/06 15:41:08 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_get_rand(char *filename)
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
			filename[bytes++] = buff[r--];
	}
	close(fd);
	filename[bytes] = 0;
}

void	ft_read_heredoc(const char *limiter)
{
	t_heredoc	heredoc;

	ft_get_rand(heredoc.rand);
	heredoc.path = ft_strjoin("/tmp/", rand);
	if (!heredoc.path)
	{
		return ;
	}
	heredoc.out = open(heredoc.path, O_CREAT, O_WRONLY, O_TRUNC, 0644);
	if (heredoc.out < 0)
	{
		free(heredoc.path);
		return ;
	}
	heredoc.in = open(heredoc.path, O_RDONLY);
	if (heredoc.in < 0)
	{
		free(heredoc.path);
		close(heredoc.out);
		return ;
	}
	unlink(heredoc.path);
	free(heredoc.path);
	ft_read(limiter, heredoc.out);
}

// void	ft_print_rand(void)
// {
// 	char	rand[33];

// 	ft_get_rand(rand);
// 	printf("%s\n", rand);
// }
