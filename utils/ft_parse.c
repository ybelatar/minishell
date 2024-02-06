/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:03:59 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/06 15:37:36 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_read(const char *limiter, int out)
{
	char		*s;

	s = readline("heredoc> ");
	while (s)
	{
		add_history(s);
		if (ft_strcmp(s, limiter) == '\n')
		{
			free(s);
			close(out);
			return ;
		}
		write(out, s, ft_strlen(s));
		free(s);
		s = readline("heredoc> ");
	}
	close(out);
}
