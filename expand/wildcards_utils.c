/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:01:39 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 04:23:47 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	a(int *i, int *j)
{
	*i = *i + 1;
	*j = *j + 1;
}
/*TODO utiliser indic*/
int	match_core(char *str, char *pattern, char *indic, int f[4])
{
    (void)indic;
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
				a(&f[1], &f[0]);
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

int	ft_ismatch(char *str, char *pattern, char *indic)
{
	int	f[4];

	if (!ft_strcmp(str, ".") || !ft_strcmp(str, ".."))
		return (0);
	f[0] = 0;
	f[1] = 0;
	f[2] = -1;
	f[3] = -1;
	return (match_core(str, pattern, indic, f));
}

char *gen_indic(char *str)
{
    int i;
    char c;
    char *res;

    res = malloc(ft_strlen(str) + 1);
    if (!res)
        return (NULL);
    i = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            c = str[i];
            res[i++] = 'Q';
            while (str[i] != c)
                res[i++] = 'N';
            res[i++] = 'Q';
        }
        else
            res[i++] = 'O';
    }
    res[i] = 0;
    return (res);
}

char	**expanded_wildcard(char *str)
{
	char			**tab;
	DIR				*dir;
	struct dirent	*entry;
	char			*indic;

	dir = opendir(".");
	if (!dir)
		return (perror("opendir"), NULL);
	tab = NULL;
	entry = readdir(dir);
    indic = gen_indic(str);
    if (!indic)
		return (NULL);
	while (entry)
	{
		if (ft_ismatch(entry->d_name, str, indic))
			tab = join_tab(tab, ft_strdup(entry->d_name));
		entry = readdir(dir);
	}
	closedir(dir);
	return (tab);
}
