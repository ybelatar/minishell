/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:03:59 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/10 02:13:06 by wouhliss         ###   ########.fr       */
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
	ft_dprintf(2, H, "minishell:", limiter);
	close(out);
}
