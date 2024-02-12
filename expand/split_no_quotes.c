/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_no_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 03:14:32 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 05:05:19 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_quotes(char *str, int *i, char q, int offset)
{
	(*i)++;
	while (str[*i + offset] && str[*i + offset] != q)
		(*i)++;
}

static int	count_words(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c || str[i] == '\t')
			while (str[i] == c || str[i] == '\t')
				i++;
		else
		{
			count++;
			while (str[i] && str[i] != c && str[i] != '\t')
			{
				if (str[i] == '"' || str[i] == '\'')
					skip_quotes(str, &i, str[i], 0);
				if (!str[i])
					return (count);
				i++;
			}
		}
	}
	return (count);
}

static char	*get_next_word(char *str, int *ptr, char c)
{
	int		i;
	int		len_word;
	char	*word;

	i = 0;
	len_word = 0;
	while (str[*ptr] && (str[*ptr] == c || str[*ptr] == '\t'))
		*ptr = *ptr + 1;
	if (!str[*ptr])
		return (NULL);
	while (str[*ptr + len_word] && str[*ptr + len_word] != c && str[*ptr
			+ len_word] != '\t')
	{
		if (str[*ptr + len_word] == '"' || str[*ptr + len_word] == '\'')
			skip_quotes(str, &len_word, str[*ptr + len_word], *ptr);
		if (!str[*ptr + len_word])
			break ;
		len_word++;
	}
	word = ft_calloc((len_word + 1), sizeof(char));
	if (!word)
		return (NULL);
	while (i < len_word)
		word[i++] = str[(*ptr)++];
	return (word);
}

char	**ft_split_no_quotes(char *s, char c)
{
	int		i;
	int		j;
	int		count;
	char	**words;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	count = count_words(s, c);
	words = malloc((count + 1) * sizeof(char *));
	if (!words)
		return (NULL);
	while (i < count)
	{
		words[i] = get_next_word(s, &j, c);
		if (!words[i])
			return (NULL);
		i++;
	}
	words[i] = 0;
	return (words);
}

void	split_no_quotes(t_node_ast *node)
{
	int	i;

	if (!node || !node->args)
		return ;
	i = 0;
	while (node->args[i])
	{
		node->args = insert_tab_in_tab(node->args,
				ft_split_no_quotes(node->args[i], ' '), &i);
	}
}
