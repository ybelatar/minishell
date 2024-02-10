/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:03:59 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/10 08:15:43 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_read(t_minishell *minishell, char *limiter, int out)
{
	char	*s;
	char	*expanded;
	int led;

	led = 0;
	if (!ft_strchr(limiter, '"') && !ft_strchr(limiter, '\''))
		led = 1;
	expanded = without_quotes(limiter, 0);
	s = readline("> ");	
	while (s)
	{
		if (!ft_strcmp(s, expanded))
		{
			(free(s), free(expanded));
			close(out);
			return ;
		}
		if (led)
			s = expand_env_one(s, minishell, 1);
		ft_dprintf(out, "%s\n", s);
		free(s);
		s = readline("> ");
	}
	free(expanded);
	ft_dprintf(2, H, "minishell:", limiter);
	close(out);
}
