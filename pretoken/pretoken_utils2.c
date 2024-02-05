/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pretoken_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 02:22:03 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/05 17:50:11 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_quoted(char *str, int *i)
{
	char	*res;
	int		j;
	int		len;
	char	quote;

	j = 0;
	len = 1;
	quote = str[*i];
	while (str[*i + len] && str[*i + len] != quote)
		len++;
	len++;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	while (j < len)
	{
		res[j] = str[*i];
		j++;
		*i = *i + 1;
	}
	res[j] = 0;
	return (res);
}

int	is_in_charset(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*get_word(char *str, int *i)
{
	char	*res;
	int		j;
	int		len;

	j = 0;
	len = 0;
	while (str[*i + len] && !is_in_charset(str[*i + len], "|&><'\" ()"))
		len++;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	while (j < len)
	{
		res[j] = str[*i];
		j++;
		*i = *i + 1;
	}
	res[j] = 0;
	return (res);
}

t_pretoken	*new_pretoken(char *content, t_pretoken_type type)
{
	t_pretoken	*new;

	new = malloc(sizeof(t_pretoken));
	if (!new)
		return (NULL);
	new->content = content;
	new->type = type;
	new->wild = 0;
	new->next_pretoken = NULL;
	return (new);
}
