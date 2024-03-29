/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 23:46:18 by pibosc            #+#    #+#             */
/*   Updated: 2024/01/10 22:49:27 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	word_count(char const *s, char c)
{
	int	count;
	int	on_word;

	count = 0;
	on_word = 0;
	while (*s)
	{
		if (*s == c)
		{
			if (on_word)
			{
				count++;
				on_word = 0;
			}
		}
		else
			on_word = 1;
		s++;
	}
	return (count + on_word);
}

static void	*free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
	return (NULL);
}

static char	*copy_next_word(char const *s, char c, int *i)
{
	char	*word;
	int		tmp;
	int		j;

	while (s[*i] && s[*i] == c)
		(*i)++;
	tmp = *i;
	while (s[*i] && s[*i] != c)
		(*i)++;
	word = malloc((*i - tmp + 1) * sizeof(char));
	if (!word)
		return (NULL);
	j = 0;
	while (j < *i - tmp)
	{
		word[j] = s[tmp + j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		words;
	int		word;
	int		i;

	if (!s)
		return (NULL);
	words = word_count(s, c);
	split = malloc((words + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	word = 0;
	while (word < words)
	{
		split[word] = copy_next_word(s, c, &i);
		if (!split[word])
			return (free_tab(split));
		word++;
	}
	split[words] = NULL;
	return (split);
}
