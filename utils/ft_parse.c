/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:03:59 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/09 09:53:11 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_read(const char *limiter, int out)
{
	char	*s;

	s = readline("> ");
	while (s)
	{
		if (!ft_strcmp(s, limiter))
		{
			free(s);
			close(out);
			return ;
		}
		write(out, s, ft_strlen(s));
		write(out, "\n", 1);
		free(s);
		s = readline("> ");
	}
	ft_dprintf(2, HEREDOC_EOF, limiter);
	close(out);
}
