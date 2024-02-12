/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 05:11:10 by wouhliss          #+#    #+#             */
/*   Updated: 2024/02/12 05:21:07 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

static inline int	wild_sort(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	while (s1[i] && s2[i])
	{
		if (ft_tolower(s1[i]) == ft_tolower(s2[i]))
			return (s2[i] - s1[i]);
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	return (ft_tolower(s1[i]) - ft_tolower(s2[i]));
}

static inline void	insert(char **tab, int j)
{
	int		k;
	char	*tmp;

	k = j;
	while (k > 0 && wild_sort(tab[k], tab[k - 1]) > 0)
	{
		tmp = tab[k];
		tab[k] = tab[k - 1];
		tab[k - 1] = tmp;
		k--;
	}
}

char	**sort_tab(char **tab)
{
	int	i;
	int	len;

	i = 1;
	len = plen(tab);
	while (i < len)
		insert(tab, i++);
	return (tab);
}
