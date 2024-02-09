/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_no_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 03:14:32 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 04:19:54 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void skip_quotes(char *str, int *i, char q, int offset)
{
    (*i)++;
    while (str[*i + offset] != q)
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
		if (str[i] == c)
		{
			while (str[i] == c)
				i++;
		}
		else
		{
			count++;
			while (str[i] != c && str[i])
            {
                if (str[i] == '"' || str[i] == '\'')
                    skip_quotes(str, &i, str[i], 0);
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
	while (str[*ptr] && str[*ptr] == c)
		*ptr = *ptr + 1;
	if (!str[*ptr])
		return (NULL);
	while (str[*ptr + len_word] && str[*ptr + len_word] != c)
    {
        if (str[*ptr + len_word] == '"' || str[*ptr + len_word] == '\'')
            skip_quotes(str, &len_word, str[*ptr + len_word], *ptr);
		len_word++;
    }
	word = malloc((len_word + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (i < len_word)
	{
		word[i] = str[*ptr];
		i++;
		*ptr = *ptr + 1;
	}
	word[i] = 0;
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

void    split_no_quotes(char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        args = insert_tab_in_tab(args, ft_split_no_quotes(args[i], ' '), &i);
        i++;
    }
}