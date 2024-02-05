/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 17:36:27 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/05 17:50:11 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*rm_quotes(char *str)
{
	char	*content_trim;
	int		len;
	int		i;

	len = ft_strlen(str);
	content_trim = malloc(len - 1);
	if (!content_trim)
		return (NULL);
	i = 0;
	while (i < len - 2)
	{
		content_trim[i] = str[i + 1];
		i++;
	}
	content_trim[i] = 0;
	free(str);
	return (content_trim);
}

void	expand_pretokens(t_pretoken *pretokens, t_minishell *minishell)
{
	(void)minishell;
	while (pretokens)
	{
		if (pretokens->type == WORD)
		{
			if (pretokens->content[0] == '\'')
				pretokens->content = rm_quotes(pretokens->content);
			else if (pretokens->content[0] == '"')
			{
				pretokens->content = rm_quotes(pretokens->content);
				expand_env_dq(pretokens, minishell);
			}
			else
			{
				expand_wildcard(pretokens);
				expand_env(pretokens, minishell);
			}
		}
		pretokens = pretokens->next_pretoken;
	}
}
