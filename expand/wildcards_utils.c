/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:01:39 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 05:10:00 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increm(int *i, int *j)
{
	*i = *i + 1;
	*j = *j + 1;
}

int	match_core(char *str, char *pattern, int f[4])
{
	while (str[f[0]])
	{
		if (pattern[f[1]] == '*' || pattern[f[1]] == str[f[0]])
		{
			if (pattern[f[1]] == '*')
			{
				f[2] = f[1]++;
				f[3] = f[0];
			}
			else
				increm(&f[1], &f[0]);
		}
		else if (f[2] != -1)
		{
			f[1] = f[2] + 1;
			f[0] = ++f[3];
		}
		else
			return (0);
	}
	while (pattern[f[1]] == '*')
		f[1]++;
	return (pattern[f[1]] == '\0');
}

int	ft_ismatch(char *str, char *pattern)
{
	int	f[4];

	if (!ft_strcmp(str, ".") || !ft_strcmp(str, ".."))
		return (0);
	f[0] = 0;
	f[1] = 0;
	f[2] = -1;
	f[3] = -1;
	return (match_core(str, pattern, f));
}

char	**expanded_wildcard(char *str)
{
	char			**tab;
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return (perror("opendir"), NULL);
	tab = NULL;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_ismatch(entry->d_name, str))
			tab = join_tab(tab, ft_strdup(entry->d_name));
		entry = readdir(dir);
	}
	if (!tab)
		tab = join_tab(tab, ft_strdup(str));
	closedir(dir);
	return (tab);
}
